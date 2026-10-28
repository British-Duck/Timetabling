// Plain-assert unit tests for the core timetabling and clash-detection logic.
// Each TEST registers itself; main() runs them all and returns non-zero on any failure.

#include "Input.h"
#include "Session.h"
#include "TimetableSystem.h"

#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct TestCase {
    const char* name;
    std::function<void()> body;
};

std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

int failures = 0;

struct Register {
    Register(const char* name, std::function<void()> body) {
        registry().push_back({name, std::move(body)});
    }
};

#define TEST(name)                                                                                 \
    static void name();                                                                            \
    static Register register_##name(#name, name);                                                  \
    static void name()

#define CHECK(cond)                                                                                \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            std::cerr << "  FAILED: " << #cond << " (" << __FILE__ << ":" << __LINE__ << ")\n";    \
            failures++;                                                                            \
        }                                                                                          \
    } while (0)

Session makeSession(const std::string& module, const std::string& lecturer, const std::string& room,
                    int week, const std::string& day, const std::string& time,
                    std::vector<std::string> groups) {
    Session s;
    s.type = "Lecture";
    s.moduleCode = module;
    s.lecturer = lecturer;
    s.room = room;
    s.week = week;
    s.day = day;
    s.time = time;
    s.studentGroups = std::move(groups);
    return s;
}

// Redirects std::cin to read from a string for the lifetime of the object.
struct CinFrom {
    std::istringstream input;
    std::streambuf* previous;
    explicit CinFrom(const std::string& text) : input(text), previous(std::cin.rdbuf()) {
        std::cin.rdbuf(input.rdbuf());
        std::cin.clear();
    }
    ~CinFrom() {
        std::cin.rdbuf(previous);
        std::cin.clear();
    }
};

} // namespace

// ---------------------------------------------------------------- Session CSV

TEST(session_toCSV_writes_fields_then_groups) {
    Session s = makeSession("SOFT20091", "Dr. Smith", "MAE202", 1, "Monday", "10:00",
                            {"GroupA", "GroupB"});
    CHECK(s.toCSV() == "Lecture,SOFT20091,Dr. Smith,MAE202,1,Monday,10:00,GroupA,GroupB");
}

TEST(session_fromCSV_parses_all_fields) {
    Session s;
    s.fromCSV("Lab,SOFT20101,Dr. Jones,MAE122,2,Tuesday,14:00,GroupC,GroupD");
    CHECK(s.type == "Lab");
    CHECK(s.moduleCode == "SOFT20101");
    CHECK(s.lecturer == "Dr. Jones");
    CHECK(s.room == "MAE122");
    CHECK(s.week == 2);
    CHECK(s.day == "Tuesday");
    CHECK(s.time == "14:00");
    CHECK(s.studentGroups.size() == 2);
    CHECK(s.studentGroups[1] == "GroupD");
}

TEST(session_csv_round_trip) {
    Session original = makeSession("M1", "L1", "R1", 53, "Friday", "16:00", {"G1"});
    Session copy;
    copy.fromCSV(original.toCSV());
    CHECK(copy.toCSV() == original.toCSV());
}

TEST(session_fromCSV_rejects_non_numeric_week) {
    Session s;
    bool threw = false;
    try {
        s.fromCSV("Lab,M1,L1,R1,notanumber,Monday,09:00,G1");
    }
    catch (const std::invalid_argument&) {
        threw = true;
    }
    CHECK(threw);
}

// ---------------------------------------------------------------- Group timetables

TEST(addSession_indexes_session_under_every_group) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"GroupA", "GroupB"}));
    CHECK(sys.getAllSessions().size() == 1);
    CHECK(sys.getGroupTimetable("GroupA").size() == 1);
    CHECK(sys.getGroupTimetable("GroupB").size() == 1);
}

TEST(getGroupTimetable_unknown_group_is_empty) {
    TimetableSystem sys;
    CHECK(sys.getGroupTimetable("Nobody").empty());
}

TEST(removeSession_updates_group_timetables) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"GroupA"}));
    sys.addSession(makeSession("M2", "L2", "R2", 1, "Monday", "10:00", {"GroupA"}));
    CHECK(sys.removeSession(0));
    CHECK(sys.getAllSessions().size() == 1);
    CHECK(sys.getGroupTimetable("GroupA").size() == 1);
    CHECK(sys.getGroupTimetable("GroupA")[0].moduleCode == "M2");
}

TEST(removeSession_and_editSession_reject_bad_index) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"GroupA"}));
    CHECK(!sys.removeSession(-1));
    CHECK(!sys.removeSession(1));
    CHECK(!sys.editSession(5, Session{}));
    CHECK(sys.getAllSessions().size() == 1);
}

TEST(editSession_moves_session_between_groups) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"GroupA"}));
    CHECK(sys.editSession(0, makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"GroupB"})));
    CHECK(sys.getGroupTimetable("GroupA").empty());
    CHECK(sys.getGroupTimetable("GroupB").size() == 1);
}

// ---------------------------------------------------------------- Clash detection

TEST(conflict_same_room_same_slot) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    auto c = sys.findConflicts(makeSession("M2", "L2", "R1", 1, "Monday", "09:00", {"G2"}));
    CHECK(c.size() == 1);
}

TEST(conflict_same_lecturer_same_slot) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    auto c = sys.findConflicts(makeSession("M2", "L1", "R2", 1, "Monday", "09:00", {"G2"}));
    CHECK(c.size() == 1);
}

TEST(conflict_shared_group_same_slot) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1", "G2"}));
    auto c = sys.findConflicts(makeSession("M2", "L2", "R2", 1, "Monday", "09:00", {"G3", "G2"}));
    CHECK(c.size() == 1);
}

TEST(no_conflict_when_week_day_or_time_differs) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    CHECK(sys.findConflicts(makeSession("M2", "L1", "R1", 2, "Monday", "09:00", {"G1"})).empty());
    CHECK(sys.findConflicts(makeSession("M2", "L1", "R1", 1, "Tuesday", "09:00", {"G1"})).empty());
    CHECK(sys.findConflicts(makeSession("M2", "L1", "R1", 1, "Monday", "10:00", {"G1"})).empty());
}

TEST(no_conflict_when_nothing_shared) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    CHECK(sys.findConflicts(makeSession("M2", "L2", "R2", 1, "Monday", "09:00", {"G2"})).empty());
}

TEST(conflict_reported_once_when_several_resources_clash) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    auto c = sys.findConflicts(makeSession("M2", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    CHECK(c.size() == 1);
}

// Regression: the original "skip self" check skipped any session of the same module at the
// same time, so these clashes were never reported.
TEST(conflict_between_two_sessions_of_the_same_module) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    auto c = sys.findConflicts(makeSession("M1", "L2", "R1", 1, "Monday", "09:00", {"G2"}));
    CHECK(c.size() == 1);
}

TEST(findConflicts_ignoreIndex_skips_only_that_entry) {
    TimetableSystem sys;
    sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"G1"}));
    sys.addSession(makeSession("M2", "L2", "R1", 1, "Monday", "09:00", {"G2"}));
    const auto& all = sys.getAllSessions();
    CHECK(sys.findConflicts(all[0], 0).size() == 1); // clashes with entry 1, not itself
    CHECK(sys.findConflicts(all[0]).size() == 2);    // without ignoreIndex it matches itself too
}

// ---------------------------------------------------------------- Reference data

TEST(addStudentGroup_rejects_empty_and_duplicates) {
    TimetableSystem sys;
    CHECK(sys.addStudentGroup("GroupA"));
    CHECK(!sys.addStudentGroup("GroupA"));
    CHECK(!sys.addStudentGroup(""));
    CHECK(sys.removeStudentGroup("GroupA"));
    CHECK(!sys.removeStudentGroup("GroupA"));
}

TEST(assignStudentToGroup_requires_registered_student_and_existing_group) {
    TimetableSystem sys;
    sys.addStudentGroup("GroupA");
    CHECK(!sys.assignStudentToGroup("s1", "GroupA")); // not registered yet
    CHECK(sys.registerStudent("s1"));
    CHECK(!sys.registerStudent("s1"));
    CHECK(!sys.assignStudentToGroup("s1", "GroupZ")); // no such group
    CHECK(sys.assignStudentToGroup("s1", "GroupA"));
    CHECK(sys.getStudentGroup("s1") == "GroupA");
    CHECK(sys.getStudentGroup("unknown").empty());
}

TEST(assignLecturerToModule_requires_both_to_exist) {
    TimetableSystem sys;
    Module m;
    m.code = "M1";
    sys.addModule(m);
    CHECK(!sys.assignLecturerToModule("L1", "M1")); // lecturer not added
    CHECK(sys.addLecturer("L1"));
    CHECK(!sys.assignLecturerToModule("L1", "M9")); // module not added
    CHECK(sys.assignLecturerToModule("L1", "M1"));
    CHECK(sys.getAllModules()[0].lecturers.size() == 1);
}

TEST(addSessionType_and_addRoom_reject_duplicates) {
    TimetableSystem sys;
    CHECK(sys.addSessionType("Lab"));
    CHECK(!sys.addSessionType("Lab"));
    CHECK(sys.addRoom("MAE122"));
    CHECK(!sys.addRoom("MAE122"));
    CHECK(!sys.addRoom(""));
}

// ---------------------------------------------------------------- Persistence

TEST(save_then_load_restores_sessions_and_student_groups) {
    {
        TimetableSystem sys;
        sys.addSession(makeSession("M1", "L1", "R1", 1, "Monday", "09:00", {"GroupA"}));
        sys.addStudentGroup("GroupA");
        sys.registerStudent("s1");
        sys.assignStudentToGroup("s1", "GroupA");
        sys.saveData();
    }
    TimetableSystem loaded;
    loaded.loadData();
    CHECK(loaded.getAllSessions().size() == 1);
    CHECK(loaded.getGroupTimetable("GroupA").size() == 1);
    CHECK(loaded.getStudentGroup("s1") == "GroupA");
    std::remove("data.dat");
    std::remove("students.dat");
}

TEST(loadData_skips_malformed_lines) {
    {
        std::ofstream out("data.dat");
        out << "Lecture,M1,L1,R1,1,Monday,09:00,G1\n"
            << "\n"
            << "broken,line\n"
            << "Lab,M2,L2,R2,2,Tuesday,10:00,G2\n";
    }
    std::cerr.setstate(std::ios::failbit); // silence the expected load error
    TimetableSystem sys;
    sys.loadData();
    std::cerr.clear();
    CHECK(sys.getAllSessions().size() == 2);
    std::remove("data.dat");
}

// ---------------------------------------------------------------- Menu input

TEST(readMenuChoice_reads_number) {
    CinFrom in("4\n");
    CHECK(readMenuChoice(3) == 4);
}

TEST(readMenuChoice_discards_non_numeric_input) {
    CinFrom in("abc\n2\n");
    CHECK(readMenuChoice(3) == 0);
    CHECK(readMenuChoice(3) == 2);
}

TEST(readMenuChoice_returns_back_option_at_end_of_input) {
    CinFrom in("");
    CHECK(readMenuChoice(7) == 7);
}

int main() {
    int failedTests = 0;
    for (const auto& test : registry()) {
        int before = failures;
        test.body();
        bool passed = failures == before;
        if (!passed) failedTests++;
        std::cout << (passed ? "[PASS] " : "[FAIL] ") << test.name << "\n";
    }
    std::cout << "\n"
              << registry().size() - failedTests << "/" << registry().size() << " tests passed, "
              << failures << " failed checks\n";
    return failedTests == 0 ? 0 : 1;
}
