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
    return table.at(symbolIdentifier)->getAddress();
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


