#include "writer.hpp"

std::string Writer::output = "";
void Writer::setOutput(std::string out) {
    output = out;
}
void Writer::writeToFile(std::vector<std::string>* code) {
    std::ofstream out;
    out.open(output);

    for(std::string line : *code) {
        out << line << "\n";
    }
}