#pragma once

#include <string>
#include <unordered_map>

#include "symbol.hpp"

class Handler {
    std::unordered_map<std::string, Symbol*> symbolTable;

public:
    void handleVarDeclaration(std::string identifier);
    void handleArrayDeclaration(std::string identifier, long long startIndex, long long endIndex);
    void handleAssign(std::string identifier, std::string value1, std::string value2);
    void handleIf();
    void handleWhile();
    void handleFor(std::string identifier);
    void handleRead(std::string identifier);
    void handleWrite(std::string value);
    void handleExpression(std::string value1, std::string op, std::string value2);
    void handleCondition(std::string value1, std::string op, std::string value2);
private:
    void logError(std::string errorMessage, int lineNumber);
};