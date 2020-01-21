#pragma once

#include <string>
#include <vector>
#include <fstream>

class Writer {
    static std::string output;
    public:
    
    static void setOutput(std::string out);
    static void writeToFile(std::vector<std::string>* code);
};