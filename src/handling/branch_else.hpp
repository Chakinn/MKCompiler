#pragma once

#include "node.hpp"
#include "condition.hpp"
#include "code_block.hpp"
#include "label.hpp"

class BranchElse : public Node {
    Node* condition;
    CodeBlock* ifCodeBlock;
    CodeBlock* elseCodeBlock;
public:
    BranchElse(SymbolTable* symbolTab, Node* cond, CodeBlock* ifBlock, CodeBlock* elseBlock);
    ~BranchElse();

    std::vector<std::string> getCode();
};