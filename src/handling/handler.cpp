#include "handler.hpp"

Handler::Handler() {
    memoryManager = new MemoryManager();
    symbolTable = new SymbolTable(memoryManager);
}

Handler::~Handler(){}


std::string Handler::handleVarDeclaration(std::string identifier, unsigned int position) {
    if(symbolTable->isDeclared(identifier)) {
        Log::logError("Variable "+identifier+" was already declared",position);
    }
    long long address = memoryManager->allocateVar();
    symbolTable->declare(identifier, new Symbol(address, false));
    return "vardecl";
}

std::string Handler::handleArrayDeclaration(std::string identifier, std::string startIndex, std::string endIndex, unsigned int position) {
    if(symbolTable->isDeclared(identifier)) {
        Log::logError("Variable "+identifier+" was already declared",position);
    }
    long long low = std::stoll(startIndex);
    long long high = std::stoll(endIndex);

    if(high < low) {
        Log::logError("Wrong array bounds",position);
    }

    long long address = memoryManager->allocateArray(low,high);
    symbolTable->declare(identifier, new Symbol(address,low,high));
    
    return "arrdecl";
}

std::string Handler::handleAssign(std::string identifier, std::string expressionIdentifier, unsigned int position) {
    symbolTable->getAddress(identifier);
    std::string nodeId = nodeIdentifier();
    Node* expression;
    try {
        expression = nodeTable.at(expressionIdentifier);
    }
    catch(std::out_of_range){
        std::cerr<<"expression not found";
    }
    nodeTable.erase(expressionIdentifier);
    Node* assignment = new Assignment(symbolTable,identifier,expression, position);
    nodeTable.insert(std::pair<std::string,Node*>(nodeId,assignment));

    try {
        symbolTable->getSymbol(identifier)->setInitialized(true);
    }
    catch(std::exception){}
    
    return nodeId;
}


std::string Handler::handleIf(std::string condtitionIdentifier, bool ifElse, unsigned int position) {
    std::string nodeId = nodeIdentifier();
    Node* condtition;
    try {
        condtition = nodeTable.at(condtitionIdentifier);
    }
    catch(std::out_of_range){
        std::cerr<<"condition not found " << condtitionIdentifier << "\n";
    }
    nodeTable.erase(condtitionIdentifier);
    Node* branch;
    if(ifElse) {
        CodeBlock* elseBlock = codeBlocks.top();
        codeBlocks.pop();
        branch = new BranchElse(symbolTable,condtition,codeBlocks.top(),elseBlock, position);
        codeBlocks.pop();
    } 
    else {
        branch = new Branch(symbolTable,condtition,codeBlocks.top(), position);
        codeBlocks.pop();
    } 
    
    nodeTable.insert(std::make_pair(nodeId, branch));

    return nodeId;
}

std::string Handler::handleWhile(std::string condtitionIdentifier, bool doFirst, unsigned int position) {
    std::string nodeId = nodeIdentifier();
    Node* conditition;
    try {
        conditition = nodeTable.at(condtitionIdentifier);
    }
    catch(std::out_of_range){
        std::cerr<<"condition not found " << condtitionIdentifier << "\n";
    }
    nodeTable.erase(condtitionIdentifier);

    Node* branch;
    if (doFirst) {
        branch = new DoWhile(symbolTable,conditition,codeBlocks.top(), position);
    }
    else {
        branch = new WhileDo(symbolTable,conditition,codeBlocks.top(), position);
    }
 
    codeBlocks.pop();
    nodeTable.insert(std::make_pair(nodeId, branch));

    return nodeId;
}


std::string Handler::handleFor(std::string identifier, std::string start, std::string end, bool down, unsigned int position) {
    std::string nodeId = nodeIdentifier();

    Node* forto;
    if (down) {
        forto = new ForDownTo(symbolTable,identifier,start,end,codeBlocks.top(), position);
    }
    else {
        forto = new ForTo(symbolTable,identifier,start,end,codeBlocks.top(), position); 
    }
    codeBlocks.pop();
    nodeTable.insert(std::make_pair(nodeId, forto));
    return nodeId;
}

std::string Handler::handleRead(std::string identifier, unsigned int position) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Read(symbolTable, identifier, position)));
    try {
        symbolTable->getSymbol(identifier)->setInitialized(true);
    }
    catch(std::exception){}
    return nodeId;
}

std::string Handler::handleWrite(std::string value, unsigned int position) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Write(symbolTable, value, position)));
    return nodeId;
}

std::string Handler::handleExpression(std::string value1, std::string op, std::string value2, unsigned int position) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Expression(symbolTable, value1, op, value2, position)));
    return nodeId;
}

std::string Handler::handleCondition(std::string value1, std::string op, std::string value2, unsigned int position) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Condition(symbolTable, value1, op, value2, position)));
    return nodeId;
}

std::string Handler::handleCommand(std::string nodeId) {
    codeBlocks.top()->addCommand(nodeTable.at(nodeId));
    return "!";
}

std::string Handler::handleFirstCommand(std::string nodeId) {
    codeBlocks.push(new CodeBlock());
    codeBlocks.top()->addCommand(nodeTable.at(nodeId));
    return "!";
}

void Handler::handleProgram() {
    std::vector<std::string> code;
    code.reserve(1000);
    
    while(!codeBlocks.empty()) {
        CodeBlock* codeBlock = codeBlocks.top();
        codeBlocks.pop();
        std::vector<std::string> blockCode = codeBlock->getCode();
        code.insert(code.end(),blockCode.begin(),blockCode.end());
    }
    std::vector<std::string> numberCode = memoryManager->getCode();
    code.insert(code.begin(),numberCode.begin(),numberCode.end());
    code.push_back("HALT");
    optimizer.manageLabels(&code);

    Writer::writeToFile(&code);

}



std::string Handler::nodeIdentifier() {
    nodeCounter++;
    return "ni:"+std::to_string(nodeCounter);
}
