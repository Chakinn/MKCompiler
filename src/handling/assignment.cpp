#include "assignment.hpp"

Assignment::Assignment(SymbolTable* symolTab, std::string identifier, Node* expression) : Node(symolTab) {
    id = identifier;
    exp = expression;
}

Assignment::~Assignment() {

}
std::vector<std::string> Assignment::getCode() {
    std::vector<std::string> code;

    std::vector<std::string> expressionCode = exp->getCode();
    code.insert(code.begin(),expressionCode.begin(),expressionCode.end());
    long long address = symbolTable->getAddress(id);
    code.push_back("STORE " + std::to_string(address));

    return code;
}