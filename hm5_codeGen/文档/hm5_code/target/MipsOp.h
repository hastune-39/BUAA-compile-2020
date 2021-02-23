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
class mips_op{
public:
    string output;
    void getOp1(fourOp four);
    void getOp2(fourOp four);
    void getOp3(fourOp four);
};

#endif //HM5_CODE_MIPSOP_H
