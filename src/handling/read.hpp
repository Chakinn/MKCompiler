#pragma once

#include "node.hpp"



class Read : public Node {
    std::string variableIdentifier;
public:
    Read(SymbolTable* SymbolTable, std::string varIdentifier);
    virtual ~Read(){}

    std::vector<std::string> getCode();
    
};