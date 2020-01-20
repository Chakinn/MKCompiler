#include "memory_manager.hpp"


MemoryManager::MemoryManager(){
    generateNumber(0);
    generateNumber(-1);
    generateNumber(1);   
}

MemoryManager::~MemoryManager(){}

long long MemoryManager::allocateVar() {
    addressCounter++;
    return addressCounter;
}

long long MemoryManager::allocateArray(long long low, long long high) {
    long long address = addressCounter+1;
    addressCounter += high - low + 1;
    generateNumber(low);
    generateNumber(high);
    return address;
}

long long MemoryManager::generateNumber(long long number) {
    // if it was generated earlier, return it
    try {
        long long add = memoryMap.at(number);
        return add;
    }
    catch(const std::out_of_range) {
        addressCounter++;
        memoryMap.insert(std::pair<long long, long long>(number,addressCounter));
    }
    
    return addressCounter;
}
void MemoryManager::generateNumbers() {
    std::vector<Number> numbers;
    std::vector<long long> negativeNumberAddresses;

    long long tempAddress = allocateVar();

    for(auto it = memoryMap.begin(); it != memoryMap.end(); it++) {
        numbers.push_back(Number(it->first));
    }

    std::sort(numbers.begin(),numbers.end());

    //generateShiftNumbers(numbers);

    //generate 1 and -1
    code.push_back("SUB 0");
    code.push_back("STORE "+std::to_string(memoryMap.at(0)));
    code.push_back("DEC");
    code.push_back("STORE "+std::to_string(memoryMap.at(-1)));
    code.push_back("INC");
    code.push_back("INC");
    code.push_back("STORE "+std::to_string(memoryMap.at(1)));

    int n = numbers.size();
    for(int i = 0; i < n; i++) {
        Number number = numbers[i]; 
        long long value = number.getValue();
        long long absValue = abs(value);
        if(absValue == 1 || absValue == 0) {
            continue;
        }
        
        long currentValue =  abs(numbers[i-1].getValue());

        int shift = 0;
        
        //shift to the closest power of two
        while (currentValue*2 < absValue) 
        {
            currentValue = currentValue << 1;
            //shift++;
            code.push_back( "SHIFT " + std::to_string(memoryMap.at(1)));
        }
        /*if(shift > 0) {
            code.push_back( "SHIFT " + std::to_string(memoryMap.at(shift)));
        }*/
        //if you're close go with INC
        if(absValue - currentValue < 10) {
            while(currentValue < absValue) {
                currentValue++;
                code.push_back( "INC");
            }
        }
        //else try shifts
        else {
            shift = 1;
            while(absValue - currentValue > 10) {
                if(currentValue + (currentValue >> shift) < absValue) {
                    code.push_back("STORE " + std::to_string(tempAddress));
                    for(int j = shift; j > 0; j--) {
                        code.push_back("SHIFT " + std::to_string(memoryMap.at(-1)));
                    }
                    //code.push_back("SHIFT " + std::to_string(memoryMap.at(-shift)));
                    code.push_back("ADD " + std::to_string(tempAddress));
                    currentValue += currentValue >> shift;
                    shift = 1;
                }
                else {
                    shift++;
                }
            }
            while(currentValue < absValue) {
                currentValue++;
                code.push_back( "INC");
            }     
        }
        
        long long address = memoryMap.at(value);
        if(number.getValue() < 0) {
            negativeNumberAddresses.push_back(address);
        }
        code.push_back("STORE "+std::to_string(address));
    }

    // switch signs for negative numbers
    for(long long negNumAddress : negativeNumberAddresses) {
        code.push_back("LOAD " + std::to_string(negNumAddress));
        code.push_back("STORE "+std::to_string(tempAddress));
        code.push_back("SUB 0");
        code.push_back("SUB " + std::to_string(tempAddress));
        code.push_back("STORE "+std::to_string(negNumAddress));
        code.push_back("LOAD "+std::to_string(tempAddress));
    }
}

//iterates over needed numbers and generaters numbers used in shifts
void MemoryManager::generateShiftNumbers(std::vector<Number>& numbers) {
    //TODO RECURSION PROBLEM
    int n = numbers.size();
    for(int i = 0; i < n; i++) {
        Number number = numbers[i]; 
        long long value = number.getValue();
        long long absValue = abs(value);
        if(absValue == 1 || absValue == 0) {
            continue;
        }
        
        long currentValue =  abs(numbers[i-1].getValue());
        
        int shift = 0;
        //shift to the closest power of two
        while (currentValue*2 < absValue) 
        {
            currentValue = currentValue << 1;
            shift++;
        }

        generateNumber(shift);
        
        shift = 1;
        while(absValue - currentValue > 10) {
            if(currentValue + (currentValue >> shift) < absValue) {
                generateNumber(-shift);
                currentValue += currentValue >> shift;
                shift = 1;
            }
            else {
                shift++;
            }
        }      
    }
}



std::vector<std::string> MemoryManager::getCode() {
    generateNumbers();
    return code;
}
