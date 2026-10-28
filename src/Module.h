#pragma once
#include <string>
#include <vector>

class Module {
public:
    std::string code;  //Module code    
    std::string name;  //Full module name
    std::vector<std::string> lecturers;  //list of lectures teaching this module
};
