#pragma once
#include <vector>
#include <map>
#include <string>
#include <set>
#include "Session.h"
#include "Module.h"
#include <unordered_map>

class TimetableSystem {
private:
    //-----------------Data storage ------------------//
    std::vector<Module> modules;
    std::vector<Session> sessions;
    std::map<std::string, std::vector<Session>> groupTimetables;
    std::set<std::string> sessionTypes;     // tracks allowed session types
    std::vector<std::string> studentGroups; // All student groups
    std::vector<std::string> rooms;         // All available rooms
    std::vector<std::string> lecturers;     // All lecturers
    std::vector<std::string> students;      // All registered students
    std::unordered_map<std::string, std::string> studentGroupMap;

public:
    //-------------Core system operations--------------------//
    void run();
    void loadData();
    void saveData();

    //-----------------Session management------------------------//
    const std::vector<Session>& getGroupTimetable(const std::string& group) const;
    void addSession(const Session& session);
    bool removeSession(int index);
    bool editSession(int index, const Session& newSession);
    // ignoreIndex: position of `session` in getAllSessions() when it is already stored,
    // so it is not reported as clashing with itself. Use -1 for a new session.
    std::vector<Session> findConflicts(const Session& session, int ignoreIndex = -1) const;

    //-------------------Module management----------------//
    void addModule(const Module& module);

    //-----------------Data management--------------------//
    bool addStudentGroup(const std::string& group);
    bool removeStudentGroup(const std::string& group);
    bool addSessionType(const std::string& type);
    bool addRoom(const std::string& room);
    bool addLecturer(const std::string& lecturer);
    bool registerStudent(const std::string& studentId);
    bool assignStudentToGroup(const std::string& studentId, const std::string& group);
    bool assignLecturerToModule(const std::string& lecturer, const std::string& moduleCode);

    //--------------------Data accessors-------------------------//
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
