#pragma once

#include "node.hpp"
#include "label.hpp"

enum class EOP {NONE, PLUS, MINUS, TIMES, DIV, MOD};

class Expression : public Node {
    std::string lvalue;
    EOP op;
    std::string rvalue;

public:
    Expression(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue);
    virtual ~Expression(){};

    std::vector<std::string> getCode();

    std::string getLvalue();
    std::string getRvalue();
    EOP getOp();
private:
    int powerOfTwo(long long number);
    std::vector<int> binaryRepresentation(long long number);
    void multiplyNumberCode(std::vector<std::string>& code, long long varAddress, long long number);
    void multiplyVarCode(std::vector<std::string>& code, long long leftAddress, long long rightAddress);
    void divisionNumberCode(std::vector<std::string>& code, long long varAddress, long long number);
    void divisionCode(std::vector<std::string>& code, long long leftAddress, long long rightAddress, std::string mode);
    void modNumberCode(std::vector<std::string>& code, long long varAddress, long long number);
};