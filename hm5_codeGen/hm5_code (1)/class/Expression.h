//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#ifndef HM5_CODE_EXPRESSION_H
#define HM5_CODE_EXPRESSION_H

#include "string"
#include "../middle/Operand.h"
#include "../include/Error.h"
#include "../include/SymbolTable.h"
#include "../include/grammer_analysis.h"
#include "../middle/MiddleCode.h"


using std::string;

extern string TYPE;
extern string SYM;
extern int symbolNum;
extern int lastSymbolNum;
extern middleCode final_middleCode;
extern basicBlock only_block;

class expression;
class item;
class factor;

class expression {
public:
    int valueType;
    operand op;

    string notation;

    void subroutine();
};

class item {
public:
    int valueType;
    operand op;

    string notation;

    void subroutine();
};

class factor {
public:
    int valueType;
    operand op;

    string notation;

    void subroutine();
};

operand getArrayOp(int basic_address, bool global, int dimension, int dimension1, int dimension2, expression subscript1, expression subscript2);

#endif //HM5_CODE_EXPRESSION_H
