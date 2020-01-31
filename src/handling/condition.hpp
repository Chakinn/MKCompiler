#pragma once

#include "node.hpp"
#include "label.hpp"

enum class COP {EQ,NEQ,LE,GE,LEQ,GEQ};

class Condition : public Node {
    std::string lvalue;
    COP op;
    std::string rvalue;
    

public:
    Condition(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue, unsigned int position);
    virtual ~Condition(){};

    std::vector<std::string> getCode();

    std::string getLvalue();
    std::string getRvalue();
    COP getOp();

};