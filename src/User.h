#pragma once
#include <string>

// Forward declaration to avoid circular dependencies
class TimetableSystem;
//-------------Base Class for all User Types--------------//
class User {
protected:
    std::string username; // Stores User login info
    std::string password;
    TimetableSystem* system; // Pointer to main system for accessing data

public:
    //---------- Constructor takes pointer to main system----///
    User(TimetableSystem* sys) : system(sys) {}

    //----------- Pure virtual functions ------------------//
    virtual bool login() = 0;    // Handles user authentication
    virtual void showMenu() = 0; // Displays user-specific menu

    // Virtual destructor for proper cleanup of derived classes//
    virtual ~User() = default;
};
