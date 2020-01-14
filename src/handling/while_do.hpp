#pragma once

#include "node.hpp"
#include "condition.hpp"
#include "code_block.hpp"

class WhileDo : public Node {
protected:
    Node* condition;
    CodeBlock* codeBlock;
public:
    WhileDo(SymbolTable* symbolTab, Node* cond, CodeBlock* cBlock);
    ~WhileDo(){}

    virtual std::vector<std::string> getCode();
};