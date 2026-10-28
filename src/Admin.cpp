#include "Admin.h"
#include "Module.h"
#include "Session.h"
#include "TimetableSystem.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <cctype>
using namespace std;

bool Admin::login() {
    cout << "Admin Username: ";
    if (!(cin >> username)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    cout << "Password: ";
    if (!(cin >> password)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    // Hard-coded demo credentials
    return username == "admin" && password == "admin123";
}

void Admin::showMenu() {
    while (true) {
        cout << "\nMAIN ADMIN MENU\n";
        cout << "1. Manage Data\n"
             << "2. Manage Timetable\n"
             << "3. Logout\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        try {
            switch (choice) {
            case 1: manageDataMenu(); break;
            case 2: manageTimetableMenu(); break;
            case 3: return;
            default: cout << "Invalid choice\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void Admin::manageDataMenu() {
    while (true) {
        cout << "\nDATA MANAGEMENT MENU\n";
        cout << "1. Manage Modules\n"
             << "2. Manage Student Groups\n"
             << "3. Manage Session Types\n"
             << "4. Manage Rooms\n"
             << "5. Manage Lecturers\n"
             << "6. Manage Students\n"
             << "7. Back to Main Menu\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        try {
            switch (choice) {
            case 1: manageModules(); break;
            case 2: manageStudentGroups(); break;
            case 3: manageSessionTypes(); break;
            case 4: manageRooms(); break;
            case 5: manageLecturers(); break;
            case 6: manageStudents(); break;
            case 7: return;
            default: cout << "Invalid choice\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void Admin::manageTimetableMenu() {
    while (true) {
        cout << "\nTIMETABLE MANAGEMENT MENU\n";
        cout << "1. Create New Entry\n"
             << "2. Edit Existing Entry\n"
             << "3. Delete Entry\n"
             << "4. Check for Conflicts\n"
             << "5. View All Entries\n"
             << "6. Back to Main Menu\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        try {
            switch (choice) {
            case 1: createTimetableEntry(); break;
            case 2: editTimetableEntry(); break;
            case 3: deleteTimetableEntry(); break;
            case 4: checkConflicts(); break;
            case 5: displayTimetableEntries(); break;
            case 6: return;
            default: cout << "Invalid choice\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void Admin::manageModules() {
    while (true) {
        cout << "\nMODULE MANAGEMENT\n";
        cout << "1. Add Module\n"
             << "2. List Modules\n"
             << "3. Back\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            Module m;
            cout << "Enter module code: ";
            cin >> m.code;
            cout << "Enter module name: ";
            cin.ignore();
            getline(cin, m.name);

            system->addModule(m);
            cout << "Module added successfully!\n";
        }
        else if (choice == 2) {
            const auto& modules = system->getAllModules();
            cout << "\nCurrent Modules:\n";
            for (const auto& m : modules) {
                cout << m.code << " - " << m.name << "\n";
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}

void Admin::manageStudentGroups() {
    while (true) {
        cout << "\nSTUDENT GROUP MANAGEMENT\n";
        cout << "1. Add Group\n"
             << "2. Remove Group\n"
             << "3. List Groups\n"
             << "4. Back\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string group;
            cout << "Enter group name: ";
            cin >> group;
            if (system->addStudentGroup(group)) {
                cout << "Group added successfully!\n";
            }
            else {
                cout << "Group already exists!\n";
            }
        }
        else if (choice == 2) {
            string group;
            cout << "Enter group name to remove: ";
            cin >> group;
            if (system->removeStudentGroup(group)) {
                cout << "Group removed successfully!\n";
            }
            else {
                cout << "Group not found!\n";
            }
        }
        else if (choice == 3) {
            const auto& groups = system->getAllStudentGroups();
            cout << "\nCurrent Groups:\n";
            for (const auto& group : groups) {
                cout << "- " << group << "\n";
            }
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}

void Admin::manageSessionTypes() {
    while (true) {
        cout << "\nSESSION TYPE MANAGEMENT\n";
        cout << "1. Add Type\n"
             << "2. List Types\n"
             << "3. Back\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string type;
            cout << "Enter session type: ";
            cin >> type;
            if (system->addSessionType(type)) {
                cout << "Session type added successfully!\n";
            }
            else {
                cout << "Session type already exists!\n";
            }
        }
        else if (choice == 2) {
            const auto& types = system->getAllSessionTypes();
            cout << "\nCurrent Session Types:\n";
            for (const auto& type : types) {
                cout << "- " << type << "\n";
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}

void Admin::manageRooms() {
    while (true) {
        cout << "\nROOM MANAGEMENT\n";
        cout << "1. Add Room\n"
             << "2. List Rooms\n"
             << "3. Back\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string room;
            cout << "Enter room name/number: ";
            cin.ignore();
            getline(cin, room);
            if (system->addRoom(room)) {
                cout << "Room added successfully!\n";
            }
            else {
                cout << "Room already exists!\n";
            }
        }
        else if (choice == 2) {
            const auto& rooms = system->getAllRooms();
            cout << "\nCurrent Rooms:\n";
            for (const auto& room : rooms) {
                cout << "- " << room << "\n";
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}

void Admin::manageLecturers() {
    while (true) {
        cout << "\nLECTURER MANAGEMENT\n";
        cout << "1. Add Lecturer\n"
             << "2. Assign to Module\n"
             << "3. List Lecturers\n"
             << "4. Back\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string lecturer;
            cout << "Enter lecturer name: ";
            cin.ignore();
            getline(cin, lecturer);
            if (system->addLecturer(lecturer)) {
                cout << "Lecturer added successfully!\n";
            }
            else {
                cout << "Lecturer already exists!\n";
            }
        }
        else if (choice == 2) {
            string lecturer, moduleCode;
            cout << "Enter lecturer name: ";
            cin.ignore();
            getline(cin, lecturer);
            cout << "Enter module code: ";
            cin >> moduleCode;
            if (system->assignLecturerToModule(lecturer, moduleCode)) {
                cout << "Lecturer assigned successfully!\n";
            }
            else {
                cout << "Failed to assign lecturer!\n";
            }
        }
        else if (choice == 3) {
            const auto& lecturers = system->getAllLecturers();
            cout << "\nCurrent Lecturers:\n";
            for (const auto& lecturer : lecturers) {
                cout << "- " << lecturer << "\n";
            }
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}

void Admin::manageStudents() {
    while (true) {
        cout << "\nSTUDENT MANAGEMENT\n";
        cout << "1. Register Student\n"
             << "2. Assign to Group\n"
             << "3. List Students\n"
             << "4. Back\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string studentId;
            cout << "Enter student ID: ";
            cin >> studentId;
            if (system->registerStudent(studentId)) {
                cout << "Student registered successfully!\n";
            }
            else {
                cout << "Student already exists!\n";
            }
        }
        else if (choice == 2) {
            string studentId, group;
            cout << "Enter student ID: ";
            cin >> studentId;
            cout << "Enter group name: ";
            cin >> group;
            if (system->assignStudentToGroup(studentId, group)) {
                cout << "Student assigned to group successfully!\n";
            }
            else {
                cout << "Failed to assign student!\n";
            }
        }
        else if (choice == 3) {
            const auto& students = system->getAllStudents();
            cout << "\nCurrent Students:\n";
            for (const auto& student : students) {
                cout << "- " << student << "\n";
            }
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }
}

void Admin::createTimetableEntry() {
    cout << "\nCREATE NEW TIMETABLE ENTRY\n";
    Session newSession;

    // Get session type
    const auto& types = system->getAllSessionTypes();
    cout << "Available session types:\n";
    for (const auto& type : types) {
        cout << "- " << type << "\n";
    }
    cout << "Enter session type: ";
    cin >> newSession.type;

    // Get module code
    const auto& modules = system->getAllModules();
    cout << "Available modules:\n";
    for (const auto& module : modules) {
        cout << "- " << module.code << ": " << module.name << "\n";
    }
    cout << "Enter module code: ";
    cin >> newSession.moduleCode;

    // Get lecturer
    const auto& lecturers = system->getAllLecturers();
    cout << "Available lecturers:\n";
    for (const auto& lecturer : lecturers) {
        cout << "- " << lecturer << "\n";
    }
    cout << "Enter lecturer: ";
    cin.ignore();
    getline(cin, newSession.lecturer);

    // Get room
    const auto& rooms = system->getAllRooms();
    cout << "Available rooms:\n";
    for (const auto& room : rooms) {
        cout << "- " << room << "\n";
    }
    cout << "Enter room: ";
    getline(cin, newSession.room);

    // Get week
    cout << "Enter week number (1-53): ";
    cin >> newSession.week;

    // Get day
    cout << "Enter day (e.g., Monday): ";
    cin >> newSession.day;

    // Get time
    cout << "Enter time (e.g., 10:00): ";
    cin >> newSession.time;

    // Get groups
    const auto& groups = system->getAllStudentGroups();
    cout << "Available groups:\n";
    for (const auto& group : groups) {
        cout << "- " << group << "\n";
    }
    cout << "Enter groups (comma separated): ";
    string groupsInput;
    cin.ignore();
    getline(cin, groupsInput);

    // Parse groups
    istringstream iss(groupsInput);
    string group;
    while (getline(iss, group, ',')) {
        group.erase(0, group.find_first_not_of(" \t"));
        group.erase(group.find_last_not_of(" \t") + 1);
        newSession.studentGroups.push_back(group);
    }

    // Check for conflicts
    auto conflicts = system->findConflicts(newSession);
    if (!conflicts.empty()) {
        cout << "\nWARNING: Potential conflicts found:\n";
        for (const auto& conflict : conflicts) {
            cout << "- " << conflict.moduleCode << " at " << conflict.day << " " << conflict.time
                 << " in " << conflict.room << "\n";
        }
        cout << "\nDo you want to add anyway? (y/n): ";
        char choice;
        cin >> choice;
        if (tolower(choice) != 'y') {
            cout << "Entry creation cancelled.\n";
            return;
        }
    }

    system->addSession(newSession);
    cout << "Timetable entry created successfully!\n";
}

void Admin::editTimetableEntry() {
    int id = selectTimetableEntry();
    if (id <= 0) return;

    const auto& sessions = system->getAllSessions();
    const Session& existing = sessions[id - 1];
    Session updated = existing;

    cout << "\nEDITING ENTRY " << id << "\n";
    cout << "Leave field blank to keep current value\n";

    // Get updated session type
    const auto& types = system->getAllSessionTypes();
    cout << "Current type: " << existing.type << "\n";
    cout << "Available types:\n";
    for (const auto& type : types) {
        cout << "- " << type << "\n";
    }
    cout << "Enter new type: ";
    string input;
    getline(cin, input);
    if (!input.empty()) updated.type = input;

    // Similar editing for other fields (module, lecturer, room, etc.)

    // Check for conflicts
    auto conflicts = system->findConflicts(updated);
    if (!conflicts.empty()) {
        cout << "\nWARNING: Potential conflicts found:\n";
        for (const auto& conflict : conflicts) {
            cout << "- " << conflict.moduleCode << " at " << conflict.day << " " << conflict.time
                 << " in " << conflict.room << "\n";
        }
        cout << "\nDo you want to save changes anyway? (y/n): ";
        char choice;
        cin >> choice;
        if (tolower(choice) != 'y') {
            cout << "Edit cancelled.\n";
            return;
        }
    }

    if (system->editSession(id - 1, updated)) {
        cout << "Timetable entry updated successfully!\n";
    }
    else {
        cout << "Failed to update entry.\n";
    }
}

void Admin::deleteTimetableEntry() {
    int id = selectTimetableEntry();
    if (id <= 0) return;

    cout << "Are you sure you want to delete this entry? (y/n): ";
    char choice;
    cin >> choice;
    if (tolower(choice) != 'y') {
        cout << "Deletion cancelled.\n";
        return;
    }

    if (system->removeSession(id - 1)) {
        cout << "Timetable entry deleted successfully!\n";
    }
    else {
        cout << "Failed to delete entry.\n";
    }
}

void Admin::checkConflicts() {
    const auto& sessions = system->getAllSessions();
    bool foundConflicts = false;

    for (size_t i = 0; i < sessions.size(); i++) {
        auto conflicts = system->findConflicts(sessions[i]);
        if (!conflicts.empty()) {
            foundConflicts = true;
            cout << "\nConflicts for entry " << (i + 1) << ":\n";
            cout << "Module: " << sessions[i].moduleCode << ", " << sessions[i].day << " "
                 << sessions[i].time << " in " << sessions[i].room << "\n";

            for (const auto& conflict : conflicts) {
                cout << "- CONFLICT WITH: " << conflict.moduleCode << ", " << conflict.day << " "
                     << conflict.time << " in " << conflict.room << "\n";
            }
        }
    }

    if (!foundConflicts) {
        cout << "\nNo conflicts found in timetable.\n";
    }
}

void Admin::displayTimetableEntries() const {
    const auto& sessions = system->getAllSessions();

    if (sessions.empty()) {
        cout << "\nNo timetable entries found.\n";
        return;
    }

    cout << "\nCURRENT TIMETABLE ENTRIES\n";
    cout << "ID\tModule\t\tType\tDay\tTime\tRoom\tGroups\n";

    for (size_t i = 0; i < sessions.size(); i++) {
        const auto& session = sessions[i];
        cout << (i + 1) << "\t" << session.moduleCode << "\t" << session.type << "\t" << session.day
             << "\t" << session.time << "\t" << session.room << "\t";

        for (size_t j = 0; j < session.studentGroups.size(); j++) {
            if (j > 0) cout << ", ";
            cout << session.studentGroups[j];
        }
        cout << "\n";
    }
}

int Admin::selectTimetableEntry() const {
    displayTimetableEntries();

    const auto& sessions = system->getAllSessions();
    if (sessions.empty()) {
        return -1;
    }

    cout << "Enter entry ID to select (0 to cancel): ";
    int choice;
    cin >> choice;

    if (cin.fail() || choice < 0 || choice > static_cast<int>(sessions.size())) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid selection.\n";
        return -1;
    }

    return choice;
}