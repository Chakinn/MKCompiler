#include "symbol.hpp"

Symbol::Symbol(long long memoryAddress) {
    address = memoryAddress;
    type = SymbolType::NUMBER;
}

Symbol::Symbol(long long memoryAddress, long long lowerIndex, long long upperIndex) {
    address = memoryAddress;
    type = SymbolType::ARRAY;
    low = lowerIndex;
    high = upperIndex;
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