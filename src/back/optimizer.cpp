#include "optimizer.hpp"

Optimizer::Optimizer(){}
Optimizer::~Optimizer(){}

void Optimizer::manageLabels(std::vector<std::string>* code) {
    arrangeLabels(code);
    subsituteLabelsWithAddresses(code);

}

void Optimizer::arrangeLabels(std::vector<std::string>* code) {
    unsigned int n = code->size();
    std::vector<std::string> currentLabels;
    std::vector<unsigned int> linesToDelete;
    for(unsigned int i = 0; i < n; i++) {
        std::string line = code->at(i);
        if(Label::isLabel(line)) {
            currentLabels.push_back(line);
            linesToDelete.push_back(i);
        }
        else {
            for(std::string label : currentLabels) {
                code->at(i) = label + " " + code->at(i);
            }
            currentLabels.clear();
        }
    }
    for(auto l=linesToDelete.end()-1; l!= linesToDelete.begin()-1; l--) {
        code->erase(code->begin()+*l);
    }
}

void Optimizer::subsituteLabelsWithAddresses(std::vector<std::string>* code) {
    std::unordered_map<std::string, unsigned int> targetMap;

    //find all jump targets
    unsigned int n = code->size();
    for(unsigned int i = 0; i < n; i++) {
        std::string line = code->at(i);
        char firstLetter = line[0];
        // list of labels
        if(firstLetter == 'L' && line[1]!='O') {
            //get all labels
            std::stringstream sstream;
            sstream << line;
            std::string label;
            std::vector<std::string> labels;
            while(std::getline(sstream, label, ' ')) {
                if(Label::isLabel(label)) {
                    labels.push_back(label);
                }
                else {
                    break;
                }
            }
            for(std::string label : labels) {
                targetMap.insert(std::make_pair(label,i));
                unsigned int replaceIndex = code->at(i).find(label);
                code->at(i).erase(replaceIndex,label.size()+1);
            }  
        }
    }
    //substitute labels in jump commands with target addresses 
    for(unsigned int i = 0; i < n; i++) {
        std::string line = code->at(i);
        char firstLetter = line[0];
        if(firstLetter == 'J') {
            unsigned int labelIndex = line.find_first_of('L');
            std::string label = line.substr(labelIndex);
            try {
                unsigned int jumpTarget = targetMap.at(label);
                code->at(i).replace(labelIndex,labelIndex,std::to_string(jumpTarget));
            }
            catch(std::out_of_range) {
                std::cout<<"jump target out of range";
            }
        }
    }
}