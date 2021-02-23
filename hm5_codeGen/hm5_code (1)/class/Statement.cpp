//
// Created by 像我这样优秀的人 on 2020/11/28.
//
#include "Statement.h"
#include "Expression.h"
#include "string"
using std::string;
extern string TYPE;
extern string SYM;
extern middleCode final_middleCode;
extern basicBlock only_block;
extern int branchNum;
extern int lastSymbolNum;
extern queue<pair<string,string>> BACKSTRACK;
extern map<string, string> FUNC_TYPE;
void condition::subroutine() {
    //跳转四元式
    fourOp branch;

    //sample (a+b)==c+d
    //调用表达式
    expression subexpression;
    if (CALL_Expression) {
        subexpression.subroutine();
        expressionValue = subexpression.valueType;
    } else {
        error();
    }

    //跳转四元式——第一个操作数
    branch.item1 = subexpression.op;

    int value1 = expressionValue;
    if (TYPE == "LSS" || TYPE == "LEQ" || TYPE == "GRE" || TYPE == "GEQ" || TYPE == "NEQ" ||TYPE == "EQL") {
        PRINT_PRE_WORD
        //跳转四元式——操作类型
        if(TYPE == "LSS"){
            branch.op = O_bge;
        }else if(TYPE == "LEQ"){
            branch.op = O_bgt;
        }else if(TYPE == "GRE"){
            branch.op = O_ble;
        }else if(TYPE == "GEQ"){
            branch.op = O_blt;
        }else if(TYPE == "NEQ"){
            branch.op = O_beq;
        }else if(TYPE == "EQL"){
            branch.op = O_bne;
        } else cout<<"error in: grammer->condition"<<endl;
    } else error();

    READ_NEW_WORD
    //调用表达式
    expression subexpression2;
    if (CALL_Expression) {
        subexpression2.subroutine();
        expressionValue = subexpression2.valueType;
    } else {
        error();
    }//调用表达式

    //跳转四元式——第二个操作数
    branch.item2 = subexpression2.op;

    //跳转四元式——标签
    label = "else" + to_string(branchNum++);
    branch.item3.label = label;

    //跳转四元式——注释
    branch.notation = "";

    //未放入四元式
    branch_op = branch;

    int value2 = expressionValue;
    if (value1 == 0 || value2 == 0) {
        file_error << lastSymbolNum << " f" << endl;
    }


#ifdef TURN_ON
    file_out << "<条件>" << endl;
#endif
}

extern int now_function_has_return;
extern int now_function_type;
void return_statement::subroutine() {
    //sample-> return / return(a+b)
    now_function_has_return = 1;
    PRE_READ_NEW_WORD
    if (TYPE == "LPARENT") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        if (TYPE != "RPARENT") {//return (dsad...
            //调用表达式
            expression subexpression;
            if (CALL_Expression) {
                subexpression.subroutine();
                expressionValue = subexpression.valueType;
            } else {
                error();
            }//调用表达式

            return_value = subexpression.op;
            if_has_return = true;

            if (expressionValue == 0) {//char{
                if (now_function_type == NONE_FUCTION)file_error << lastSymbolNum << " g" << endl;
                if (now_function_type == INT_FUNCTION)file_error << lastSymbolNum << " h" << endl;

            } else {
                if (now_function_type == NONE_FUCTION)
                    if (now_function_type == NONE_FUCTION)
                        file_error << lastSymbolNum << " g" << endl;
                if (now_function_type == CHAR_FUNCTION)file_error << lastSymbolNum << " h" << endl;
            }
        } else {//return ();
            if (now_function_type == INT_FUNCTION || now_function_type == CHAR_FUNCTION)
                file_error << lastSymbolNum << " h" << endl;

            if_has_return = false;
        }

        if (TYPE == "RPARENT") {//return ();
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else file_error << lastSymbolNum << " l" << endl;

    } else {//return ;
        if (now_function_type == INT_FUNCTION || now_function_type == CHAR_FUNCTION)
            file_error << lastSymbolNum << " h" << endl;

        if_has_return = false;
    }

#ifdef TURN_ON
    file_out << "<返回语句>" << endl;
#endif
}