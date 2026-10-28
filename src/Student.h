#pragma once
#include "User.h"

//Derived class representing student users with limited access
class Student : public User {
    std::string studentID;   //unique student identifier    
    std::string group;      //Student assigned groups

public:

    //----Constructor passes sysetm refrences to base user class----//
    Student(TimetableSystem* sys) : User(sys) {}


    //--------------Implementaion of base class fucntions--------//
    bool login() override;
    void showMenu() override;

    //----------Student specific functions------------//
    void viewTimetable();
    void exportToCSV();
};

