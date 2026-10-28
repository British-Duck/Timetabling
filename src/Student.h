#pragma once
#include "User.h"

// Derived class representing student users with limited access
class Student : public User {
    std::string studentID; // unique student identifier
    std::string group;     // Student's assigned group

public:
    //----Constructor passes system reference to base user class----//
    Student(TimetableSystem* sys) : User(sys) {}

    //--------------Implementation of base class functions--------//
    bool login() override;
    void showMenu() override;

    //----------Student specific functions------------//
    void viewTimetable();
    void exportToCSV();
};
