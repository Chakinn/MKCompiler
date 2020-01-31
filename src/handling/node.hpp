#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "symbol_table.hpp"

class Node {
protected:
    SymbolTable* symbolTable;
public:
    Node(SymbolTable* symbolTable);
    ~Node(){};
    virtual std::vector<std::string> getCode(){std::vector<std::string> a; return a;};
    void validateIdentifier(std::string identifier, bool needInitialized);
};