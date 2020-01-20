#pragma once

#include <string>
class Label {
    static int labelCounter;
    public:
    static std::string lastLabel();
    static bool isLabel(std::string label);
    static std::string newLabel();
};