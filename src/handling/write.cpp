#include "write.hpp"

Write::Write(SymbolTable* symbolTable, std::string val) : Node(symbolTable) {
    value = val;
}

std::vector<std::string> Write::getCode() {
    std::vector<std::string> code;

    long long address;
    //if left is number generate number address
    try {
        long long n = std::stoll(value);
        address = symbolTable->generateNumber(n);
    }
    // else get address fromm symbol table
    catch(const std::invalid_argument) {
        address = symbolTable->getAddress(value);
    }

    code.push_back("LOAD "+std::to_string(address));
    code.push_back("PUT");
    return code;
}