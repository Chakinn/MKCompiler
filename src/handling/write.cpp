#include "write.hpp"

Write::Write(SymbolTable* symbolTable, std::string val) : Node(symbolTable) {
    value = val;
}

std::vector<std::string> Write::getCode() {
    std::vector<std::string> code;

    long long address = symbolTable->getAddress(value);

    code.push_back("LOAD "+std::to_string(address));
    code.push_back("PUT");
    return code;
}