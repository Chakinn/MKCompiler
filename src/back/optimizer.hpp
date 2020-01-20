#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "../handling/label.hpp"



class Optimizer{
    
public:
   Optimizer();
   virtual ~Optimizer();

   void manageLabels(std::vector<std::string>* code);

private:
   void arrangeLabels(std::vector<std::string>* code);
   void subsituteLabelsWithAddresses(std::vector<std::string>* code);
};