cmake_minimum_required(VERSION 3.16)
project(Timetabling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Everything except main() lives in a library so the tests can link against it.
add_library(timetable_core
    src/Admin.cpp
    src/Session.cpp
    src/Student.cpp
    src/TimetableSystem.cpp
)
target_include_directories(timetable_core PUBLIC src)

add_executable(timetable src/Main.cpp)
target_link_libraries(timetable PRIVATE timetable_core)

# ---- Tests: `ctest --test-dir build` (or run build/timetable_tests directly)
enable_testing()
add_executable(timetable_tests tests/test_main.cpp)
target_link_libraries(timetable_tests PRIVATE timetable_core)
add_test(NAME timetable_tests COMMAND timetable_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
