#pragma once
#include <string>
#include <vector>

class Session {
public:
    std::string type;       // Session type(e.g. Lecture/Lab/Tutorial)
    std::string moduleCode; // Module this session belongs too
    std::string lecturer;   // Lecturer for session
    std::string room;       // Room for the session
    int week;               // week number(1-53)

    // Time/day of the session
    std::string day;
    std::string time;

    // Groups attending the session
    std::vector<std::string> studentGroups;

    // converts data to CSV format for storage
    std::string toCSV() const;

    // Populates session data from CSV string
    void fromCSV(const std::string& csvLine);
};
