#include "for_down_to.hpp"

ForDownTo::ForDownTo(SymbolTable* symbolTab, std::string ident, std::string startIndex, std::string endIndex, CodeBlock* cBlock) : Node(symbolTab) {
    identifier = ident;
    start = startIndex;
    end = endIndex;
    codeBlock = cBlock;
}

ForDownTo::~ForDownTo(){}

std::vector<std::string> ForDownTo::getCode() {
    std::vector<std::string> code;

    //calculate number of iters
    long long startAddress = symbolTable->getAddress(start);
    long long endAddress = symbolTable->getAddress(end);
    long long itAddress = symbolTable->getAddress(identifier);

    std::string startLabel = Label::newLabel();
    std::string endLabel = Label::newLabel();

    code.push_back("LOAD " + std::to_string(startAddress));
    code.push_back("STORE " + std::to_string(itAddress));

    code.push_back(startLabel);
    code.push_back("SUB " + std::to_string(endAddress));
    code.push_back("JNEG "+ endLabel);
    std::vector<std::string> blockCode = codeBlock->getCode();
    code.insert(code.end(),blockCode.begin(),blockCode.end());

    code.push_back("LOAD " + std::to_string(itAddress));
    code.push_back("DEC");
    code.push_back("STORE " + std::to_string(itAddress));
    
    
    code.push_back("JUMP " + startLabel);
    code.push_back(endLabel);

    return code;
}