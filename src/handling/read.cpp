#include "read.hpp"

Read::Read(SymbolTable* symbolTable, std::string varIdentifier) : Node(symbolTable) {
    variableIdentifier = varIdentifier;
}

std::vector<std::string> Read::getCode() {
    std::vector<std::string> code;

    int bracketIndex = variableIdentifier.find_first_of('(');
    long long address;
    if (bracketIndex < 0) {
        //var
        address = symbolTable->getAddress(variableIdentifier);
    }
    else {
        //array
    }

    code.push_back("GET");
    code.push_back("STORE "+std::to_string(address));
    return code;
}