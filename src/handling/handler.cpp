#include "handler.hpp"

Handler::Handler() {
    memoryManager = new MemoryManager();
    symbolTable = new SymbolTable(memoryManager);
    std::cout<<memoryManager->generateFirstNumbers(5);
    std::cout<<"\n"<<memoryManager->generateNumber(10).second;
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
    
    return "arrdecl";
}

std::string Handler::handleAssign(std::string identifier, std::string expressionIdentifier) {
    //Expression* expression = static_cast<Expression*>(nodeTable[expressionIdentifier]);
    Node* expression = nodeTable.at(expressionIdentifier);
    //std::cout<<expressionIdentifier;
    //std::cout << expression->getCode();
    //std::cout << codeGenerator->generateExpression(expression);
    
    return "assign";
}


std::string handleIf() {
    return "if";
}

std::string handleWhile() {
    return "while";
}

std::string handleFor(std::string identifier) {
    return "for";
}

std::string handleRead(std::string identifier) {
    return "read";
}

std::string handleWrite(std::string value) {
    return "write";
}

std::string Handler::handleExpression(std::string value1, std::string op, std::string value2) {
    nodeTable.insert(std::make_pair("abc", new Expression(symbolTable, value1, op, value2)));
    return "abc";
}

std::string Handler::handleCondition(std::string value1, std::string op, std::string value2) {
    //std::cout<<"Condition " << value1 << op << value2 << "\n";
    return "condition";
}

void Handler::logError(std::string const& errorMessage, int lineNumber) {
    std::cout << "Error on line: " << lineNumber << ". " << errorMessage << "\n";
}

std::string Handler::nodeIdentifier() {
    nodeCounter++;
    return std::to_string(nodeCounter);
}

