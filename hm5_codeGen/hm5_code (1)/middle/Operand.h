//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#ifndef HM5_CODE_OPERAND_H
#define HM5_CODE_OPERAND_H

#include "iostream"
#include <string>
#include "../include/AddressSpace.h"
#include "../include/Enum.h"
#include "set"

using std::set;
using std::string;
using std::pair;

extern int varAddress;
extern int tempAddress;
extern int string_nameNum;

class operand {
public:
    string name;
    int nameNum;
    int kind = -1;
    int value;
    int dimension = 0;

    int size = 0;
    int valueType;
    string stringValue;

    //if,else,for,while
    string label;
    bool isFunctionLabel = false;
    //若地址无法直接得到
    bool direct_address = true;
    bool undirect_Base = A_TEMP;
    int address;
    int addressBaseType = A_TEMP;

    //寄存器分配部分
    bool use_register = false;
    int reg_type = no_reg;
    int reg = -1;
    set<pair<int,int>> protectReg;
    bool shouldSave = false;

    //return
    bool addSpace;
    int registerType;



    int getValue() {
        return value;
    }

    int getAddress() {
        return address;
    }

    void setVarAddress() {
        address = varAddress;
        kind = K_VARIABLE;
        varAddress = varAddress + 4;
    }

    void setTempAddress() {
        address = tempAddress;
        kind = K_VARIABLE;
        tempAddress = tempAddress + 4;
        addressBaseType = A_TEMP;
    }

    void setArrayAddress(int size){
        address = varAddress;
        kind = K_VARIABLE;
        varAddress += size;
    }

    void set_stringOp(string tempString) {
        name = "str";
        nameNum = string_nameNum;
        string_nameNum = string_nameNum + 1;
        stringValue = tempString;
        kind = K_STRING;
    }

};

void initVarAddress();
int getVarAddress();

#endif //HM5_CODE_OPERAND_H
