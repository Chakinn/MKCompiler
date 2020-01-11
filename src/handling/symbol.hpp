#pragma once

#include <string>

enum class SymbolType {NUMBER, ARRAY};

class Symbol {
    SymbolType type;

public:
    Symbol(SymbolType SymbolType);
    virtual ~Symbol(){};
};