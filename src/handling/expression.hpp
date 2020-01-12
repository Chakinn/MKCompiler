#pragma once

#include <string>
#include <sstream>
#include "node.hpp"

class Expression : public Node {
    std::string lvalue;
    std::string op;
    std::string rvalue;

public:
    Expression(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue);
    virtual ~Expression(){};

    std::string getCode();

    std::string getLvalue();
    std::string getRvalue();
    std::string getOp();
};