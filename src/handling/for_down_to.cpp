#include "for_down_to.hpp"

ForDownTo::ForDownTo(SymbolTable* symbolTab, std::string ident, std::string startIndex, std::string endIndex, CodeBlock* cBlock, unsigned int position) : Node(symbolTab, position) {
    identifier = ident;
    start = startIndex;
    end = endIndex;
    codeBlock = cBlock;
}

ForDownTo::~ForDownTo(){}

std::vector<std::string> ForDownTo::getCode() {
    validateIdentifier(start, true);
    validateIdentifier(end, true);

    long long address = symbolTable->getFreeAddress();
    symbolTable->declare(identifier,new Symbol(address, true));

    std::vector<std::string> code;

    //calculate number of iters
    long long startAddress = symbolTable->getAddress(start);
    if(startAddress == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(start);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        long long helpAddress = symbolTable->getFreeAddress();
        code.push_back("LOADI 0");
        code.push_back("STORE " + std::to_string(helpAddress));
        startAddress = helpAddress;
    }
    long long endIndexAddress = symbolTable->getAddress(end);
    if(endIndexAddress == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(end);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        long long helpAddress = symbolTable->getFreeAddress();
        code.push_back("LOADI 0");
        code.push_back("STORE " + std::to_string(helpAddress));
        endIndexAddress = helpAddress;
    }
    long long endAddress = symbolTable->getFreeAddress();
    long long itAddress = symbolTable->getAddress(identifier);

    std::string startLabel = Label::newLabel();
    std::string endLabel = Label::newLabel();

    code.push_back("LOAD " + std::to_string(endIndexAddress));
    code.push_back("STORE " + std::to_string(endAddress));

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

    symbolTable->undeclare(identifier);

    return code;
}