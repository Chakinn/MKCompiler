#include "write.hpp"

Write::Write(SymbolTable* symbolTable, std::string val) : Node(symbolTable) {
    value = val;
}

std::vector<std::string> Write::getCode() {
    validateIdentifier(value, true);

    std::vector<std::string> code;

    long long address = symbolTable->getAddress(value);
    if (address == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(value);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        code.push_back("LOADI 0");
    }
    else {
        code.push_back("LOAD " + std::to_string(address));
    }

    code.push_back("PUT");
    return code;
}