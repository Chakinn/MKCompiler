#pragma once

#include "node.hpp"
#include "condition.hpp"
#include "code_block.hpp"
#include "while_do.hpp"

class DoWhile : public WhileDo {

public:
    DoWhile(SymbolTable* symbolTab, Node* cond, CodeBlock* cBlock);
    ~DoWhile(){}

    std::vector<std::string> getCode();
};