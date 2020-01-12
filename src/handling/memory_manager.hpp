#pragma once

#include <unordered_map>
#include <sstream>
#include <string>

class MemoryManager {
    std::unordered_map<long long, long long> memoryMap;
    long long addressCounter = 1000;

public:
    MemoryManager();
    ~MemoryManager();

    long long allocateVar();
    long long allocateArray(long long size);
    std::pair<long long,std::string> generateNumber(long long number);
    std::string generateFirstNumbers(int n);
};