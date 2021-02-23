//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#include "Expression.h"
extern queue<pair<string,string>> BACKSTRACK;
extern map<string, string> FUNC_TYPE;
void expression::subroutine() {//FIRST = {+/-/IDENFR/"/'/(/数字常量}
    //start gen expression op
    operand expressionOp;
    expressionOp.setTempAddress();

    //prepare [+-]<item> fourOp
    fourOp symbol_fourOp;

    int value = 0;
    //sample-> (((1))*a+b)
    operand fontOp;
    if (TYPE == "MINU") {
        //generate [+-]<item> fourOp's l_op
//        symbol_fourOp.op = O_MULT;
//        operand minuOp;
//        minuOp.kind = K_CONSTANT;
//        minuOp.value = -1;
//        symbol_fourOp.item2 = minuOp;
        symbol_fourOp.op = O_MINU;
        symbol_fourOp.item2.kind = K_CONSTANT;
        symbol_fourOp.item2.value = 0;
        value = 1;
        READ_NEW_WORD
    } else if (TYPE == "PLUS") {
        symbol_fourOp.item2.kind = -1;
        value = 1;
        READ_NEW_WORD
    }

    //调用项
    //generate [+-]<item> fourOp's r_op
    int itemValue;
    item firstItem;
    if (CALL_Item) {
        firstItem.subroutine();
        itemValue = firstItem.valueType;
        if (symbol_fourOp.item2.kind == K_CONSTANT) {
            if (firstItem.op.kind == K_CONSTANT) {
                //calculate directly
                symbol_fourOp.item1.kind = K_CONSTANT;
                symbol_fourOp.item3 = firstItem.op;
                symbol_fourOp.item1.value = calculate_directly(symbol_fourOp);
            } else {
                //generate four Code
                symbol_fourOp.item3 = firstItem.op;
                symbol_fourOp.item1.setTempAddress();
                only_block.fourOpList.push_back(symbol_fourOp);
            }
            fontOp = symbol_fourOp.item1;
        } else {
            fontOp = firstItem.op;
        }
    } else error();


    //handle (+-[item])*next, font*next
    while (TYPE == "PLUS" || TYPE == "MINU") {
        //prepare fourOp's （math_op, l_op)
        fourOp circle_fourOp;
        circle_fourOp.item2 = fontOp;
        if (TYPE == "PLUS") {
            circle_fourOp.op = O_PLUS;
        } else {
            circle_fourOp.op = O_MINU;
        }


        value = 1;
        PRINT_PRE_WORD
        READ_NEW_WORD
        //调用项
        item circleItem;
        if (CALL_Item) {
            circleItem.subroutine();
            itemValue = circleItem.valueType;
        } else error();

        //prepare fourOp's r_op and top_op
        circle_fourOp.item3 = circleItem.op;
        if (circle_fourOp.item2.kind == K_CONSTANT && circle_fourOp.item3.kind == K_CONSTANT) {
            //top_op need to be constant and not generate fourOp
            circle_fourOp.item1.kind = K_CONSTANT;
            circle_fourOp.item1.value = calculate_directly(circle_fourOp);
        } else {
            //need generate fourOp
            circle_fourOp.item1.setTempAddress();
            only_block.fourOpList.push_back(circle_fourOp);
        }
        fontOp = circle_fourOp.item1;
    }

    op = fontOp;

    if (itemValue == -1)valueType = -1;
    else if (value == 0 && itemValue == 0) valueType = 0;
    else valueType = 1;

    op.valueType = valueType;
#ifdef TURN_ON
    file_out << "<表达式>" << endl;
#endif
}

void item::subroutine() {//FIRST={IDENFR/(/+/-/整型常量/'}
    //sample-> (a+b)*5
    //调用因子
    int thisValue = 0;
    int value;
    factor firstFactor;
    if (CALL_Factor) {
        firstFactor.subroutine();
        value = firstFactor.valueType;
    } else error();

    //if item is formed with one factor
    op = firstFactor.op;
    //prepare more factors
    operand fontOp = firstFactor.op;

    //低配版,只有第一个变量之前的常量会被直接运算
    while (TYPE == "MULT" || TYPE == "DIV") {
        //prepare fourOp
        fourOp itemFourOp;
        itemFourOp.item2 = fontOp;
        if (TYPE == "MULT")itemFourOp.op = O_MULT;
        else if (TYPE == "DIV")itemFourOp.op = O_DIV;
        else cout << "wrong type when choose * and / in factor !" << endl;
        //prepare r_op
        thisValue = 1;
        PRINT_PRE_WORD
        READ_NEW_WORD
        //调用因子
        factor circleFactor;
        if (CALL_Factor) {
            circleFactor.subroutine();
            value = circleFactor.valueType;
        } else error();
        itemFourOp.item3 = circleFactor.op;

        //prepare top_op and generate next l_op
        if (itemFourOp.item2.kind == K_CONSTANT && itemFourOp.item3.kind == K_CONSTANT) {
            //calculate directly
            int value = calculate_directly(itemFourOp);
            fontOp.kind = K_CONSTANT;
            fontOp.value = value;
            //set this Item op
            op = fontOp;
        } else {
            itemFourOp.item1.setTempAddress();
            //generate fourOp
            only_block.fourOpList.push_back(itemFourOp);

            fontOp = itemFourOp.item1;
            //set this Item op
            op = fontOp;
        }
    }
    if (thisValue == 0 && value == 0)valueType = 0;
    else if (value == -1) valueType = -1;
    else valueType = 1;

    op.valueType = valueType;
#ifdef TURN_ON
    file_out << "<项>" << endl;
#endif
}

void factor::subroutine() {
    //generate o
    if (TYPE == "IDENFR") {
        symbolLine temp;
        temp.type = -1;
        int dimension = 0;
        int dimension1 = 0;
        int dimension2 = 0;
        bool global;
        if (check_table_symbol(SYM)) {//used identifr
            temp = get_table_symbol(SYM);//expression value
            symbolLine &getTemp = get_table_symbol(SYM);
            //load address
            if (getTemp.kind == K_VARIABLE) {
                variable_line getVariable = dynamic_cast<variable_line &>(getTemp);
                op.address = getVariable.address;
                //判断base
                if(getVariable.global){
                    op.addressBaseType = A_GP;
                }else{
                    op.addressBaseType = A_VAR;
                }
                global = getVariable.global;//只用来进行因子为数组的情况
                op.name = SYM;
                op.kind = K_VARIABLE;
                op.valueType = getVariable.type;
                dimension = getVariable.dimension;
                dimension1 = getVariable.dimension1;
                dimension2 = getVariable.dimension2;
            } else if (getTemp.kind == K_CONSTANT){//constant
                constant_line getConstant = dynamic_cast<constant_line &>(getTemp);
                op.name = SYM;
                op.kind = K_CONSTANT;
                op.value = getConstant.value;
                op.valueType = getConstant.type;
            }else if(getTemp.kind == K_FUNCTION){
            }else cout << "factor: type not exist!" << endl;
        } else {
            cout << "wrong in factor identifr!" << endl;
        }



        //sample-> s1/f1(a,b,c)
        BACKSTRACK.push(PAIR(TYPE, SYM));
        PRE_READ_NEW_WORD
        BACKSTRACK.push(PAIR(TYPE, SYM));
        if (TYPE == "LPARENT") {
            PRE_READ_NEW_WORD_FROM_ALL
            //调用有返回值函数语句
            Call_function_with_return_statement

            //四元式-取返回值结果
            fourOp getReturnValue;
            getReturnValue.op = O_GetReturnValue;
            getReturnValue.notation = "在源代码第"+ to_string(lastSymbolNum)+ "行";
            //第一个操作数,临时空间
            getReturnValue.item1.kind = K_VARIABLE;
            getReturnValue.item1.addressBaseType = A_TEMP;
            getReturnValue.item1.setTempAddress();
            //第二个操作数,在a0上
            //存入
            only_block.fourOpList.push_back(getReturnValue);

            //因子的操作符就为item1
            op = getReturnValue.item1;
        } else if (TYPE == "LBRACK") {
            /***
             * 四元式-数组因子取址
             */
            PRE_READ_NEW_WORD_FROM_ALL
            READ_NEW_WORD
            READ_NEW_WORD
            expression subscript1;
            if (CALL_Expression) {
                subscript1.subroutine();
            } else error();

            //check array subscript
            if (subscript1.valueType == 0) {
                file_error << lastSymbolNum << " i" << endl;
            }
            //check ]
            if (TYPE == "RBRACK") {
                PRINT_PRE_WORD
                PRE_READ_NEW_WORD
            } else file_error << lastSymbolNum << " m" << endl;

            expression subscript2;
            if (TYPE == "LBRACK") {
                PRINT_PRE_WORD
                READ_NEW_WORD

                if (CALL_Expression) {
                    subscript2.subroutine();
                }
                //check array subscript
                if (subscript2.valueType == 0) {
                    file_error << lastSymbolNum << " i" << endl;
                }
                //check ]
                if (TYPE == "RBRACK") {
                    PRINT_PRE_WORD
                    PRE_READ_NEW_WORD
                } else file_error << lastSymbolNum << " m" << endl;
            }

            //获得数组元素地址(直接或非直接)
            op = getArrayOp(op.address, global ,dimension, dimension1, dimension2, subscript1, subscript2);
        } else {
            PRE_READ_NEW_WORD_FROM_ALL
            PRE_READ_NEW_WORD_FROM_ALL
        }

        //expression value
        if (temp.type == T_CHAR) valueType = 0;
        else if (temp.type == T_INT) valueType = 1;
        else valueType = -1;
        return;
    } else if (TYPE == "LPARENT") {
        READ_NEW_WORD
        //调用表达式
        expression subexpression;
        if (CALL_Expression) {
            subexpression.subroutine();
            op = subexpression.op;
        } else {
            error();
        }

        //(a+b*c;
        if (TYPE != "RPARENT")file_error << lastSymbolNum << " l" << endl;
        else {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        }
        valueType = 1;
        return;
    } else if (TYPE == "INTCON" || TYPE == "PLUS" || TYPE == "MINU") {
        //调用整数
        op.kind = K_CONSTANT;
        if (CALL_Constant_integer) {
            op.value = Constant_integer();
        } else {
            error();
        }

        valueType = 1;
        return;
    } else if (TYPE == "CHARCON") {
        op.kind = K_CONSTANT;
        char symbol = SYM[0];
        op.value = symbol;
        PRE_READ_NEW_WORD
        valueType = 0;
        return;
    } else error();
    valueType = 1;
    return;

#ifdef TURN_ON
    file_out << "<因子>" << endl;
#endif
}

operand getArrayOp(int basic_address, bool global, int dimension, int dimension1, int dimension2, expression subscript1,
                   expression subscript2) {
    //现在公开的信息为: basic op.address, dimension,dimension1,dimension2, subscript1, subscript2
    //1.维度1,分一维数组跟二维数组
    operand op;

    if (dimension == 1) {
        op.dimension = 1;
        if (subscript1.op.kind == K_CONSTANT) {//表达式为常数,直接得出结果
            op.kind = K_VARIABLE;
            if(global) op.addressBaseType = A_GP;
            else op.addressBaseType = A_VAR;
            op.address = basic_address + 4*subscript1.op.value; //op的地址是直接地址
        } else if (subscript1.op.kind == K_VARIABLE) {//表达式为变量，需要先load


            //获得以字为单位的偏移量
            fourOp arrayOffset;
            arrayOffset.op = O_MULT;
            arrayOffset.item1.kind = K_VARIABLE;
            arrayOffset.item1.setTempAddress();
            arrayOffset.item2=subscript1.op;
            arrayOffset.item3.kind = K_CONSTANT;
            arrayOffset.item3.value = 4;
            only_block.fourOpList.push_back(arrayOffset);

            //四元式-获得数组元素地址
            fourOp getArrayAddress;
            //1.操作类型
            getArrayAddress.op = O_PLUS;
            //2.顶操作数
            getArrayAddress.item1.kind = K_VARIABLE;
            getArrayAddress.item1.setTempAddress();
            //3.左操作数
            getArrayAddress.item2 = arrayOffset.item1;
            //4.右操作数
            getArrayAddress.item3.kind = K_CONSTANT;
            getArrayAddress.item3.value = basic_address;
            //4.放入
            only_block.fourOpList.push_back(getArrayAddress);
            //此时item1地址存放的就是我们想要的地址

            op = getArrayAddress.item1;
            op.direct_address = false;//非直接寻址
            if(global) op.addressBaseType = A_GP;
            else op.addressBaseType= A_VAR;
        } else cout << "grammer-factor: error";
    } else if (dimension == 2) {
        op.dimension = 2;
        //考虑subscript1
        operand row;
        if (subscript1.op.kind == K_CONSTANT) {
            row.kind = K_CONSTANT;
            row.value = dimension2 * subscript1.op.value;//??
        } else {
            row.kind = K_VARIABLE;
            //四元式-获取横排基地址
            fourOp baseRow;
            //1.操作符
            baseRow.op = O_MULT;
            //2.top操作数
            baseRow.item1.kind = K_VARIABLE;
            baseRow.item1.setTempAddress();
            //3.left操作数
            baseRow.item2 = subscript1.op;
//            if(global){
//                baseRow.item2.addressBaseType = A_GP;
//            }else {
//                baseRow.item2.addressBaseType = A_VAR;
//            }
            //4.right操作数
            baseRow.item3.kind = K_CONSTANT;
            baseRow.item3.value = dimension2;
            //5.放入四元式
            only_block.fourOpList.push_back(baseRow);
            //6.获取表达式运算结果
            row = baseRow.item1;
        }
        //考虑subscript2
        operand column;
        if (subscript2.op.kind == K_CONSTANT) {
            column.kind = K_CONSTANT;
            column.value = subscript2.op.value;
        } else {
            column = subscript2.op;
        }

        //考虑二者组合
        if (row.kind == K_CONSTANT && column.kind == K_CONSTANT) {
            op.kind = K_VARIABLE;
            op.address = basic_address + 4*(row.value + column.value);//真实地址
            if(global) op.addressBaseType = A_GP;
            else op.addressBaseType = A_VAR;
        } else {
            //四元式-有未知量，获得偏移
            fourOp arrayUnitOffset;
            arrayUnitOffset.op = O_PLUS;
            arrayUnitOffset.item1.kind = K_VARIABLE;
            arrayUnitOffset.item1.setTempAddress();
            arrayUnitOffset.item2 = row;
            arrayUnitOffset.item3 = column;
            only_block.fourOpList.push_back(arrayUnitOffset);
            
            //四元式-还要乘以4才是偏移量
            fourOp arrayOffset;
            arrayOffset.op = O_MULT;
            arrayOffset.item1.kind = K_VARIABLE;
            arrayOffset.item1.setTempAddress();
            arrayOffset.item2 = arrayUnitOffset.item1;
            arrayOffset.item3.kind = K_CONSTANT;
            arrayOffset.item3.value = 4;
            only_block.fourOpList.push_back(arrayOffset);

            //获得数组元素地址
            fourOp arrayAddress;
            arrayAddress.op = O_PLUS;
            arrayAddress.item1.kind = K_VARIABLE;
            arrayAddress.item1.setTempAddress();//存放地址的地址
            arrayAddress.item2 = arrayOffset.item1;
            arrayAddress.item3.kind = K_CONSTANT;
            arrayAddress.item3.value = basic_address;
            only_block.fourOpList.push_back(arrayAddress);

            op = arrayAddress.item1;//存放地址的地址
            op.direct_address = false;//告诉生成代码要做取地址以及取元素的操作
            if(global) op.addressBaseType = A_GP;
            else op.addressBaseType = A_VAR;
        }
    } else cout << "grammer-factor: wrong dimension" << endl;
    return  op;
}
