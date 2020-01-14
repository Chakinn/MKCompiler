#include "branch.hpp"

Branch::Branch(SymbolTable* symbolTab, Node* cond, CodeBlock* cBlock) : Node(symbolTab) {
    condition= cond;
    codeBlock = cBlock;
}
Branch::~Branch() {}

std::vector<std::string> Branch::getCode() {
    std::vector<std::string> code;

    std::vector<std::string> conditionCode = condition->getCode();
    std::string label = conditionCode[0];

    code.insert(code.begin(), conditionCode.begin()+1, conditionCode.end());

    std::vector<std::string> blockCode = codeBlock->getCode();
    code.insert(code.end(),blockCode.begin(),blockCode.end());
    code.push_back(label);

    return code;
}