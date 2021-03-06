#pragma once

#include "node.hpp"
#include "label.hpp"
#include "code_block.hpp"

class ForDownTo : public Node {
protected:
    std::string identifier;
    std::string start;
    std::string end;
    CodeBlock* codeBlock;
public:
    ForDownTo(SymbolTable* symbolTab, std::string identifier, std::string startIndex, std::string endIndex, CodeBlock* cBlock, unsigned int position);
    ~ForDownTo();

    virtual std::vector<std::string> getCode();
};