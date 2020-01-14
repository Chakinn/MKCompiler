#pragma once

#include <deque>
#include "node.hpp"

class CodeBlock {
    std::deque<Node*> commands;

public:
    CodeBlock();
    ~CodeBlock();

    void addCommand(Node* command);
    std::vector<std::string> getCode();
};