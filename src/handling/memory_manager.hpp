#pragma once

#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

class MemoryManager {
    std::map<long long, long long> memoryMap;
    long long addressCounter = 1;
    std::vector<std::string> code;

public:
    MemoryManager();
    ~MemoryManager();

    long long allocateVar();
    long long allocateArray(long long low, long long high);
    long long generateNumber(long long number);
    void generateNumbers();
    std::vector<std::string> getCode();
private:
    class Number {
        long long value;

        public:
        Number(long long val){value=val;}
        long long getValue(){return value;}
        bool operator< (const Number &num) const {return llabs(value) < llabs(num.value);}
    };
    void generateShiftNumbers(std::vector<Number>& numbers);

private:
    
};