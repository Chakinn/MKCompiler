#include "condition.hpp"


Condition::Condition(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue) : Node(symbolTab) {
    lvalue = leftValue;
    rvalue = rightValue;

    if (operatorr == "EQ") {
        op = COP::EQ;
    }
    else if (operatorr == "NEQ") {
        op = COP::NEQ;
    }
    else if (operatorr == "LE") {
        op = COP::LE;
    }
    else if (operatorr == "GE") {
        op = COP::GE;
    }
    else if (operatorr == "LEQ") {
        op = COP::GEQ;
    }
    else if (operatorr == "GEQ") {
        op = COP::GEQ;
    }
};

std::vector<std::string> Condition::getCode() {
    
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

    std::string jump;

    switch(op) {
        case COP::EQ:
        case COP::NEQ: {
            jump = "JZERO ";
            break;
        }
        case COP::LE:
        case COP::LEQ: {
            jump = "JNEG ";
            break;
        }
        case COP::GE:
        case COP::GEQ: {
            jump = "JPOS ";
            break;
        }
    }

    switch(op) {
        case COP::EQ:
        case COP::LE:
        case COP::GE: {
            std::string label1 = newLabel();
            std::string label2 = newLabel();

            code.push_back(label2);    //for external node to read

            code.push_back("LOAD " + std::to_string(leftAddress));
            code.push_back("SUB " + std::to_string(rightAddress));
            code.push_back(jump + label1);
            code.push_back("JUMP " + label2);
            code.push_back(label1);
            /*switch(op) {
                case COP::EQ: {
                    code.push_back("JNEG " + label2);
                    code.push_back("JPOS " + label2);
                    break;
                }
                case COP::LE: {
                    code.push_back("JZERO " + label2);
                    code.push_back("JPOS " + label2);
                    break;
                }
                case COP::GE: {
                    code.push_back("JZERO " + label2);
                    code.push_back("JNEG " + label2);
                    break;
                }
            }*/

            break;
        }
        case COP::NEQ:
        case COP::GEQ:
        case COP::LEQ: {
            std::string label1 = newLabel();

            code.push_back(label1); //for external node to read

            code.push_back("LOAD " + std::to_string(leftAddress));
            code.push_back("SUB " + std::to_string(rightAddress));
            code.push_back(jump + label1);
            break;
        }
    }
    
    return code;

}

std::string Condition::getLvalue() {
    return lvalue;
}
std::string Condition::getRvalue() {
    return rvalue;
}
COP Condition::getOp() {
    return op;
}

int Condition::labelCounter = 0;
std::string Condition::newLabel() {
    labelCounter++;
    return "L"+std::to_string(labelCounter);
}
std::string Condition::lastLabel() {
    return "L"+std::to_string(labelCounter);
}