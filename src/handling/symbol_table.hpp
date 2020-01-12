#pragma once

#include <unordered_map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "symbol.hpp"
#include "memory_manager.hpp"

class SymbolTable {
    std::unordered_map<std::string, std::unique_ptr<Symbol>> table;
    MemoryManager* memoryManager;

public:
    SymbolTable(){}
    SymbolTable(MemoryManager* memManager);
    ~SymbolTable(){};

    bool isDeclared(std::string const& symbolIdentifier);
    void declare(std::string identifier, Symbol* symbol);

    long long getAddress(std::string const& symbolIdentifier);
    bool isNumber(std::string value);

    std::pair<long long,std::string> generateNumber(long long number);
    

};