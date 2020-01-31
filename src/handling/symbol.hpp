#pragma once

#include <string>

enum class SymbolType {NUMBER, ARRAY, ITERATOR};

class Symbol {
    SymbolType type;
    long long address;
    long long low;
    long long high;
    bool initialized;

public:
    /* construct int symbol */
    Symbol(long long memoryAddress, bool isIterator);
    /* construct array symbol*/
    Symbol(long long memoryAddress, long long lowerIndex, long long upperIndex);
    virtual ~Symbol(){};

    long long getAddress();
    long long getLow();
    long long getHigh();
    SymbolType getType();
    bool getInitialized();

    void setInitialized(bool b);
    
};