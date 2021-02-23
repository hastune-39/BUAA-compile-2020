//
// Created by 像我这样优秀的人 on 2020/11/28.
//

#ifndef HM5_CODE_STATEMENT_H
#define HM5_CODE_STATEMENT_H
#include "iostream"
#include "../middle/FourOp.h"
using std::string;
class condition{
public:
    string label;
    fourOp branch_op;

    void subroutine();
};

class return_statement{
public:
    bool if_has_return;
    operand return_value;

    void subroutine();
};

#endif //HM5_CODE_STATEMENT_H
