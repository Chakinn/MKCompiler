#include "handler.hpp"

#include "symbol.hpp"

#include <iostream>

void Handler::handleVarDeclaration(std::string identifier) {
    try {
        symbolTable.at(identifier);
        logError("Variable " + identifier + " was already declared.\n",1);
    }
    catch (std::out_of_range ex) {
        symbolTable.insert(std::make_pair(identifier, new Symbol(SymbolType::NUMBER)));
        std::cout << "Declared " << identifier << "\n";
    }
}

void Handler::handleArrayDeclaration(std::string identifier, long long startIndex, long long endIndex) {
    try {
        symbolTable.at(identifier);
        logError("Variable " + identifier + " was already declared.\n",1);
    }
    catch (std::out_of_range ex) {
        symbolTable.insert(std::make_pair(identifier, new Symbol(SymbolType::NUMBER)));
        std::cout << "Declared " << identifier << "\n";
    }
}

void Handler::logError(std::string errorMessage, int lineNumber) {
    std::cout << "Error on line: " << lineNumber << ". " << errorMessage << "\n";
}