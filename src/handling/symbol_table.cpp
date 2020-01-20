#include "symbol_table.hpp"

SymbolTable::SymbolTable(MemoryManager* memManager){
    memoryManager = memManager;
}

bool SymbolTable::isDeclared(std::string const& symbolIdentifier) {
    try {
        table.at(symbolIdentifier);
        return true;
    }
    catch (std::out_of_range ex) {
        return false;
    }
}

void SymbolTable::declare(std::string identifier, Symbol* symbol) {
    table.insert(std::make_pair(identifier,std::unique_ptr<Symbol>(symbol)));
}

long long SymbolTable::getAddress(std::string const& symbolIdentifier) {
    long long address;
    try {
        long long n = std::stoll(symbolIdentifier);
        address = generateNumber(n);
    }
    // else get address fromm symbol table
    catch(const std::invalid_argument) {
        int bracketIndex = symbolIdentifier.find_first_of('(');
        if (bracketIndex == -1) {
            address = table.at(symbolIdentifier)->getAddress();
        }
        else {
            const std::string id = symbolIdentifier.substr(0,bracketIndex);
            std::string num = symbolIdentifier.substr(bracketIndex).substr(1);
            num = num.substr(0,num.length()-1);
            long long index;
            try {
                index = std::stoll(num);
            }
            catch(const std::invalid_argument) {
                return -1;
            }
            
            auto symbol = *table.at(id);
            if(index > symbol.getHigh()) {
                //Handler::logError("Indeks" + num + "poza granicami tablicy " + id + 
                // "("+std::to_string(symbol.getLow()) + "," +std::to_string(symbol.getHigh()) + ")",1);
            }
            address = index + symbol.getAddress() - symbol.getLow();
        }  
    }
    return address;
}

std::vector<std::string> SymbolTable::calculateAddress(std::string identifier) {
    int bracketIndex = identifier.find_first_of('(');
    const std::string id = identifier.substr(0,bracketIndex);
    std::string num = identifier.substr(bracketIndex).substr(1);
    num = num.substr(0,num.length()-1);

    long long arrayAddress = table.at(id)->getAddress();
    long long arrayPointer = generateNumber(arrayAddress);
    long long arrayLow = generateNumber(table.at(id)->getLow());
    long long arrayHigh = generateNumber(table.at(id)->getHigh());

    std::vector<std::string> code;
    code.push_back("LOAD "+std::to_string(arrayPointer));
    code.push_back("ADD "+std::to_string(getAddress(num)));
    code.push_back("SUB "+std::to_string(arrayLow));
    return code;
}

long long SymbolTable::getFreeAddress() {
    return memoryManager->allocateVar();
}

bool SymbolTable::isNumber(std::string value) {
    try {
        std::stoll(value);
        return true;
    }
    catch(const std::invalid_argument) {
        return false;
    }
}

long long SymbolTable::generateNumber(long long number) {
    return memoryManager->generateNumber(number);
}


