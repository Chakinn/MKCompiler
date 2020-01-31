#pragma once

#include <string>
#include <unordered_map>
#include <stack>
#include <deque>
#include <memory>
#include <utility>
#include <iostream>


#include "symbol.hpp"
#include "symbol_table.hpp"
#include "memory_manager.hpp"
#include "expression.hpp"
#include "assignment.hpp"
#include "code_block.hpp"
#include "condition.hpp"
#include "branch.hpp"
#include "branch_else.hpp"
#include "while_do.hpp"
#include "do_while.hpp"
#include "for_to.hpp"
#include "for_down_to.hpp"

#include "write.hpp"
#include "read.hpp"
#include "../back/optimizer.hpp"
#include "../front/writer.hpp"

#include "log.hpp"

class Handler {
    std::unordered_map<std::string, Node*> nodeTable;
    SymbolTable* symbolTable;
    MemoryManager* memoryManager;
    Optimizer optimizer;
    std::stack<CodeBlock*> codeBlocks;
    int nodeCounter = 0;

public:
    Handler();
    ~Handler();
    std::string handleVarDeclaration(std::string identifier, unsigned int position);
    std::string handleArrayDeclaration(std::string identifier, std::string startIndex, std::string endIndex, unsigned int position);
    std::string handleAssign(std::string identifier, std::string expressonIdentifier, unsigned int position);
    std::string handleIf(std::string conditionIdentifier, bool ifElse, unsigned int position);
    std::string handleWhileDo(std::string condtitionIdentifier, unsigned int position);
    std::string handleWhile(std::string condtitionIdentifier, bool doFirs, unsigned int position);
    std::string handleFor(std::string identifier, std::string start, std::string end, bool down, unsigned int position);

    std::string handleRead(std::string identifier, unsigned int position);
    std::string handleWrite(std::string value, unsigned int position);
    std::string handleExpression(std::string value1, std::string op, std::string value2, unsigned int position);
    std::string handleCondition(std::string value1, std::string op, std::string value2, unsigned int position);
    std::string handleCommand(std::string nodeId);
    std::string handleFirstCommand(std::string nodeId);
    void handleProgram();

    

private:
    
    long long nextAddress();
    std::string nodeIdentifier();
    
};