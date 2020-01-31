#pragma once

#include "node.hpp"
#include "expression.hpp"

class Assignment : public Node {
    std::string id;
    Node* exp;

public:
    Assignment(SymbolTable* symolTab, std::string identifier, Node* expression, unsigned int position);
    ~Assignment();
    std::vector<std::string> getCode();
};