#include "for_to.hpp"

ForTo::ForTo(SymbolTable* symbolTab, std::string ident, std::string startIndex, std::string endIndex, CodeBlock* cBlock) : Node(symbolTab) {
    identifier = ident;
    start = startIndex;
    end = endIndex;
    codeBlock = cBlock;
}

ForTo::~ForTo(){}

std::vector<std::string> ForTo::getCode() {
    std::vector<std::string> code;

    //calculate number of iters
    long long startAddress = symbolTable->getAddress(start);
    long long endAddress = symbolTable->getAddress(end);
    long long itAddress = symbolTable->getAddress(identifier);

    std::string startLabel = Condition::newLabel();

    code.push_back("LOAD " + std::to_string(startAddress));
    code.push_back("STORE " + std::to_string(itAddress));

    code.push_back(startLabel);
    std::vector<std::string> blockCode = codeBlock->getCode();
    code.insert(code.end(),blockCode.begin(),blockCode.end());

    code.push_back("LOAD " + std::to_string(itAddress));
    code.push_back("INC");
    code.push_back("STORE " + std::to_string(itAddress));
    code.push_back("SUB " + std::to_string(endAddress));
    std::string endLabel = Condition::newLabel();
    code.push_back("JPOS "+endLabel);
    code.push_back("JUMP " + startLabel);
    code.push_back(endLabel);

    return code;
}