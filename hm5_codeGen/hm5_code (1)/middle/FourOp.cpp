//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#include "FourOp.h"
#include "iostream"

using std::cout;
using std::endl;
#include "Operand.h"
/***
 * l_op and r_op both need to be constant
 */
int calculate_directly(fourOp four){
    switch (four.op) {
        case O_PLUS:
            return four.item2.value + four.item3.value;
            break;
        case O_MINU:
            return four.item2.value - four.item3.value;
            break;
        case O_MULT:
            return four.item2.value * four.item3.value;
            break;
        case O_DIV:
            return four.item2.value / four.item3.value;
            break;
        default:
            cout << "fourOp's math_op is wrong!"<< endl;
            return 0;
            break;
    }
}

void print_fourOp(fourOp four);