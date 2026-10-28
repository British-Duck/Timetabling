#include "TimetableSystem.h"
#include "Admin.h"
#include "Student.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;


void TimetableSystem::run() {
    while (true) {
        cout << "\nNTU Timetabling System\n";
        cout << "1. Admin Login\n2. Student Login\n3. Exit\nChoice: ";

        int choice;
        cin >> choice;

        try {
            if (choice == 1) {
                Admin admin(this);
                if (admin.login()) {
                    admin.showMenu();
                }
            }
            else if (choice == 2) {
                Student student(this);
                if (student.login()) {
                    student.showMenu();
                }
            }
            else if (choice == 3) {
                break;
            }
            else {
                cout << "Invalid choice. Please try again.\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

const vector<Session>& TimetableSystem::getGroupTimetable(const string& group) const {
    static const vector<Session> emptyVector;
    auto it = groupTimetables.find(group);
    return (it != groupTimetables.end()) ? it->second : emptyVector;
}

void TimetableSystem::addSession(const Session& session) {
    sessions.push_back(session);
    for (const auto& group : session.studentGroups) {
        groupTimetables[group].push_back(session);
    }
}

void TimetableSystem::addModule(const Module& module) {
    modules.push_back(module);
}

void TimetableSystem::loadData() {
    
    try {
        ifstream file("data.dat");
        if (file) {
            string line;
            while (getline(file, line)) {
                Session s;
                s.fromCSV(line);
                addSession(s);
            }
        }
    }
    catch (const exception& e) {
        cerr << "Load error: " << e.what() << endl;
    }

    ifstream sgfile("students.dat");
    if (sgfile) {
        string line;
        while (getline(sgfile, line)) {
            size_t comma = line.find(',');
            if (comma != string::npos) {
                string studentId = line.substr(0, comma);
                string group = line.substr(comma + 1);
                studentGroupMap[studentId] = group;
            }
        }
        sgfile.close();
    }
}

void TimetableSystem::saveData() {
    try {
        ofstream file("data.dat");
        for (const auto& session : sessions) {
            file << session.toCSV() << "\n";
        }
    }
    catch (const exception& e) {
        cerr << "Save error: " << e.what() << endl;
    }
    ofstream sgfile("students.dat");
    for (const auto& pair : studentGroupMap) {
        sgfile << pair.first << "," << pair.second << "\n";
    }
    sgfile.close();
}

void TimetableSystem::createSampleData() {
    // Sample module
    Module m;
    m.code = "SOFT20091";
    m.name = "Software Design";
    addModule(m);

    // Sample session
    Session s;
    s.type = "Lecture";
    s.moduleCode = "SOFT20091";
    s.lecturer = "Dr. Smith";
    s.room = "MAE202";
    s.week = 1;
    s.day = "Monday";
    s.time = "10:00";
    s.studentGroups.push_back("GroupA");
    addSession(s);
}
// Add student group
bool TimetableSystem::addStudentGroup(const string& group) {
    if (group.empty()) return false;
    if (find(studentGroups.begin(), studentGroups.end(), group) != studentGroups.end()) {
        return false; // Group already exists
    }
    studentGroups.push_back(group);
    return true;
}

// Remove student group
bool TimetableSystem::removeStudentGroup(const string& group) {
    auto it = find(studentGroups.begin(), studentGroups.end(), group);
    if (it == studentGroups.end()) return false;
    studentGroups.erase(it);
    return true;
}

// Add session type
bool TimetableSystem::addSessionType(const string& type) {
    if (type.empty()) return false;
    return sessionTypes.insert(type).second;
}

// Add room
bool TimetableSystem::addRoom(const string& room) {
    if (room.empty()) return false;
    if (find(rooms.begin(), rooms.end(), room) != rooms.end()) {
        return false; // Room already exists
    }
    rooms.push_back(room);
    return true;
}

// Add lecturer
bool TimetableSystem::addLecturer(const string& lecturer) {
    if (lecturer.empty()) return false;
    if (find(lecturers.begin(), lecturers.end(), lecturer) != lecturers.end()) {
        return false; // Lecturer already exists
    }
    lecturers.push_back(lecturer);
    return true;
}

// Register student
bool TimetableSystem::registerStudent(const string& studentId) {
    if (studentId.empty()) return false;
    if (find(students.begin(), students.end(), studentId) != students.end()) {
        return false; // Student already exists
    }
    students.push_back(studentId);
    return true;
}

// Assign student to group
bool TimetableSystem::assignStudentToGroup(const string& studentId, const string& group) {
    if (find(students.begin(), students.end(), studentId) == students.end()) {
        return false; // Student doesn't exist
    }
    if (find(studentGroups.begin(), studentGroups.end(), group) == studentGroups.end()) {
        return false; // Group doesn't exist
    }

    // Store the group assignment
    studentGroupMap[studentId] = group;
    return true;
}

// Assign lecturer to module
bool TimetableSystem::assignLecturerToModule(const string& lecturer, const string& moduleCode) {
    auto it = find_if(modules.begin(), modules.end(),
        [&moduleCode](const Module& m) { return m.code == moduleCode; });

    if (it == modules.end()) return false; // Module doesn't exist
    if (find(lecturers.begin(), lecturers.end(), lecturer) == lecturers.end()) {
        return false; // Lecturer doesn't exist
    }

    it->lecturers.push_back(lecturer);
    return true;
}

// Getters


const vector<string>& TimetableSystem::getAllStudentGroups() const {
    return studentGroups;
}

const set<string>& TimetableSystem::getAllSessionTypes() const {
    return sessionTypes;
}

const vector<string>& TimetableSystem::getAllRooms() const {
    return rooms;
}

const vector<string>& TimetableSystem::getAllLecturers() const {
    return lecturers;
}

const vector<string>& TimetableSystem::getAllStudents() const {
    return students;
}

const vector<Module>& TimetableSystem::getAllModules() const {
    return modules;
}

std::string TimetableSystem::getStudentGroup(const std::string& studentId) const {
    auto it = studentGroupMap.find(studentId);
    return (it != studentGroupMap.end()) ? it->second : "";
}



bool TimetableSystem::removeSession(int index) {
    if (index < 0 || index >= sessions.size()) return false;

    // Remove from main sessions list
    sessions.erase(sessions.begin() + index);

    // Rebuild group timetables
    groupTimetables.clear();
    for (const auto& session : sessions) {
        for (const auto& group : session.studentGroups) {
            groupTimetables[group].push_back(session);
        }
    }
    return true;
}

bool TimetableSystem::editSession(int index, const Session& newSession) {
    if (index < 0 || index >= sessions.size()) return false;

    sessions[index] = newSession;

    // Rebuild group timetables
    groupTimetables.clear();
    for (const auto& session : sessions) {
        for (const auto& group : session.studentGroups) {
            groupTimetables[group].push_back(session);
        }
    }
    return true;
}

vector<Session> TimetableSystem::findConflicts(const Session& session) const {
    vector<Session> conflicts;

    for (const auto& existing : sessions) {
        // Skip self-comparison for existing sessions
        if (existing.moduleCode == session.moduleCode &&
            existing.week == session.week &&
            existing.day == session.day &&
            existing.time == session.time) {
            continue;
        }

        // Check room conflict
        if (existing.room == session.room &&
            existing.week == session.week &&
            existing.day == session.day &&
            existing.time == session.time) {
            conflicts.push_back(existing);
            continue;
        }

        // Check lecturer conflict
        if (existing.lecturer == session.lecturer &&
            existing.week == session.week &&
            existing.day == session.day &&
            existing.time == session.time) {
            conflicts.push_back(existing);
            continue;
        }

        // Check group conflicts
        for (const auto& group : session.studentGroups) {
            if (find(existing.studentGroups.begin(),
                existing.studentGroups.end(), group) != existing.studentGroups.end() &&
                existing.week == session.week &&
                existing.day == session.day &&
                existing.time == session.time) {
                conflicts.push_back(existing);
                break;
            }
        }
    }

    return conflicts;
}

const vector<Session>& TimetableSystem::getAllSessions() const {
    return sessions;
}