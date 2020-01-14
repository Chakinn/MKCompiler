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
        if(line[0]=='L' && line[1]!='O') {
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
    std::unordered_multimap<std::string, unsigned int> jumpMap;

    unsigned int n = code->size();
    for(unsigned int i = 0; i < n; i++) {
        std::string line = code->at(i);
        char firstLetter = line[0];
        // first letter J - jump command
        if(firstLetter == 'J') {
            unsigned int labelIndex = line.find_first_of('L');
            std::string label = line.substr(labelIndex);
            //if jump target was already found
            try {
                unsigned int jumpTarget = targetMap.at(label);
                code->at(i).replace(labelIndex,labelIndex,std::to_string(jumpTarget));

                unsigned int replaceIndex = code->at(i).find(label);
                code->at(jumpTarget).erase(replaceIndex,label.size()+1);
            }
            // else save label in jump map
            catch(std::out_of_range) {
                jumpMap.insert(std::make_pair(label,i));
            }
        }
        else if(firstLetter == 'L' && line[1]!='O') {
            //get all labels
            std::stringstream sstream;
            sstream << line;
            std::string label;
            std::vector<std::string> labels;
            while(std::getline(sstream, label, ' ')) {
                labels.push_back(label);
            }
            for(auto label = labels.begin(); label!=labels.end(); label++) {
                if(label->at(0) != 'L' || label->at(1) == 'O') {
                    labels.erase(label--);
                }
            }

            
            for(std::string label : labels) {
                //if jump source already found
                auto range = jumpMap.equal_range(label);

                for(auto it = range.first; it != range.second; ++it) {
                    unsigned jumpSource = it->second;
                    unsigned int labelIndex = code->at(jumpSource).find_first_of('L');
                    code->at(jumpSource).replace(labelIndex,labelIndex,std::to_string(i)); 
                }

                if(range.first!=range.second) {
                    unsigned int replaceIndex = code->at(i).find(label);
                    code->at(i).erase(replaceIndex,label.size()+1);
                }
                else {
                    targetMap.insert(std::make_pair(label,i));
                }
            }
            
        }
    }
}