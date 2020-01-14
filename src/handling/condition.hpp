#pragma once

#include "node.hpp"

enum class COP {EQ,NEQ,LE,GE,LEQ,GEQ};

class Condition : public Node {
    std::string lvalue;
    COP op;
    std::string rvalue;
    static int labelCounter;

public:
    Condition(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue);
    virtual ~Condition(){};

    std::vector<std::string> getCode();

    std::string getLvalue();
    std::string getRvalue();
    COP getOp();

    static std::string lastLabel();
    static std::string newLabel();
    
};