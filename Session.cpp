#include "Session.h"
#include <sstream>
using namespace std;

string Session::toCSV() const {
    ostringstream oss;
    oss << type << "," << moduleCode << "," << lecturer << ","
        << room << "," << week << "," << day << "," << time;
    for (const auto& group : studentGroups) {
        oss << "," << group;
    }
    return oss.str();
}

void Session::fromCSV(const string& csvLine) {
    istringstream iss(csvLine);
    string token;

    getline(iss, type, ',');
    getline(iss, moduleCode, ',');
    getline(iss, lecturer, ',');
    getline(iss, room, ',');

    getline(iss, token, ',');
    week = stoi(token);

    getline(iss, day, ',');
    getline(iss, time, ',');

    studentGroups.clear();
    while (getline(iss, token, ',')) {
        studentGroups.push_back(token);
    }
}
