#include "expression.hpp"

Expression::Expression(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue) : Node(symbolTab) {
    lvalue = leftValue;
    op = operatorr;
    rvalue = rightValue;
};

std::string Expression::getCode() {
    std::stringstream code;

    long long leftAddress;
    long long rightAddress;
    //if left is number generate number address
    try {
        long long n = std::stoll(lvalue);
        leftAddress = symbolTable->generateNumber(n).first;
    }
    // else get address fromm symbol table
    catch(const std::invalid_argument) {
        leftAddress = symbolTable->getAddress(lvalue);
    }
        //if right is number generate number address
    try {
        long long n = std::stoll(rvalue);
        rightAddress = symbolTable->generateNumber(n).first;
    }
    // else get address fromm symbol table
    catch(const std::invalid_argument) {
        rightAddress = symbolTable->getAddress(rvalue);
    }

    code << "LOAD " << leftAddress; 

    if (op == "PLUS") {
        code << "ADD " << rightAddress << "\n";
    }
    else if (op == "MINUS") {
        code << "SUB" << rightAddress << "\n";
    }

    return code.str();
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