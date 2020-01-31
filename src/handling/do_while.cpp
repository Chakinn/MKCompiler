#include "do_while.hpp"    
    
DoWhile::DoWhile(SymbolTable* symbolTab, Node* cond, CodeBlock* cBlock, unsigned int position) : WhileDo(symbolTab, cond, cBlock, position) {}

std::vector<std::string> DoWhile::getCode() {
    std::vector<std::string> code;

    std::vector<std::string> conditionCode = condition->getCode();
    std::string endLabel = conditionCode[0];
    
    std::string startLabel = Label::newLabel();
    code.push_back(startLabel);

    std::vector<std::string> blockCode = codeBlock->getCode();
    code.insert(code.end(),blockCode.begin(),blockCode.end());

    code.insert(code.end(), conditionCode.begin()+1, conditionCode.end());
    code.push_back("JUMP "+startLabel);
    code.push_back(endLabel);

    return code;
}