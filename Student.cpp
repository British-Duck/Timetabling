#include "Student.h"
#include "TimetableSystem.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

bool Student::login() {
    studentID.clear();
    group.clear();

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Student ID: ";
    if (!(cin >> studentID)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    group = system->getStudentGroup(studentID);
    if (group.empty()) {
        cout << "You are not assigned to any group. Contact admin.\n";
        return false;
    }

    return true;
}

void Student::showMenu() {
    while (true) {
        cout << "\nSTUDENT MENU\n";
        cout << "1. View Timetable\n2. Export to CSV\n3. Logout\nChoice: ";

        int choice;
        cin >> choice;

        try {
            switch (choice) {
            case 1: viewTimetable(); break;
            case 2: exportToCSV(); break;
            case 3: return;
            default: cout << "Invalid choice\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

void Student::viewTimetable() {
    const auto& timetable = system->getGroupTimetable(group);

    if (timetable.empty()) {
        cout << "No timetable entries found for your group.\n";
        return;
    }

    cout << "\nYour Timetable (" << group << "):\n";
    cout << "Week\tDay\t\tTime\tModule\t\tType\tRoom\tLecturer\n";
    for (const auto& session : timetable) {
        cout << session.week << "\t"
            << session.day << "\t"
            << session.time << "\t"
            << session.moduleCode << "\t"
            << session.type << "\t"
            << session.room << "\t"
            << session.lecturer << "\n";
    }
}

void Student::exportToCSV() {
    const auto& timetable = system->getGroupTimetable(group);
    if (timetable.empty()) {
        cout << "No timetable entries to export.\n";
        return;
    }

    string filename = "timetable_" + studentID + ".csv";
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error creating file: " << filename << "\n";
        return;
    }

    file << "Week,Day,Time,Module,Type,Room,Lecturer\n";
    for (const auto& session : timetable) {
        file << session.week << ","
            << session.day << ","
            << session.time << ","
            << session.moduleCode << ","
            << session.type << ","
            << session.room << ","
            << session.lecturer << "\n";
    }

    file.close();
    cout << "Timetable exported to " << filename << "\n";
}