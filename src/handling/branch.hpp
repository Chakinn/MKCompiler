#pragma once

#include "node.hpp"
#include "condition.hpp"
#include "code_block.hpp"

class Branch : public Node {
    Node* condition;
    CodeBlock* codeBlock;
public:
    Branch(SymbolTable* symbolTab, Node* cond, CodeBlock* cBlock);
    ~Branch();

    std::vector<std::string> getCode();
};