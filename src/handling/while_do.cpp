#include "while_do.hpp"    
    
WhileDo::WhileDo(SymbolTable* symbolTab, Node* cond, CodeBlock* cBlock) : Node(symbolTab) {
    condition = cond;
    codeBlock = cBlock;
}

std::vector<std::string> WhileDo::getCode() {
    std::vector<std::string> code;

    std::vector<std::string> conditionCode = condition->getCode();
    std::string endLabel = conditionCode[0];

    std::string startLabel = Condition::newLabel();
    code.push_back(startLabel);
    code.insert(code.end(), conditionCode.begin()+1, conditionCode.end());

    std::vector<std::string> blockCode = codeBlock->getCode();
    code.insert(code.end(),blockCode.begin(),blockCode.end());
    code.push_back("JUMP "+startLabel);
    code.push_back(endLabel);

    return code;
}