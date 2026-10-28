#!/usr/bin/env sh
# Runs the timetable program against the sample data with scripted menu input.
# The program reads/writes data.dat and students.dat in its working directory, so it runs
# in a temporary copy and the files in examples/ are left untouched.
#
# Usage (from the repository root, after building):  sh examples/run_example.sh
set -e

root="$(cd "$(dirname "$0")/.." && pwd)"
bin="$root/build/timetable"
[ -x "$bin" ] || bin="$root/build/timetable.exe"
[ -x "$bin" ] || { echo "Build first: cmake -B build && cmake --build build" >&2; exit 1; }

work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT
cp "$root/examples/data.dat" "$root/examples/students.dat" "$work/"

cd "$work"
"$bin" < "$root/examples/demo_input.txt"
