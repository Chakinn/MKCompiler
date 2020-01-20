#include "read.hpp"

Read::Read(SymbolTable* symbolTable, std::string varIdentifier) : Node(symbolTable) {
    variableIdentifier = varIdentifier;
}

std::vector<std::string> Read::getCode() {
    std::vector<std::string> code;

    long long address = symbolTable->getAddress(variableIdentifier);
    if (address == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(variableIdentifier);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        long long arrayPointerAddress = symbolTable->getFreeAddress();
        code.push_back("STORE " + std::to_string(arrayPointerAddress));
        code.push_back("GET");
        code.push_back("STOREI " + std::to_string(arrayPointerAddress));
    }
    else {
        code.push_back("GET");
        code.push_back("STORE "+std::to_string(address));
    }

    
    return code;
}