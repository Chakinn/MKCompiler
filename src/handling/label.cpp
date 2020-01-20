#include "label.hpp"

int Label::labelCounter = 0;
std::string Label::newLabel() {
    labelCounter++;
    return "L"+std::to_string(labelCounter);
}

bool Label::isLabel(std::string label) {
    return (label[0] == 'L' && label[1] != 'O');
}

std::string Label::lastLabel() {
    return "L"+std::to_string(labelCounter);
}