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
        op = COP::LEQ;
    }
    else if (operatorr == "GEQ") {
        op = COP::GEQ;   
    }
};

std::vector<std::string> Condition::getCode() {
    
    std::vector<std::string> code;

    long long leftAddress = symbolTable->getAddress(lvalue);
    long long rightAddress = symbolTable->getAddress(rvalue);

    std::string jump;
    
    switch(op) {
        case COP::EQ:
        case COP::NEQ: {
            jump = "JZERO ";
            break;
        }
        case COP::LE:
        case COP::GEQ: {
            jump = "JNEG ";
            break;
        }
        case COP::GE:
        case COP::LEQ: {
            jump = "JPOS ";
            break;
        }
    }

    //if condition not met - jump to this label
    std::string label = newLabel();
    code.push_back(label);    //for external node to read

    switch(op) {
        case COP::EQ:
        case COP::LE:
        case COP::GE: {
            std::string label1 = newLabel();
            code.push_back("LOAD " + std::to_string(leftAddress));
            code.push_back("SUB " + std::to_string(rightAddress));
            code.push_back(jump + label1);
            code.push_back("JUMP " + label);
            code.push_back(label1);
            break;
        }
        case COP::NEQ:
        case COP::GEQ:
        case COP::LEQ: {
            code.push_back("LOAD " + std::to_string(leftAddress));
            code.push_back("SUB " + std::to_string(rightAddress));
            code.push_back(jump + label);
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