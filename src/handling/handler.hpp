#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <utility>
#include <iostream>

#include "symbol.hpp"
#include "symbol_table.hpp"
#include "memory_manager.hpp"
#include "expression.hpp"

class Handler {
    std::unordered_map<std::string, Node*> nodeTable;
    SymbolTable* symbolTable;
    MemoryManager* memoryManager;
    int nodeCounter = 0;

public:
    Handler();
    ~Handler();
    std::string handleVarDeclaration(std::string identifier);
    std::string handleArrayDeclaration(std::string identifier, std::string startIndex, std::string endIndex);
    std::string handleAssign(std::string identifier, std::string expressonIdentifier);
    std::string handleIf();
    std::string handleWhile();
    std::string handleFor(std::string identifier);
    std::string handleRead(std::string identifier);
    std::string handleWrite(std::string value);
    std::string handleExpression(std::string value1, std::string op, std::string value2);
    std::string handleCondition(std::string value1, std::string op, std::string value2);
private:
    void logError(std::string const& errorMessage, int lineNumber);
    long long nextAddress();
    std::string nodeIdentifier();
};