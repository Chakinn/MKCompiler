#include "condition.hpp"


Condition::Condition(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue, unsigned int position) : Node(symbolTab,position) {
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
    validateIdentifier(lvalue, true);
    validateIdentifier(rvalue, true);

    std::vector<std::string> code;

    //if condition not met - jump to this label
    std::string label = Label::newLabel();
    code.push_back(label);    //for external node to read

    long long leftAddress = symbolTable->getAddress(lvalue);
    long long rightAddress = symbolTable->getAddress(rvalue);

    bool leftIsUnknown = false; //flag signaling that left address cannot be determined at compile time
    long long leftPointerAddress; // if leftIsUnkown this will hold address of array pointer
    long long rightPointerAddress;

    //create left array pointer if left address cannot be determined
    if(leftAddress == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(lvalue);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        leftPointerAddress = symbolTable->getFreeAddress();
        code.push_back("STORE "+std::to_string(leftPointerAddress));
        leftIsUnknown=true;
    }
    //create right array pointer if left address cannot be determined
    if(rightAddress == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(rvalue);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        rightPointerAddress = symbolTable->getFreeAddress();
        code.push_back("STORE "+std::to_string(rightPointerAddress));
        
        //make temp right variable
        long long helpAddress = symbolTable->getFreeAddress();
        code.push_back("LOADI " + std::to_string(rightPointerAddress));
        code.push_back("STORE " + std::to_string(helpAddress));
        rightAddress = helpAddress;
    }

    //if both are unknown
    if(leftIsUnknown) {
        code.push_back("LOADI " + std::to_string(leftPointerAddress));
        code.push_back("SUB " + std::to_string(rightAddress));
    }
    else {
        code.push_back("LOAD " + std::to_string(leftAddress));
        code.push_back("SUB " + std::to_string(rightAddress));
    }

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

    switch(op) {
        case COP::EQ:
        case COP::LE:
        case COP::GE: {
            std::string label1 = Label::newLabel();
            code.push_back(jump + label1);
            code.push_back("JUMP " + label);
            code.push_back(label1);
            break;
        }
        case COP::NEQ:
        case COP::GEQ:
        case COP::LEQ: {
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

