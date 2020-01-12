#pragma once

#include "symbol_table.hpp"

class Node {
protected:
    SymbolTable* symbolTable;
public:
    Node(SymbolTable* symbolTable);
    ~Node(){};
    virtual std::string getCode(){return "code";};
};