//
// Created by 像我这样优秀的人 on 2020/11/14.
//

#include "Operand.h"

int varAddress = varBase;
int tempAddress = tempVarBase;
int string_nameNum = 0;

void initVarAddress(){
    varAddress = varBase;
}

int getVarAddress(){
    return varAddress;
}