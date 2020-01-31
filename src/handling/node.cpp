#include "node.hpp"

Node::Node(SymbolTable* symbolTab) {
    symbolTable = symbolTab;
}

void Node::validateIdentifier(std::string identifier, bool needInitialized) {
    
    if(symbolTable->isNumber(identifier) || identifier == "") {
        return;
    }

    int bracketIndex = identifier.find_first_of('(');
    const std::string id = bracketIndex == -1 ? identifier : identifier.substr(0,bracketIndex);

    //check declaration
    if(!symbolTable->isDeclared(id)) {
        Log::logError("Variable " + id + " not declared",1);
    }
    //check initialization if needed
    Symbol* symbol = symbolTable->getSymbol(id);
    if(needInitialized && !symbol->getInitialized()) {
        Log::logError("Variable " + id + " not initialized",1);
    }
    //check correct array/var access
    
    switch (symbol->getType())
    {
    case SymbolType::NUMBER: {
        if(bracketIndex != -1) {
            Log::logError("Variable " + id + " is a number and should not be accessed as array",1);
        }
        break;
    }
    case SymbolType::ARRAY: {
        if(bracketIndex == -1) {
            Log::logError("Variable " + id + " is an array and should not be accessed as number",1);
        }
        std::string num = identifier.substr(bracketIndex).substr(1);
        num = num.substr(0,num.length()-1);
        validateIdentifier(num,true);
        break;
    } 
    case SymbolType::ITERATOR: {
        if(bracketIndex != -1) {
            Log::logError("Variable " + id + " is a number and should not be accessed as array",1);
        }
        if(!needInitialized) {
            Log::logError("You cannot change iterator inside for loop",1);
        }
        break;
    }
        
    }
}
