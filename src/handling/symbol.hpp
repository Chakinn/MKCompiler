#pragma once

#include <string>

enum class SymbolType {NUMBER, ARRAY};

class Symbol {
    SymbolType type;
    long long address;
    long long low;
    long long high;

public:
    /* construct int symbol */
    Symbol(long long memoryAddress);
    /* construct array symbol*/
    Symbol(long long memoryAddress, long long lowerIndex, long long upperIndex);
    virtual ~Symbol(){};

    long long getAddress();
    long long getLow();
    long long getHigh();
};