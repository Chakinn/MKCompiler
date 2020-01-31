#include "symbol.hpp"

Symbol::Symbol(long long memoryAddress, bool isIterator) {
    address = memoryAddress;
    type = isIterator ? SymbolType::ITERATOR : SymbolType::NUMBER;
    low = 0;
    high = 0;
    initialized = isIterator ? true : false;
}

Symbol::Symbol(long long memoryAddress, long long lowerIndex, long long upperIndex) {
    address = memoryAddress;
    type = SymbolType::ARRAY;
    low = lowerIndex;
    high = upperIndex;
    initialized = true;
}

long long Symbol::getAddress() {
    return address;
}
long long Symbol::getLow() {
    return low;
}
long long Symbol::getHigh() {
    return high;
}
SymbolType Symbol::getType() {
    return type;
}

bool Symbol::getInitialized() {
    return initialized;
}

void Symbol::setInitialized(bool b) {
        initialized = b;
}