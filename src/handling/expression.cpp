#include "expression.hpp"
#include <iostream>

Expression::Expression(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue) : Node(symbolTab) {
    lvalue = leftValue;
    op = operatorr;
    rvalue = rightValue;
};

std::vector<std::string> Expression::getCode() {
    
    std::vector<std::string> code;

    long long leftAddress;
    long long rightAddress;
    //if left is number generate number address
    try {
        long long n = std::stoll(lvalue);
        leftAddress = symbolTable->generateNumber(n);
    }
    // else get address fromm symbol table
    catch(const std::invalid_argument) {
        leftAddress = symbolTable->getAddress(lvalue);
    }
        //if right is number generate number address
    try {
        long long n = std::stoll(rvalue);
        rightAddress = symbolTable->generateNumber(n);
    }
    // else get address fromm symbol table
    catch(const std::invalid_argument) {
        rightAddress = symbolTable->getAddress(rvalue);
    }

    code.push_back("LOAD " + std::to_string(leftAddress)); 

    if (op == "PLUS") {
        code.push_back("ADD " + std::to_string(rightAddress));
    }
    else if (op == "MINUS") {
        code.push_back("SUB" + std::to_string(rightAddress));
    }

    return code;

}

std::string Expression::getLvalue() {
    return lvalue;
}
std::string Expression::getRvalue() {
    return rvalue;
}
std::string Expression::getOp() {
    return op;
}