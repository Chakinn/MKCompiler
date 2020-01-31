#pragma once

#include "node.hpp"



class Write : public Node {
    std::string value;

public:
    Write(SymbolTable* SymbolTable, std::string val, unsigned int position);
    virtual ~Write(){}

    std::vector<std::string> getCode();
    
};