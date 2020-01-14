#include "code_block.hpp"

CodeBlock::CodeBlock() {

}

CodeBlock::~CodeBlock() {

}

void CodeBlock::addCommand(Node* command) {
    commands.push_back(command);
}

std::vector<std::string> CodeBlock::getCode() {
    std::vector<std::string> code;
    for(Node* command : commands) {
        std::vector<std::string> commandCode = command->getCode();
        code.insert(code.end(),commandCode.begin(),commandCode.end());
    }
    return code;
}