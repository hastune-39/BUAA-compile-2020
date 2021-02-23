//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#ifndef HM5_CODE_FOUROP_H
#define HM5_CODE_FOUROP_H
#include "string"
#include "Operand.h"
#include "../include/Enum.h"
#include <set>
using std::string;
using std::set;
using std::pair;


static int temp_subscript = 0;
class fourOp{
public:
    int op;
    operand item1;
    operand item2;
    operand item3;

    string notation;
    /***
     * 四元式活跃变量分析需要
     */
    set<pair<int,int>> live;


    void setTempTop_op(){
        item1.name="temp";
        item1.setTempAddress();
        item1.kind=K_VARIABLE;
        // no value,type
    }
};

void initial_temp();
int calculate_directly(fourOp four);//when l_op and r_op both constant
void print_fourOp(fourOp four);

#endif //HM5_CODE_FOUROP_H
