#include "memory_manager.hpp"

MemoryManager::MemoryManager(){}

MemoryManager::~MemoryManager(){}

long long MemoryManager::allocateVar() {
    addressCounter++;
    return addressCounter;
}

long long MemoryManager::allocateArray(long long size) {
    addressCounter += size;
    return addressCounter;
}

std::pair<long long,std::string> MemoryManager::generateNumber(long long number) {
    std::stringstream code;

    // if it was generated earlier, return it
    try {
        long long add = memoryMap.at(number);
        return std::pair<long long, std::string>(add,"");
    }
    catch(const std::out_of_range) {

    }
    
    long long currentNumber = 1;
    long long ONE;
    try {
        ONE = memoryMap.at(1);
    }
    catch(const std::out_of_range) {

    }
    
    //code << "SUB 0";
    //code << "INC";
    //code << "STORE 101";
    while (currentNumber*2 < number) 
    {
        currentNumber = currentNumber << 1;
        code << "SHIFT " << ONE << "\n";
    }
    while(currentNumber < number) {
        currentNumber++;
        code << "INC" << "\n";
    }

    addressCounter++;
    code << "STORE " << addressCounter << "\n";
    

    return std::pair<long long, std::string>(addressCounter, code.str());
}

std::string MemoryManager::generateFirstNumbers(int n) {
    std::stringstream code;

    code << "SUB 0" << "\n";

    for(int i = 1; i <= n; i++) {
        addressCounter++;
        code << "INC" << "\n";
        code << "STORE " << addressCounter << "\n";
        memoryMap.insert(std::pair<long long, long long>(i,addressCounter));
    }

    return code.str();
    
}

/*
long long MemoryManager::nextAddress() {
    addressCounter++;
    return addressCounter;
}*/