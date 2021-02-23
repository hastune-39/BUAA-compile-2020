//
// Created by 像我这样优秀的人 on 2020/12/18.
//

#ifndef HM5_CODE_MIPSOP_H
#define HM5_CODE_MIPSOP_H
#include "string"
#include "../middle/FourOp.h"
#include "./GenMips.h"
#include "iostream"
using std::endl;
using std::string;
enum {
    optype_none, optype_cons,optype_reg
};
class mips_op{
public:
    string output;
    int type;
    int value;
    void getOp1(fourOp four);
    void getOp2(fourOp four);
    void getOp3(fourOp four);
    void genTempOp1(fourOp four);
};

#endif //HM5_CODE_MIPSOP_H
