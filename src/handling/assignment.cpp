#include "assignment.hpp"

Assignment::Assignment(SymbolTable* symolTab, std::string identifier, Node* expression) : Node(symolTab) {
    id = identifier;
    exp = expression;
}

Assignment::~Assignment() {

}
std::vector<std::string> Assignment::getCode() {
    std::vector<std::string> code;

    long long address = symbolTable->getAddress(id);
    if (address == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(id);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        long long pointerAddress = symbolTable->getFreeAddress();
        code.push_back("STORE "+std::to_string(pointerAddress));
        std::vector<std::string> expressionCode = exp->getCode();
        code.insert(code.end(),expressionCode.begin(),expressionCode.end());
        code.push_back("STOREI " + std::to_string(pointerAddress));
    }
    else {
        std::vector<std::string> expressionCode = exp->getCode();
        code.insert(code.end(),expressionCode.begin(),expressionCode.end());
        code.push_back("STORE " + std::to_string(address));
    }
    return code;
}