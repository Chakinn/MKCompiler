#pragma once

#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

class MemoryManager {
    std::unordered_map<long long, long long> memoryMap;
    long long addressCounter = 1000;
    std::vector<std::string> code;

public:
    MemoryManager();
    ~MemoryManager();

    long long allocateVar();
    long long allocateArray(long long size);
    long long generateNumber(long long number);
    void generateFirstNumbers(int n);
    std::vector<std::string> getCode();
};