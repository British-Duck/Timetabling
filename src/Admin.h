#pragma once
#include "User.h"
#include "Session.h"
#include "TimetableSystem.h"

// Replace existing class with this enhanced version
class Admin : public User {
public:
    Admin(TimetableSystem* sys) : User(sys) {}

    bool login() override;
    void showMenu() override;

private:
    // -------------------Main menu functions----------------//
    void manageDataMenu(); // submenu options
    void manageTimetableMenu();

    //--------------- Data management functions------------//
    void manageModules();
    void manageStudentGroups();
    void manageSessionTypes();
    void manageRooms();
    void manageLecturers();
    void manageStudents();

    // ------------------Timetable management functions-------//
    void createTimetableEntry();
    void editTimetableEntry();
    void deleteTimetableEntry();
    void checkConflicts();

    //-------------------- Helper functions---------------------//
    void displayTimetableEntries() const; // shows all entries
    int selectTimetableEntry() const;     // selcets entry by ID
    Session getSessionInput(bool forEdit = false, const Session* existing = nullptr) const;
    void displayConflicts(const std::vector<Session>& conflicts) const;
};
