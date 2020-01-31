#pragma once

#include <unordered_map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "symbol.hpp"
#include "memory_manager.hpp"
#include "log.hpp"

class SymbolTable {
    std::unordered_map<std::string, std::unique_ptr<Symbol>> table;
    MemoryManager* memoryManager;

public:
    SymbolTable(){}
    SymbolTable(MemoryManager* memManager);
    ~SymbolTable(){};

    bool isDeclared(std::string const& symbolIdentifier);
    void declare(std::string identifier, Symbol* symbol);
    void undeclare(std::string identifier);
    bool isNumber(std::string value);

    Symbol* getSymbol(std::string const& symbolIdentifier);

    long long getAddress(std::string const& symbolIdentifier);
    long long getFreeAddress();
    std::vector<std::string> calculateAddress(std::string identifier);


    long long generateNumber(long long number);
    

};