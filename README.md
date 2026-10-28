# Timetabling

A console timetabling system in C++17. Admins build a university timetable of lectures, labs and
tutorials and are warned about room, lecturer and student-group clashes. Students log in to view
or export their group's timetable.

## Build and run

You need CMake 3.16 or later and a C++17 compiler (tested with GCC 15.1; there are no
platform-specific dependencies).

```bash
cmake -B build && cmake --build build
./build/timetable          # interactive menus
ctest --test-dir build     # or ./build/timetable_tests
sh examples/run_example.sh # scripted demo against the sample data
```

The program reads and writes `data.dat` (sessions) and `students.dat` (student → group) in the
current directory. If there are no sessions, it creates one sample entry. The demo admin login is
`admin` / `admin123`.

## Example

[`examples/data.dat`](examples/data.dat) holds five sessions, one line per session:
`type,module,lecturer,room,week,day,time,group1,group2,...`

```
Lecture,CS201,Dr Patel,A101,1,Monday,09:00,GroupA,GroupB
Lab,CS202,Dr Okafor,B204,1,Monday,11:00,GroupA
Lab,CS202,Dr Okafor,B205,1,Monday,13:00,GroupB
Tutorial,CS203,Dr Okafor,C010,1,Tuesday,10:00,GroupB
Lecture,CS204,Dr Okafor,A101,1,Tuesday,10:00,GroupA
```

[`examples/run_example.sh`](examples/run_example.sh) feeds
[`demo_input.txt`](examples/demo_input.txt) to the program. The script logs in as admin, lists
the entries and runs "Check for Conflicts". It then logs in as student `s1001` (GroupA) and
views that timetable. Below is an excerpt of the actual output; the full transcript is in
[`examples/demo_output.txt`](examples/demo_output.txt).

```
CURRENT TIMETABLE ENTRIES
ID	Module		Type	Day	Time	Room	Groups
1	CS201	Lecture	Monday	09:00	A101	GroupA, GroupB
2	CS202	Lab	Monday	11:00	B204	GroupA
3	CS202	Lab	Monday	13:00	B205	GroupB
4	CS203	Tutorial	Tuesday	10:00	C010	GroupB
5	CS204	Lecture	Tuesday	10:00	A101	GroupA
...
Conflicts for entry 4:
Module: CS203, Tuesday 10:00 in C010
- CONFLICT WITH: CS204, Tuesday 10:00 in A101

Conflicts for entry 5:
Module: CS204, Tuesday 10:00 in A101
- CONFLICT WITH: CS203, Tuesday 10:00 in C010
...
Your Timetable (GroupA):
Week	Day		Time	Module		Type	Room	Lecturer
1	Monday	09:00	CS201	Lecture	A101	Dr Patel
1	Monday	11:00	CS202	Lab	B204	Dr Okafor
1	Tuesday	10:00	CS204	Lecture	A101	Dr Okafor
```

Entries 4 and 5 are in different rooms for different groups, but both are taught by Dr Okafor at
the same time, so the system reports a lecturer clash.

## Design

```
User (abstract: login(), showMenu())
 ├── Admin    – data management and timetable menus
 └── Student  – view / export own group's timetable

TimetableSystem  – owns all data; menus call into it through a TimetableSystem*
Session          – one timetabled event, with toCSV()/fromCSV()
Module           – code, name, assigned lecturers
```

| File | Responsibility |
|---|---|
| `src/TimetableSystem.{h,cpp}` | Data store, clash detection, persistence, top-level menu |
| `src/Admin.{h,cpp}` | Admin menus: reference data plus create/edit/delete/check entries |
| `src/Student.{h,cpp}` | Student login, timetable view, CSV export |
| `src/Session.{h,cpp}` | Session record and CSV (de)serialisation |
| `src/User.h`, `src/Module.h` | Base class; module record |
| `src/Input.h` | `readMenuChoice()` – robust numeric menu input |

**Data structures** (all in `TimetableSystem`):

- `std::vector<Session> sessions` is the timetable. A session's position in it is the ID the
  admin sees.
- `std::map<std::string, std::vector<Session>> groupTimetables` is a per-group index, so a
  student's view is one map lookup. It holds copies of the sessions and is rebuilt after any edit
  or delete.
- `std::unordered_map<std::string, std::string> studentGroupMap` maps a student ID to a group.
- `std::set<std::string> sessionTypes` and vectors of rooms, lecturers, groups, students and
  modules hold the reference data, with duplicate checks on insert.

**Scheduling approach.** This program does not generate a timetable automatically. The admin
places each session, and the system checks every placement against the existing timetable.
`findConflicts(session, ignoreIndex)` reports an existing session as a clash when its week, day
and time are equal to the new session's and any of these is true:

- it uses the same room;
- it has the same lecturer;
- it shares at least one student group.

When a clash is found, the admin is warned and can still add or save the entry.
`ignoreIndex` lets a stored session be checked without matching itself.

**Time complexity.** *n* is the number of sessions, *g* the number of groups per session and
*G* the number of distinct groups.

| Operation | Cost |
|---|---|
| Add session | O(g) amortised (plus O(log G) per group for the index) |
| Check one session for clashes | O(n · g²), since group overlap is a nested linear search |
| "Check for Conflicts" over the whole timetable | O(n² · g²) |
| Edit / delete a session | O(n · g · log G), which is a full index rebuild |
| Student timetable lookup | O(log G) |

*g* is small in practice, so the whole-timetable check is effectively O(n²).

## Tests

`tests/test_main.cpp` contains **26 tests** using a small built-in test runner with no external
framework. CTest runs them through `ctest --test-dir build`. All 26 pass. They cover:

- `Session` CSV formatting, parsing, round trip and rejection of a non-numeric week
- group timetable indexing after add, edit and delete, and bad-index handling
- clash detection by room, lecturer and shared group; no clash when week, day or time differs;
  one report per clashing session; the same-module regression (see below); `ignoreIndex`
- reference-data validation: duplicate or empty groups, rooms and session types; student-to-group
  and lecturer-to-module assignment rules
- persistence: save/load round trip, and skipping malformed lines in `data.dat`
- menu input: numeric, non-numeric and end of input

## Known limitations

- **No automatic scheduler.** Sessions are placed by hand; the program only detects clashes.
- **Clashes need an exact slot match.** Times are free-text strings and sessions have no
  duration. A 2-hour lecture at `10:00` does not clash with something at `11:00`, and `10:00`
  does not match `10.00`.
- **Only sessions and student→group assignments are saved.** Modules, rooms, lecturers, groups,
  session types and registered students are lost on exit.
- **Editing only changes an entry's session type.** To change anything else, delete the entry
  and re-create it.
- **Not much input validation.** Room, lecturer and group names are not checked against the
  reference lists. Week numbers are not range-checked. Modules can be added twice. Removing a
  group does not update the sessions or students that use it.
- **The whole-timetable check reports each clash twice**, once from each side.
- **No real authentication.** The admin credentials are hard-coded, and students log in by ID
  alone.
- **The CSV format has no quoting**, so a comma inside a name breaks the line.

## What I'd improve now

- Represent a slot as `{week, day, startMinute, endMinute}` and test interval overlap rather than
  string equality.
- Index sessions by `(week, day)` or by room and lecturer, so a whole-timetable check costs
  roughly O(n log n) rather than O(n²). Store indices in `groupTimetables` rather than copies.
- Persist all the reference data, and validate entries against it.
- Separate the menus from `std::cin`/`std::cout` (for example, pass streams in) so the admin
  workflows can be unit-tested the way the core logic is.
- Add an automatic scheduler on top of the existing clash check. One option is greedy placement
  ordered by most-constrained session; another is to treat it as graph colouring or a small
  constraint-satisfaction problem.

## Provenance

Second-year undergraduate project, Nottingham Trent University — graded First. Tidied for
publication in 2026; the original submission is tagged
[`v1-as-submitted`](../../tree/v1-as-submitted).

The tidy-up made these changes:

- added the CMake build, `.clang-format` and the tests;
- fixed the bugs listed in the commit history, including missing clashes between sessions of the
  same module, menus looping forever on non-numeric input, and the edit prompt being skipped;
- removed dead code.

The overall design is unchanged.
