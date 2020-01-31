#pragma once

#include <string>
#include <iostream>

class Log {
    public:
    static void logError(std::string const& errorMessage, int lineNumber);
};