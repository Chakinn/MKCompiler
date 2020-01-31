#include "log.hpp"

void Log::logError(std::string const& errorMessage, int lineNumber) {
    std::cout << "Error on line: " << lineNumber << ". " << errorMessage << "\n";
    exit(1);
}