#include "handler.hpp"

Handler::Handler() {
    memoryManager = new MemoryManager();
    symbolTable = new SymbolTable(memoryManager);
}

Handler::~Handler(){}


std::string Handler::handleVarDeclaration(std::string identifier) {
    if(symbolTable->isDeclared(identifier)) {
        logError("Variable "+identifier+" was already declared",1);
    }
    long long address = memoryManager->allocateVar();
    symbolTable->declare(identifier, new Symbol(address));
    return "vardecl";
}

std::string Handler::handleArrayDeclaration(std::string identifier, std::string startIndex, std::string endIndex) {
    if(symbolTable->isDeclared(identifier)) {
        logError("Variable "+identifier+" was already declared",1);
    }
    long long low = std::stoll(startIndex);
    long long high = std::stoll(startIndex);

    if(high < low) {
        logError("Wrong array bounds",1);
    }

    long long address = memoryManager->allocateArray(high-low+1);
    symbolTable->declare(identifier, new Symbol(address,low,high));
    
    return "arrdecl";
}

std::string Handler::handleAssign(std::string identifier, std::string expressionIdentifier) {
    std::string nodeId = nodeIdentifier();
    Node* expression;
    try {
        expression = nodeTable.at(expressionIdentifier);
    }
    catch(std::out_of_range){
        std::cerr<<"expression not found";
    }
    nodeTable.erase(expressionIdentifier);
    Node* assignment = new Assignment(symbolTable,identifier,expression);
    nodeTable.insert(std::pair<std::string,Node*>(nodeId,assignment));
    
    return nodeId;
}


std::string Handler::handleIf(std::string condtitionIdentifier, bool ifElse) {
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
        branch = new BranchElse(symbolTable,condtition,codeBlocks.top(),elseBlock);
        codeBlocks.pop();
    } 
    else {
        branch = new Branch(symbolTable,condtition,codeBlocks.top());
        codeBlocks.pop();
    } 
    
    nodeTable.insert(std::make_pair(nodeId, branch));

    return nodeId;
}

std::string Handler::handleWhile(std::string condtitionIdentifier, bool doFirst) {
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
        branch = new DoWhile(symbolTable,conditition,codeBlocks.top());
    }
    else {
        branch = new WhileDo(symbolTable,conditition,codeBlocks.top());
    }
 
    codeBlocks.pop();
    nodeTable.insert(std::make_pair(nodeId, branch));

    return nodeId;
}


std::string Handler::handleFor(std::string identifier, std::string start, std::string end, bool down) {
    std::string nodeId = nodeIdentifier();
    long long address = memoryManager->allocateVar();
    symbolTable->declare(identifier,new Symbol(address));

    Node* forto;
    if (down) {
        forto = new ForDownTo(symbolTable,identifier,start,end,codeBlocks.top());
    }
    else {
        forto = new ForTo(symbolTable,identifier,start,end,codeBlocks.top()); 
    }
    codeBlocks.pop();
    nodeTable.insert(std::make_pair(nodeId, forto));
    return nodeId;
}

std::string Handler::handleRead(std::string identifier) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Read(symbolTable, identifier)));
    return nodeId;
}

std::string Handler::handleWrite(std::string value) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Write(symbolTable, value)));
    return nodeId;
}

std::string Handler::handleExpression(std::string value1, std::string op, std::string value2) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Expression(symbolTable, value1, op, value2)));
    return nodeId;
}

std::string Handler::handleCondition(std::string value1, std::string op, std::string value2) {
    std::string nodeId = nodeIdentifier();
    nodeTable.insert(std::make_pair(nodeId, new Condition(symbolTable, value1, op, value2)));
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
    std::vector<std::string> numberCode = memoryManager->getCode();
    code.insert(code.begin(),numberCode.begin(),numberCode.end());
    
    while(!codeBlocks.empty()) {
        CodeBlock* codeBlock = codeBlocks.top();
        codeBlocks.pop();
        std::vector<std::string> blockCode = codeBlock->getCode();
        code.insert(code.end(),blockCode.begin(),blockCode.end());
    }
    code.push_back("HALT");
    optimizer.manageLabels(&code);

    std::ofstream out1;
    out1.open("out.mr");

    for(std::string line : code) {
        out1 << line << "\n";
    }

}

void Handler::logError(std::string const& errorMessage, int lineNumber) {
    std::cout << "Error on line: " << lineNumber << ". " << errorMessage << "\n";
}

std::string Handler::nodeIdentifier() {
    nodeCounter++;
    return "ni:"+std::to_string(nodeCounter);
}

