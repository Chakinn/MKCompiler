#include "branch_else.hpp"

BranchElse::BranchElse(SymbolTable* symbolTab, Node* cond, CodeBlock* ifBlock, CodeBlock* elseBlock, unsigned int position) : Node(symbolTab, position) {
    condition= cond;
    ifCodeBlock = ifBlock;
    elseCodeBlock = elseBlock;
}
BranchElse::~BranchElse() {}

std::vector<std::string> BranchElse::getCode() {
    std::vector<std::string> code;

    std::vector<std::string> conditionCode = condition->getCode();
    std::string label = conditionCode[0];

    code.insert(code.begin(), conditionCode.begin()+1, conditionCode.end());
    //if code
    std::vector<std::string> ifBlockCode = ifCodeBlock->getCode();
    code.insert(code.end(),ifBlockCode.begin(),ifBlockCode.end());

    std::string endLabel = Label::newLabel();
    code.push_back("JUMP " + endLabel);
    //label
    code.push_back(label);
    //else code
    std::vector<std::string> elseBlockCode = elseCodeBlock->getCode();
    code.insert(code.end(),elseBlockCode.begin(),elseBlockCode.end());
    code.push_back(endLabel);


    return code;
}