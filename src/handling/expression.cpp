#include "expression.hpp"
#include <iostream>

Expression::Expression(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue) : Node(symbolTab) {
    lvalue = leftValue;
    op = operatorr;
    rvalue = rightValue;
};

std::vector<std::string> Expression::getCode() {
    std::vector<std::string> code;

    long long leftAddress = symbolTable->getAddress(lvalue);
    
    if (op =="") {
        code.push_back("LOAD " + std::to_string(leftAddress));
        return code;
    }
    long long rightAddress = symbolTable->getAddress(rvalue);

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