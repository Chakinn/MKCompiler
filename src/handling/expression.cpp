#include "expression.hpp"
#include <iostream>

Expression::Expression(SymbolTable* symbolTab, std::string leftValue, std::string operatorr, std::string rightValue, unsigned int position) : Node(symbolTab, position) {
    lvalue = leftValue;
    rvalue = rightValue;

    if (operatorr == "") {
        op = EOP::NONE;
    }
    else if (operatorr == "PLUS") {
        op = EOP::PLUS;
    }
    else if (operatorr == "MINUS") {
        op = EOP::MINUS;
    }
    else if (operatorr == "TIMES") {
        op = EOP::TIMES;
    }
    else if (operatorr == "DIV") {
        op = EOP::DIV;
    }
    else if (operatorr == "MOD") {
        op = EOP::MOD;   
    }
};

std::vector<std::string> Expression::getCode() {
    validateIdentifier(lvalue, true);
    validateIdentifier(rvalue, true);

    std::vector<std::string> code;

    long long leftAddress = symbolTable->getAddress(lvalue);
    long long leftPointerAddress;
    bool leftIsTemp = false;

    if(leftAddress == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(lvalue);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        leftPointerAddress = symbolTable->getFreeAddress();
        code.push_back("STORE "+std::to_string(leftPointerAddress));
        
        //make temp left variable
        long long helpAddress = symbolTable->getFreeAddress();
        code.push_back("LOADI " + std::to_string(leftPointerAddress));
        code.push_back("STORE " + std::to_string(helpAddress));
        leftAddress = helpAddress;
        leftIsTemp = true;
    }
    
    if (op == EOP::NONE) { 
        code.push_back("LOAD " + std::to_string(leftAddress));
        return code;
    }

    long long rightAddress = symbolTable->getAddress(rvalue);
    long long rightPointerAddress;
    bool rightIsTemp = false;

    if(rightAddress == -1) {
        std::vector<std::string> calculateAddressCode = symbolTable->calculateAddress(rvalue);
        code.insert(code.end(),calculateAddressCode.begin(),calculateAddressCode.end());
        rightPointerAddress = symbolTable->getFreeAddress();
        code.push_back("STORE "+std::to_string(rightPointerAddress));
        
        //make temp right variable
        long long helpAddress = symbolTable->getFreeAddress();
        code.push_back("LOADI " + std::to_string(rightPointerAddress));
        code.push_back("STORE " + std::to_string(helpAddress));
        rightAddress = helpAddress;
        rightIsTemp = true;
    }

    switch(op) {
        case EOP::PLUS: {
            code.push_back("LOAD " + std::to_string(leftAddress));
            code.push_back("ADD " + std::to_string(rightAddress));
            break;
        }
        case EOP::MINUS: {
            code.push_back("LOAD " + std::to_string(leftAddress));
            code.push_back("SUB " + std::to_string(rightAddress));
            break;
        }
        case EOP::TIMES: {
            bool leftIsNumber = symbolTable->isNumber(lvalue);
            bool rightIsNumber = symbolTable->isNumber(rvalue);
            //both factors are numbers - precompile
            if(leftIsNumber && rightIsNumber) {
                code.push_back("LOAD " + std::to_string(symbolTable->generateNumber(std::stoll(lvalue) * std::stoll(rvalue))));
            }
            //left is number
            else if(leftIsNumber) {
                multiplyNumberCode(code, rightAddress, std::stoll(lvalue));
            }
            //right is number
            else if(rightIsNumber) {
                multiplyNumberCode(code, leftAddress, std::stoll(rvalue));
            }
            //both are identifiers - generate mul algorithm
            else {
                //create temp variables that can be modified
                if(!leftIsTemp) {
                    long long helpAddress = symbolTable->getFreeAddress();
                    code.push_back("LOAD " + std::to_string(leftAddress));
                    code.push_back("STORE " + std::to_string(helpAddress));
                    leftAddress = helpAddress;
                }
                if(!rightIsTemp) {
                    long long helpAddress = symbolTable->getFreeAddress();
                    code.push_back("LOAD " + std::to_string(rightAddress));
                    code.push_back("STORE " + std::to_string(helpAddress));
                    rightAddress = helpAddress;
                }
                multiplyVarCode(code,leftAddress,rightAddress);
            }
            break;
        }
        case EOP::DIV: {
            bool leftIsNumber = symbolTable->isNumber(lvalue);
            bool rightIsNumber = symbolTable->isNumber(rvalue);
            //both factors are numbers - precompile
            if(leftIsNumber && rightIsNumber) {
                code.push_back("LOAD " + std::to_string(symbolTable->generateNumber(std::stoll(lvalue) / std::stoll(rvalue))));
            }
            //right is number
            else if(rightIsNumber && powerOfTwo(std::stoll(rvalue))!=-1) {
                divisionNumberCode(code, leftAddress, std::stoll(rvalue));
            }
            //both are identifiers - generate mul algorithm
            else {
                //create temp variables that can be modified
                if(!leftIsTemp) {
                    long long helpAddress = symbolTable->getFreeAddress();
                    code.push_back("LOAD " + std::to_string(leftAddress));
                    code.push_back("STORE " + std::to_string(helpAddress));
                    leftAddress = helpAddress;
                }
                if(!rightIsTemp) {
                    long long helpAddress = symbolTable->getFreeAddress();
                    code.push_back("LOAD " + std::to_string(rightAddress));
                    code.push_back("STORE " + std::to_string(helpAddress));
                    rightAddress = helpAddress;
                }
                divisionCode(code,leftAddress,rightAddress, "DIV");
            }            
            break;
        }
        case EOP::MOD: {
            bool leftIsNumber = symbolTable->isNumber(lvalue);
            bool rightIsNumber = symbolTable->isNumber(rvalue);
            //both factors are numbers - precompile
            if(leftIsNumber && rightIsNumber) {
                code.push_back("LOAD " + std::to_string(symbolTable->generateNumber(std::stoll(lvalue) % std::stoll(rvalue))));
            }
            //right is number
            else if(rightIsNumber && powerOfTwo(std::stoll(rvalue))!=-1) {
                modNumberCode(code, leftAddress, std::stoll(rvalue));
            }
            //both are identifiers - generate mul algorithm
            else {
                //create temp variables that can be modified
                if(!leftIsTemp) {
                    long long helpAddress = symbolTable->getFreeAddress();
                    code.push_back("LOAD " + std::to_string(leftAddress));
                    code.push_back("STORE " + std::to_string(helpAddress));
                    leftAddress = helpAddress;
                }
                if(!rightIsTemp) {
                    long long helpAddress = symbolTable->getFreeAddress();
                    code.push_back("LOAD " + std::to_string(rightAddress));
                    code.push_back("STORE " + std::to_string(helpAddress));
                    rightAddress = helpAddress;
                }
                divisionCode(code,leftAddress,rightAddress, "MOD");
            }      
            break;
        }
    }
    return code;
}

void Expression::multiplyNumberCode(std::vector<std::string>&  code, long long varAddress, long long number) {
    int power = powerOfTwo(number);
    //number is a power of two - shifts only
    if(power != -1) {
        long long powerAddress = symbolTable->generateNumber(power);
        code.push_back("LOAD " + std::to_string(varAddress));
        code.push_back("SHIFT " + std::to_string(powerAddress));
        return;
    }
    //else split into 2^i factors
    else {
        long long resultAddress = symbolTable->getFreeAddress();
        code.push_back("LOAD " + std::to_string(varAddress));

        std::vector<int> digits = binaryRepresentation(number);
        int n = digits.size();
        int shift;
        for(int i = 0; i < n - 1; i++) {
            if(digits[i] == 1) {
                code.push_back("ADD " + std::to_string(resultAddress));
                code.push_back("STORE " + std::to_string(resultAddress));
                // look ahead to check how big of a shift to make
                shift = 1;
                for(int j = i; j < n; j++) {
                    if(digits[j] == 0) {
                        shift++;
                    }
                }
                code.push_back("SHIFT " + std::to_string(symbolTable->generateNumber(shift)));
            }
        }
        if(digits[n-1] == 1) {
            code.push_back("ADD " + std::to_string(resultAddress));
        }
        return;
    }
}

void Expression::multiplyVarCode(std::vector<std::string>& code, long long leftAddress, long long rightAddress) {
    long long tempAddress = symbolTable->getFreeAddress();
    long long signAddress = symbolTable->getFreeAddress();
    long long ONE = symbolTable->generateNumber(1); //address of 1
    long long mONE = symbolTable->generateNumber(-1);   //address of -1

    std::string endForLabel = Label::newLabel();
    std::string startLabel = Label::newLabel();
    std::string condLabel = Label::newLabel();
    std::string evenLabel = Label::newLabel();
    std::string negLabel = Label::newLabel();
    std::string negResLabel = Label::newLabel();
    std::string endLabel = Label::newLabel();
    

    code.push_back("SUB 0");
    code.push_back("STORE " + std::to_string(tempAddress));

    //load multiplier
    code.push_back("LOAD " + std::to_string(leftAddress));
    code.push_back("STORE " + std::to_string(signAddress));
    code.push_back("JNEG " + negLabel);
    code.push_back("JUMP " + condLabel);

    code.push_back(negLabel);
    code.push_back("SUB " + std::to_string(leftAddress));
    code.push_back("SUB " + std::to_string(leftAddress));
    code.push_back("STORE " + std::to_string(leftAddress));

    //conditions
    code.push_back(condLabel);
    code.push_back("JZERO " + endForLabel);

    // save value
    code.push_back("STORE " + std::to_string(leftAddress));
    //check parity
    code.push_back("SHIFT " + std::to_string(mONE));
    code.push_back("SHIFT " + std::to_string(ONE));
    code.push_back("SUB " + std::to_string(leftAddress));
    code.push_back("JZERO " + evenLabel);    //jump if even

    //if not even sum right
    code.push_back("LOAD " + std::to_string(rightAddress));
    code.push_back("ADD " + std::to_string(tempAddress));
    code.push_back("STORE " + std::to_string(tempAddress));

    //shifts
    code.push_back(evenLabel);
    code.push_back("LOAD " + std::to_string(rightAddress));
    code.push_back("SHIFT " + std::to_string(ONE));
    code.push_back("STORE " + std::to_string(rightAddress));
    code.push_back("LOAD " + std::to_string(leftAddress));
    code.push_back("SHIFT " + std::to_string(mONE));

    //jump start
    code.push_back("JUMP " + condLabel);

    //end
    code.push_back(endForLabel);

    code.push_back("LOAD " + std::to_string(signAddress));
    code.push_back("JNEG " + negResLabel);
    code.push_back("LOAD " + std::to_string(tempAddress));
    code.push_back("JUMP " + endLabel);

    code.push_back(negResLabel);
    code.push_back("LOAD " + std::to_string(tempAddress));
    code.push_back("SUB " + std::to_string(tempAddress));
    code.push_back("SUB " + std::to_string(tempAddress));

    code.push_back(endLabel);

    
}

void Expression::divisionNumberCode(std::vector<std::string>& code, long long varAddress, long long number) {
    int power = powerOfTwo(number);
    long long powerAddress = symbolTable->generateNumber(-power);
    code.push_back("LOAD " + std::to_string(varAddress));
    code.push_back("SHIFT " + std::to_string(powerAddress));
    return;
}

void Expression::divisionCode(std::vector<std::string>& code, long long leftAddress, long long rightAddress, std::string mode) {
    long long quotientAddress = symbolTable->getFreeAddress();
    long long remainderAddress = symbolTable->getFreeAddress();
    long long iteratorAddress = symbolTable->getFreeAddress();
    long long negativeIteratorAddress = symbolTable->getFreeAddress();

    long long dividendSignAddress = symbolTable->getFreeAddress();
    long long divisorSignAddress = symbolTable->getFreeAddress();

    long long tempAddress = symbolTable->getFreeAddress();

    std::string iteratorLabel = Label::newLabel();
    std::string preparationLabel = Label::newLabel();
    std::string divisorSignLabel = Label::newLabel();
    std::string dividendLabel = Label::newLabel();
    std::string dividendSignLabel = Label::newLabel();
    std::string forLabel = Label::newLabel();
    std::string bitLabel = Label::newLabel();
    std::string remainderLabel = Label::newLabel();
    std::string ifLabel = Label::newLabel();
    std::string quotientLabel = Label::newLabel();
    std::string endForLabel = Label::newLabel();
    std::string positiveSignLabel = Label::newLabel();
    std::string endLabel = Label::newLabel();


    long long ONE = symbolTable->generateNumber(1);
    long long MONE = symbolTable->generateNumber(-1);

    //set quotient and remainder
    code.push_back("SUB 0");
    code.push_back("STORE " + std::to_string(quotientAddress));
    code.push_back("STORE " + std::to_string(remainderAddress));
    code.push_back("STORE " + std::to_string(iteratorAddress));
    // 0/x = 0, 0%x = 0
    code.push_back("LOAD " + std::to_string(leftAddress));
    code.push_back("JZERO " + endLabel);
    //division by 0 gives 0
    code.push_back("LOAD " + std::to_string(rightAddress));
    code.push_back("JZERO " + endLabel);
    //check signs
    code.push_back("JNEG " + divisorSignLabel);
    //set divisiorSign = 0 (positive)
    code.push_back("SUB 0");
    code.push_back("STORE " + std::to_string(divisorSignAddress));
    code.push_back("JUMP " + dividendLabel);

    //set divisor = abs(divisor) and divisorSign = 1 (negative)
    code.push_back(divisorSignLabel);
    code.push_back("SUB " + std::to_string(rightAddress));
    code.push_back("SUB " + std::to_string(rightAddress));
    code.push_back("STORE " + std::to_string(rightAddress));
    code.push_back("LOAD " + std::to_string(ONE));
    code.push_back("STORE " + std::to_string(divisorSignAddress));

    code.push_back(dividendLabel);
    code.push_back("LOAD " + std::to_string(leftAddress));
    code.push_back("STORE " + std::to_string(tempAddress)); //save left to temp for optimisation purpose
    code.push_back("JNEG " + dividendSignLabel);

    //set dividendSign = 0 (positive)
    code.push_back("SUB 0");
    code.push_back("STORE " + std::to_string(dividendSignAddress));
    code.push_back("JUMP " + iteratorLabel);

    //set dividend = abs(dividend) and dividendSign = 1 (negative)
    code.push_back(dividendSignLabel);
    code.push_back("SUB " + std::to_string(leftAddress));
    code.push_back("SUB " + std::to_string(leftAddress));
    code.push_back("STORE " + std::to_string(leftAddress));
    code.push_back("STORE " + std::to_string(tempAddress));
    code.push_back("LOAD " + std::to_string(ONE));
    code.push_back("STORE " + std::to_string(dividendSignAddress));

    //calculate number of operations
    code.push_back(iteratorLabel);
    code.push_back("LOAD " + std::to_string(iteratorAddress));
    code.push_back("INC");
    code.push_back("STORE " + std::to_string(iteratorAddress));
    code.push_back("LOAD " + std::to_string(tempAddress));
    code.push_back("SHIFT " + std::to_string(MONE));
    code.push_back("JZERO " + preparationLabel);
    code.push_back("STORE " + std::to_string(tempAddress));
    code.push_back("JUMP " + iteratorLabel);

    //prepare negative iterator for shifts
    code.push_back(preparationLabel);
    code.push_back("LOAD " + std::to_string(iteratorAddress));
    code.push_back("SUB " + std::to_string(iteratorAddress));
    code.push_back("SUB " + std::to_string(iteratorAddress));
    code.push_back("STORE " + std::to_string(negativeIteratorAddress));

    //for every bit in dividend
    code.push_back(forLabel);
    code.push_back("LOAD " + std::to_string(iteratorAddress));
    code.push_back("DEC");
    code.push_back("JNEG " + endForLabel);
    code.push_back("STORE " + std::to_string(iteratorAddress));
    code.push_back("LOAD " + std::to_string(negativeIteratorAddress));
    code.push_back("INC");
    code.push_back("STORE " + std::to_string(negativeIteratorAddress));
    

    //shift quotient left
    code.push_back("LOAD " + std::to_string(remainderAddress));
    code.push_back("SHIFT " + std::to_string(ONE));
    code.push_back("STORE " + std::to_string(remainderAddress));
    
    //calcualte i-th bit of dividend
    code.push_back("LOAD " + std::to_string(leftAddress));
    code.push_back("SHIFT " + std::to_string(negativeIteratorAddress));
    code.push_back("STORE " +std::to_string(tempAddress));
    code.push_back("SHIFT " + std::to_string(MONE));
    code.push_back("SHIFT " + std::to_string(ONE));
    code.push_back("SUB " + std::to_string(tempAddress));
    
    code.push_back("JZERO " + bitLabel);    //ifN(i) even

    //if dividend i-th bit was 1
    code.push_back("LOAD " + std::to_string(remainderAddress));
    code.push_back("SHIFT " + std::to_string(MONE));
    code.push_back("SHIFT " + std::to_string(ONE));
    code.push_back("SUB " + std::to_string(remainderAddress));
    code.push_back("JZERO " + remainderLabel);
    code.push_back("JUMP " + ifLabel);

    //if dividend i-th bit was 0
    code.push_back(bitLabel);
    code.push_back("LOAD " + std::to_string(remainderAddress));
    code.push_back("SHIFT " + std::to_string(MONE));
    code.push_back("SHIFT " + std::to_string(ONE));
    code.push_back("SUB " + std::to_string(remainderAddress));
    code.push_back("JZERO " + ifLabel);

    //sub 1 from remainder
    code.push_back("LOAD " + std::to_string(remainderAddress));
    code.push_back("DEC");
    code.push_back("STORE " + std::to_string(remainderAddress));
    code.push_back("JUMP " + ifLabel);

    //add 1 to remainder
    code.push_back(remainderLabel);
    code.push_back("LOAD " + std::to_string(remainderAddress));
    code.push_back("INC");
    code.push_back("STORE " + std::to_string(remainderAddress));

    //if remainder >= divisor
    code.push_back(ifLabel);
    code.push_back("LOAD " + std::to_string(remainderAddress));
    
    code.push_back("SUB " + std::to_string(rightAddress));
    code.push_back("JNEG " + forLabel);
    code.push_back("STORE " + std::to_string(remainderAddress));

    //Q(i) = 1
    code.push_back("LOAD " + std::to_string(quotientAddress));
    code.push_back("SHIFT " + std::to_string(negativeIteratorAddress));
    code.push_back("STORE " + std::to_string(tempAddress));
    code.push_back("SHIFT " + std::to_string(MONE));
    code.push_back("SHIFT " + std::to_string(ONE));
    code.push_back("SUB " + std::to_string(tempAddress));
    code.push_back("JZERO " + quotientLabel);

    //if quotient i-th bit was 1 - sub
    code.push_back("LOAD " + std::to_string(ONE));
    code.push_back("SHIFT " + std::to_string(iteratorAddress));
    code.push_back("STORE " + std::to_string(tempAddress));
    code.push_back("LOAD " + std::to_string(quotientAddress));
    code.push_back("SUB " + std::to_string(tempAddress));
    code.push_back("STORE " + std::to_string(quotientAddress));

    code.push_back("JUMP " + forLabel);
    
    //if quotient i-th bit was 0 - add
    code.push_back(quotientLabel);
    code.push_back("LOAD " + std::to_string(ONE));
    code.push_back("SHIFT " + std::to_string(iteratorAddress));
    code.push_back("STORE " + std::to_string(tempAddress));
    code.push_back("LOAD " + std::to_string(quotientAddress));
    code.push_back("ADD " + std::to_string(tempAddress));
    code.push_back("STORE " + std::to_string(quotientAddress));

    code.push_back("JUMP " + forLabel);
    
    code.push_back(endForLabel);
    //check if signs are equal
    code.push_back("LOAD " + std::to_string(divisorSignAddress));
    code.push_back("SUB " + std::to_string(dividendSignAddress));
    code.push_back("JZERO " + positiveSignLabel);   //if equal - positive result

    if(mode == "DIV") {
        std::string remainderZeroLabel = Label::newLabel();
        
        //if not - negative result
        //check remainder
        code.push_back("LOAD " + std::to_string(remainderAddress));
        code.push_back("JZERO " + remainderZeroLabel);

        //if remainder != 0 flip quotient and sub 1
        code.push_back("LOAD " + std::to_string(quotientAddress));
        code.push_back("SUB " + std::to_string(quotientAddress));
        code.push_back("SUB " + std::to_string(quotientAddress));
        code.push_back("DEC");
        code.push_back("JUMP " + endLabel);

        //if remainder was 0 flip quotient
        code.push_back(remainderZeroLabel);
        code.push_back("LOAD " + std::to_string(quotientAddress));
        code.push_back("SUB " + std::to_string(quotientAddress));
        code.push_back("SUB " + std::to_string(quotientAddress));
        code.push_back("JUMP " + endLabel);

        //positive result
        code.push_back(positiveSignLabel);
        code.push_back("LOAD " + std::to_string(quotientAddress));
    }
    else if(mode == "MOD") {
        std::string positiveDivisorLabel = Label::newLabel();

        //if negative quotient - flip remainder and add divisor
        code.push_back("LOAD " + std::to_string(remainderAddress));
        code.push_back("SUB " + std::to_string(remainderAddress));
        code.push_back("SUB " + std::to_string(remainderAddress));
        code.push_back("ADD " + std::to_string(rightAddress));
        code.push_back("STORE " + std::to_string(remainderAddress));

        //if positive quotient
        code.push_back(positiveSignLabel);
        code.push_back("LOAD " + std::to_string(divisorSignAddress));
        code.push_back("JZERO " + positiveDivisorLabel);

        //if negative - negative result
        code.push_back("LOAD " + std::to_string(remainderAddress));
        code.push_back("SUB " + std::to_string(remainderAddress));
        code.push_back("SUB " + std::to_string(remainderAddress));
        code.push_back("JUMP " + endLabel);

        code.push_back(positiveDivisorLabel);
        code.push_back("LOAD " + std::to_string(remainderAddress));
    }
    code.push_back(endLabel);
}

void Expression::modNumberCode(std::vector<std::string>& code, long long varAddress, long long number) {
    int power = powerOfTwo(number);
    long long tempAddress = symbolTable->getFreeAddress();

    code.push_back("LOAD " + std::to_string(varAddress));

    if (power == 0) {
        return;
    }
    
    long long powerAddress = symbolTable->generateNumber(power);
    long long minusPowerAddress = symbolTable->generateNumber(-power);

    
    code.push_back("SHIFT " + std::to_string(minusPowerAddress));
    code.push_back("SHIFT " + std::to_string(powerAddress));
    code.push_back("STORE " + std::to_string(tempAddress));
    code.push_back("LOAD " + std::to_string(varAddress));
    code.push_back("SUB " + std::to_string(tempAddress));

    
    return;
}


int Expression::powerOfTwo(long long number) {
        if(number == 0) {
            return -1;
        }
        int counter =0;
        while(number){
            if(number == 1) {
                return counter;
            }
            if(number & 1){
                return -1;
            }
            number >>= 1;
            counter++;
        }
        return counter;
}

std::vector<int> Expression::binaryRepresentation(long long number) {
    std::vector<int> digits;
    while(number > 0) {
        if(number & 1) {
            digits.push_back(1);
        }
        else {
            digits.push_back(0);
        }
        number >>= 1;
    }
    //std::reverse(digits.begin(),digits.end());
    return digits;
}

std::string Expression::getLvalue() {
    return lvalue;
}
std::string Expression::getRvalue() {
    return rvalue;
}
EOP Expression::getOp() {
    return op;
}