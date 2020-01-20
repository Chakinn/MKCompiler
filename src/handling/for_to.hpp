#pragma once

#include "node.hpp"
#include "label.hpp"
#include "code_block.hpp"

class ForTo : public Node {
protected:
    std::string identifier;
    std::string start;
    std::string end;
    CodeBlock* codeBlock;
public:
    ForTo(SymbolTable* symbolTab, std::string identifier, std::string startIndex, std::string endIndex, CodeBlock* cBlock);
    ~ForTo();

    virtual std::vector<std::string> getCode();
};