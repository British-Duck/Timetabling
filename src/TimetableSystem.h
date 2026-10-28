#pragma once
#include <vector>
#include <map>
#include <string>
#include <set>
#include "Session.h"
#include "Module.h"
#include <unordered_map>

class User; // Forward declaration

class TimetableSystem {
private:
    //-----------------Data storage ------------------//
    std::vector<Module> modules;
    std::vector<Session> sessions;
    std::map<std::string, std::vector<Session>> groupTimetables;
    std::set<std::string> sessionTypes;     // tracks allowed session types
    std::vector<std::string> studentGroups; // All student groups
    std::vector<std::string> rooms;         // All avalible rooms
    std::vector<std::string> lecturers;     // All lecturers
    std::vector<std::string> students;      // All registered students
    std::unordered_map<std::string, std::string> studentGroupMap;

public:
    //-------------Core system operations--------------------//
    void run();
    void loadData();
    void saveData();

    //-----------------Session managment------------------------//
    const std::vector<Session>& getGroupTimetable(const std::string& group) const;
    void addSession(const Session& session);
    bool removeSession(int index);
    bool editSession(int index, const Session& newSession);
    std::vector<Session> findConflicts(const Session& session) const;

    //-------------------Module managment----------------//
    void addModule(const Module& module);

    //-----------------Data Managment--------------------//
    bool addStudentGroup(const std::string& group);
    bool removeStudentGroup(const std::string& group);
    bool addSessionType(const std::string& type);
    bool addRoom(const std::string& room);
    bool addLecturer(const std::string& lecturer);
    bool registerStudent(const std::string& studentId);
    bool assignStudentToGroup(const std::string& studentId, const std::string& group);
    bool assignLecturerToModule(const std::string& lecturer, const std::string& moduleCode);

    //--------------------Data Accesors-------------------------//
    const std::vector<Session>& getAllSessions() const;
    const std::vector<std::string>& getAllStudentGroups() const;
    const std::set<std::string>& getAllSessionTypes() const;
    const std::vector<std::string>& getAllRooms() const;
    const std::vector<std::string>& getAllLecturers() const;
    const std::vector<std::string>& getAllStudents() const;
    const std::vector<Module>& getAllModules() const;

    std::string getStudentGroup(const std::string& studentId) const;

    // For testing/demo purposes
    void createSampleData();
};
