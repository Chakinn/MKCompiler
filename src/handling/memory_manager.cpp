#include "memory_manager.hpp"


MemoryManager::MemoryManager(){
    generateFirstNumbers(5);
}

MemoryManager::~MemoryManager(){}

long long MemoryManager::allocateVar() {
    addressCounter++;
    return addressCounter;
}

long long MemoryManager::allocateArray(long long size) {
    addressCounter += size;
    return addressCounter;
}

long long MemoryManager::generateNumber(long long number) {

    // if it was generated earlier, return it
    try {
        long long add = memoryMap.at(number);
        return add;
    }
    catch(const std::out_of_range) {}
    
    long long currentNumber = 1;
    long long ONE;
    try {
        ONE = memoryMap.at(1);
    }
    catch(const std::out_of_range) {
        std::cout<<"cannnot find ONE";
    }
    
    code.push_back( "SUB 0");
    code.push_back( "INC");
    code.push_back( "STORE 101");
    while (currentNumber*2 < number) 
    {
        currentNumber = currentNumber + 1;
        code.push_back( "SHIFT " + std::to_string(ONE));
    }
    while(currentNumber < number) {
        currentNumber++;
        code.push_back( "INC");
    }

    addressCounter++;
    code.push_back( "STORE " + std::to_string(addressCounter));
    
    return addressCounter;
}

void MemoryManager::generateFirstNumbers(int n) {
    code.push_back( "SUB 0");

    for(int i = 1; i <= n; i++) {
        addressCounter++;
        code.push_back( "INC");
        code.push_back( "STORE " + std::to_string(addressCounter));
        memoryMap.insert(std::pair<long long, long long>(i,addressCounter));
    }

    long long ONE = memoryMap.at(1);


    code.push_back( "LOAD " + std::to_string(ONE));
    for(int i =1; i<=n; i++) {
        addressCounter++;
        code.push_back( "SHIFT " + std::to_string(ONE));
        code.push_back( "STORE " + std::to_string(addressCounter));
    }
}

std::vector<std::string> MemoryManager::getCode() {
    return code;
}

/*
long long MemoryManager::nextAddress() {
    addressCounter++;
    return addressCounter;
}*/