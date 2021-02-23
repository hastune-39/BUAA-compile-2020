//
// Created by 像我这样优秀的人 on 2020/9/30.
//

#include "include/grammer_analysis.h"
#include "middle/FourOp.h"
#include "middle/BasicBlock.h"
#include "middle/MiddleCode.h"
#include "class/Statement.h"

#include "class/Expression.h"

extern map<int, symbolTable> Tables;
extern ofstream file_out;
extern int varAddress;
int nowLevel = 0;
extern int lastSymbolNum;
extern int symbolNum;
static int constant_integer_value;
static int constant_value_type;
int branchNum = 1;
int nowIsGlobalVarClare = 1;
function_line now_function_line;

/***
 * global variable
 */
queue<pair<string, string>> BACKSTRACK;
map<string, string> FUNC_TYPE;

string SYM;
string TYPE;
int now_function_has_return = 0;
int now_function_type;
//block
middleCode final_middleCode;
basicBlock only_block;
basicBlock global_block;

int constant_value = 0;

/***
 *
 */



void program_cycle_function();

void program_cycle_Variable_Function();

void get_data(pair<string, string> word) {
    TYPE = word.first;
    SYM = word.second;
}

//initialize
void grammer_analysis_RUN() {
    grammer_initialize();
    //program
    PRE_READ_NEW_WORD
    program();
}

void grammer_initialize() {
    lexical_str.clear();
    lexical_type.clear();
    TYPE.clear();
    SYM.clear();
    while (!BACKSTRACK.empty())BACKSTRACK.pop();

}

//<program>
void program() {
    //only one block
    final_middleCode.blockList.push_back(only_block);

    create_table(nowLevel);
    if (TYPE == "CONSTTK") {
        PRINT_PRE_WORD
        constant_description();
    }

    if (TYPE == "CHARTK" || TYPE == "INTTK") {//规则改变
        program_cycle_Variable_Function();
    }

    nowIsGlobalVarClare = 0;

    if (TYPE == "VOIDTK" || TYPE == "INTTK" || TYPE == "CHARTK") {
        program_cycle_function();
        PRINT_PRE_WORD
        main_function();
    } else error();

#ifdef TURN_ON
    file_out << "<程序>" << endl;
#endif
}

void program_cycle_Variable_Function() {//FIRST={CHAR/INT}
    bool variable_description_key = false;
    while (1) {
        if (TYPE == "CHARTK" || TYPE == "INTTK") {
            BACKSTRACK.push(PAIR(TYPE, SYM));
            PRE_READ_NEW_WORD
            if (TYPE == "IDENFR") {
                BACKSTRACK.push(PAIR(TYPE, SYM));
                PRE_READ_NEW_WORD
                if (TYPE == "ASSIGN" || TYPE == "COMMA" || TYPE == "SEMICN" || TYPE == "LBRACK") {
                    BACKSTRACK.push(PAIR(TYPE, SYM));
                    PRE_READ_NEW_WORD_FROM_ALL
                    //变量说明定义
                    variable_description_key = true;
                    //变量定义
                    variable_line line;
                    line.kind = K_VARIABLE;
                    Variable_define(line);
                    if (TYPE == "SEMICN") {
                        PRINT_PRE_WORD
                        PRE_READ_NEW_WORD
                    } else {
                        file_error << lastSymbolNum << " k" << endl;
                    }
                } else if (TYPE == "LPARENT") {
                    BACKSTRACK.push(PAIR(TYPE, SYM));
                    get_data(BACKSTRACK.front());
                    BACKSTRACK.pop();
                    break;
                } else { error(); }
            } else { error(); }
        } else break;
    }

#ifdef TURN_ON
    if (variable_description_key) file_out << "<变量说明>" << endl;
#endif
}

void program_cycle_function() {//FIRST={INT/CHAR/VOID} 一定是函数
    while (1) {//with_return,no_return, main
        if (TYPE == "VOIDTK") {
            BACKSTRACK.push(PAIR(TYPE, SYM));
            PRE_READ_NEW_WORD
            if (TYPE == "MAINTK") {
                BACKSTRACK.push(PAIR(TYPE, SYM));
                get_data(BACKSTRACK.front());
                BACKSTRACK.pop();
                break;
            } else if (TYPE == "IDENFR") {
                BACKSTRACK.push(PAIR(TYPE, SYM));
                READ_NEW_WORD
                function_no_return();
            } else error();
        } else if (TYPE == "INTTK" || TYPE == "CHARTK") {
            //有返回值函数定义
            PRINT_PRE_WORD
            function_with_return();
        } else error();
    }
}


//<constant_description>---------------------------------------------
void constant_description() {//FIRST={CONST}
    READ_NEW_WORD
    CALL_Constant_define
    //check ;
    if (TYPE == "SEMICN") {
        PRINT_PRE_WORD
        PRE_READ_NEW_WORD
    } else {
        file_error << lastSymbolNum << " k" << endl;
    }


    while (TYPE == "CONSTTK") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Constant_define
        //check ;
        if (TYPE == "SEMICN") {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else {
            file_error << lastSymbolNum << " k" << endl;
        }
    }

#ifdef TURN_ON
    file_out << "<常量说明>" << endl;
#endif
}


void Constant_define() {//FIRST={INT/CHAR}
    constant_line first;
    first.kind = K_CONSTANT;
    if (TYPE == "INTTK") {
        first.type = T_INT;
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        first.name = SYM;
        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        if (CALL_Constant_integer) {
            first.value = Constant_integer();
        } else error();
        set_table_symbol(nowLevel, first.kind, first);
        while (TYPE == "COMMA") {
            PRINT_PRE_WORD
            constant_line more;
            more.kind = K_CONSTANT;
            more.type = T_INT;
            READ_NEW_WORD
            if (TYPE != "IDENFR")error();
            more.name = SYM;
            READ_NEW_WORD
            if (TYPE != "ASSIGN")error();
            READ_NEW_WORD
            if (CALL_Constant_integer) {
                more.value = Constant_integer();
            } else error();
            set_table_symbol(nowLevel, more.kind, more);
        }
    } else if (TYPE == "CHARTK") {
        first.type = T_CHAR;
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        first.name = SYM;
        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        if (TYPE != "CHARCON")error();
        first.value = SYM[0];
        set_table_symbol(nowLevel, first.kind, first);
        PRE_READ_NEW_WORD
        while (TYPE == "COMMA") {
            PRINT_PRE_WORD
            constant_line more;
            more.kind = K_CONSTANT;
            more.type = T_CHAR;
            READ_NEW_WORD
            if (TYPE != "IDENFR")error();
            more.name = SYM;
            READ_NEW_WORD
            if (TYPE != "ASSIGN")error();
            READ_NEW_WORD
            if (TYPE != "CHARCON")error();
            more.value = SYM[0];
            set_table_symbol(nowLevel, more.kind, more);
            PRE_READ_NEW_WORD
        }
    }

#ifdef TURN_ON
    file_out << "<常量定义>" << endl;
#endif
}

//<variable description>--------------------------------------------------
void variable_description() {//FIRST={INT/CHAR}
    variable_line first;
    first.kind = K_VARIABLE;

    Variable_define(first);
    if (TYPE == "SEMICN") PRINT_PRE_WORD else file_error << lastSymbolNum << " k" << endl;
    PRE_READ_NEW_WORD_FROM_ALL
    while (TYPE == "CHARTK" || TYPE == "INTTK") {
        PRINT_PRE_WORD
        variable_line more;
        more.kind = K_VARIABLE;
        Variable_define(more);
        if (TYPE == "SEMICN") {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else {
            file_error << lastSymbolNum << " k" << endl;
        }

    }

#ifdef TURN_ON
    file_out << "<变量说明>" << endl;
#endif
}

void Variable_define(variable_line &line) {//FIRST = {INT/CHAR}
    if (TYPE == "INTTK")line.type = T_INT;
    else line.type = T_CHAR;
    READ_NEW_WORD
    variable(line);
    if (TYPE == "SEMICN" || TYPE == "COMMA") {
        //don't need to generate fourOp
        if (nowIsGlobalVarClare) {
            line.global = true;
        } else {
            line.global = false;
        }

        set_table_symbol(nowLevel, line.kind, line);

        //四元式 变量定义无初始化的var
        fourOp defNoInitial;

        defNoInitial.op = O_VAR;

        defNoInitial.item1.kind = K_VARIABLE;
        defNoInitial.item1.direct_address = true;
        if(line.global)defNoInitial.item1.addressBaseType=A_GP;
        else defNoInitial.item1.addressBaseType = A_VAR;
        defNoInitial.item1.address = line.address;
        defNoInitial.item1.dimension = line.dimension;

        defNoInitial.item2.kind = K_CONSTANT;
        defNoInitial.item2.value = 0;

        only_block.fourOpList.push_back(defNoInitial);

        if (TYPE == "COMMA") {
            Variable_define_no_initial(line);
        }
#ifdef TURN_ON
        file_out << "<变量定义无初始化>" << endl;
#endif
    } else if (TYPE == "COMMA") {

    } else if (TYPE == "ASSIGN") Variable_define_with_initial(line);
    else error();

#ifdef TURN_ON
    file_out << "<变量定义>" << endl;
#endif
}

//现在无初始化数组还不会自动赋0
void Variable_define_no_initial(variable_line &first) {//FIRST={,}
    while (TYPE == "COMMA") {
        PRINT_PRE_WORD
        variable_line line;
        line.type = first.type;
        line.kind = first.kind;
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        line.name = SYM;
        variable(line);

        if (nowIsGlobalVarClare) {
            line.global = true;
        } else {
            line.global = false;
        }
        set_table_symbol(nowLevel, line.kind, line);

        //四元式 变量定义无初始化的var
        fourOp defNoInitial;

        defNoInitial.op = O_VAR;

        defNoInitial.item1.kind = K_VARIABLE;
        defNoInitial.item1.direct_address = true;
        if(line.global)defNoInitial.item1.addressBaseType=A_GP;
        else defNoInitial.item1.addressBaseType = A_VAR;
        defNoInitial.item1.address = line.address;
        defNoInitial.item1.dimension = line.dimension;

        defNoInitial.item2.kind = K_CONSTANT;
        defNoInitial.item2.value = 0;

        only_block.fourOpList.push_back(defNoInitial);
    }

#ifdef TURN_ON
    file_out << "<变量定义无初始化>" << endl;
#endif
}

void fourOp_initArrayVar(variable_line &line, int *offset) {
    //四元式-数组变量初始化
    fourOp initialize_Array_Var;
    initialize_Array_Var.op = O_ASSIGN;
    //1.item1操作数
    initialize_Array_Var.item1.address = line.address + *(offset);
    if (nowIsGlobalVarClare) {
        initialize_Array_Var.item1.addressBaseType = A_GP;
    } else {
        initialize_Array_Var.item1.addressBaseType = A_VAR;
    }
    initialize_Array_Var.item1.direct_address = true;
    initialize_Array_Var.item1.dimension = 9;//不知道维数，但绝对大于0
    *(offset) = *(offset) + 4;
    //2.item2操作数
    initialize_Array_Var.item2.kind = K_CONSTANT;
    initialize_Array_Var.item2.value = constant_value;
    //3.放入四元式
    if (nowIsGlobalVarClare) {
        global_block.fourOpList.push_back(initialize_Array_Var);
    } else {
        only_block.fourOpList.push_back(initialize_Array_Var);
    }

}

void Variable_define_with_initial(variable_line &line) {//FIRST={=} 这里有大问题！
    bool dimenstion_equal = true;
    PRINT_PRE_WORD
    PRE_READ_NEW_WORD
    if (TYPE == "LBRACE") {
        int dimension1 = 1;
        PRINT_PRE_WORD
        PRE_READ_NEW_WORD
        if (TYPE == "LBRACE") {
            /***
             * 二维数组初始化
             */


            if (line.dimension != 2)dimenstion_equal = false;
            int dimension2 = 1;
            PRINT_PRE_WORD
            READ_NEW_WORD
            CALL_Constant_value

            int offset = 0;
            //四元式-数组变量初始化
            fourOp_initArrayVar(line, &offset);

            //check int=int char=char
            if (line.type != constant_value_type)file_error << lastSymbolNum << " o" << endl;

            while (TYPE == "COMMA") {
                PRINT_PRE_WORD
                READ_NEW_WORD
                CALL_Constant_value

                //四元式-数组变量初始化
                fourOp_initArrayVar(line, &offset);

                //check int=int char=char
                if (line.type != constant_value_type)file_error << lastSymbolNum << " o" << endl;

                dimension2++;
            }
            if (TYPE == "RBRACE") PRINT_PRE_WORD else dimenstion_equal = false;
            if (dimension2 != line.dimension2)dimenstion_equal = false;
            PRE_READ_NEW_WORD
            while (TYPE == "COMMA") {
                dimension1++;
                PRINT_PRE_WORD
                READ_NEW_WORD
                if (TYPE != "LBRACE")dimenstion_equal = false;
                else READ_NEW_WORD
                CALL_Constant_value

                //四元式-数组变量初始化
                fourOp_initArrayVar(line, &offset);


                //check int=int char=char
                if (line.type != constant_value_type)file_error << lastSymbolNum << " o" << endl;

                int moredimension2 = 1;
                while (TYPE == "COMMA") {
                    PRINT_PRE_WORD
                    READ_NEW_WORD
                    CALL_Constant_value

                    //四元式-数组变量初始化
                    fourOp_initArrayVar(line, &offset);

                    //check int=int char=char
                    if (line.type != constant_value_type)file_error << lastSymbolNum << " o" << endl;

                    moredimension2++;
                }
                if (TYPE == "RBRACE") {
                    PRINT_PRE_WORD
                    PRE_READ_NEW_WORD
                } else dimenstion_equal = false;
                if (moredimension2 != line.dimension2)dimenstion_equal = false;
            }
            if (TYPE == "RBRACE") {
                PRINT_PRE_WORD
                PRE_READ_NEW_WORD
            } else dimenstion_equal = false;
            if (dimension1 != line.dimension1)dimenstion_equal = false;
        } else {
            /***
             * 一维数组初始化,不分char,int,统统4字节
             */

            PRINT_PRE_WORD
            if (line.dimension != 1)dimenstion_equal = false;
            CALL_Constant_value

            int offset = 0;
            //四元式-数组变量初始化
            fourOp_initArrayVar(line, &offset);

            //check int=int char=char
            if (line.type != constant_value_type)file_error << lastSymbolNum << " o" << endl;

            while (TYPE == "COMMA") {
                PRINT_PRE_WORD
                READ_NEW_WORD
                CALL_Constant_value

                //四元式-数组变量初始化
                fourOp_initArrayVar(line, &offset);

                //check int=int char=char
                if (line.type != constant_value_type)file_error << lastSymbolNum << " o" << endl;

                dimension1++;
            }
            if (TYPE == "RBRACE") {
                PRINT_PRE_WORD
                PRE_READ_NEW_WORD
            } else dimenstion_equal = false;

            if (dimension1 != line.dimension1)dimenstion_equal = false;
        }


        if (nowIsGlobalVarClare) {
            line.global = true;
        } else {
            line.global = false;
        }
        //数组加入符号表
        set_table_symbol(nowLevel, line.kind, line);
    } else {
        if (line.dimension > 0)dimenstion_equal = false;
        PRINT_PRE_WORD
        CALL_Constant_value
        int value = constant_value;
        //generate fourOp
        fourOp varDefine_fourOp;
        varDefine_fourOp.item1.kind = K_VARIABLE;
        varDefine_fourOp.item1.address = line.address;
        if (nowIsGlobalVarClare) {
            varDefine_fourOp.item1.addressBaseType = A_GP;
        } else {
            varDefine_fourOp.item1.addressBaseType = A_VAR;
        }
        varDefine_fourOp.op = O_VAR;
        varDefine_fourOp.item2.kind = K_CONSTANT;
        varDefine_fourOp.item2.value = value;
        if (nowIsGlobalVarClare) {
            global_block.fourOpList.push_back(varDefine_fourOp);
        } else {
            only_block.fourOpList.push_back(varDefine_fourOp);
        }


        line.address = varDefine_fourOp.item1.address;
        if (nowIsGlobalVarClare) {
            line.global = true;
        } else {
            line.global = false;
        }
        //set symbolTable
        set_table_symbol(nowLevel, line.kind, line);

        //check int=int char=char
        if (line.type != constant_value_type)file_error << lastSymbolNum << " o" << endl;

    }

    if (dimenstion_equal == false) {
        file_error << lastSymbolNum << " n" << endl;
    }

#ifdef TURN_ON
    file_out << "<变量定义及初始化>" << endl;
#endif
}


void variable(variable_line &line) {//FIRST={IDENFR}
    if (TYPE == "IDENFR") {
        line.name = SYM;
        if (!BACKSTRACK.empty()) {
            get_data(BACKSTRACK.front());
            BACKSTRACK.pop();//处理的不行！
        } else
            PRE_READ_NEW_WORD

        if (TYPE == "LBRACK") {
            PRINT_PRE_WORD
            line.dimension = 1;
            READ_NEW_WORD
            CALL_Nosymbol_constant_integer
            line.dimension1 = constant_integer_value;
            if (TYPE == "RBRACK") {
                PRINT_PRE_WORD
                PRE_READ_NEW_WORD
            } else file_error << lastSymbolNum << " m" << endl;

            if (TYPE == "LBRACK") {
                line.dimension = 2;
                PRINT_PRE_WORD
                READ_NEW_WORD
                CALL_Nosymbol_constant_integer
                line.dimension2 = constant_integer_value;
                if (TYPE == "RBRACK") {
                    PRINT_PRE_WORD
                    PRE_READ_NEW_WORD
                } else file_error << lastSymbolNum << " m" << endl;
            }
        } else {
            line.dimension = 0;
        }

        //四元式-分配数组空间
//        if(line.dimension > 0){
//            fourOp allocArraySpcae;
//            //1.四元式操作符为分配
//            allocArraySpcae.op = O_arraySpaceAlloc;
//            //2.确定分配大小,不分char和int,统统4字节,呜呜呜
//            if(line.dimension == 1){//一维数组
//                line.size = 4*(line.dimension1);
//            }else {//二维数组
//                line.size = 4*(line.dimension1)*(line.dimension2);
//            }
//            //3.四元式item1操作数有了
//            allocArraySpcae.item1.value = line.size;
//            allocArraySpcae.item1.name = line.name;
//            //4.放入四元式
//            only_block.fourOpList.push_back(allocArraySpcae);
//            //5.更改地址空间
//            line.address = arrayAddress;
//            arrayAddress += line.size;
//        }
        //2.确定分配大小,不分char和int,统统4字节,呜呜呜

        if (line.dimension > 0) {
            if (line.dimension == 1) {//一维数组
                line.size = 4 * (line.dimension1);
            } else if (line.dimension == 2) {//二维数组
                line.size = 4 * (line.dimension1) * (line.dimension2);
            }
            //5.更改地址空间
            line.address = varAddress;
            varAddress += line.size;
        } else {
            line.address = varAddress;
            varAddress += 4;
        }
    } else error();
}


//function--------------------------------------------
//<function_with_return>
void function_with_return() { //FIRST = {char/int}
    //初始化地址空间,以k0为基准
    initVarAddress();
    varAddress += 4;//0($k0) 为回退的地址空间大小
    varAddress += 4; //为返回跳的ra值
    //

    now_function_has_return = 0;
    function_line line;
    line.kind = K_FUNCTION;
    line.has_return = true;
    Declaration_header(line);
    if (line.type == T_INT)now_function_type = INT_FUNCTION;
    else now_function_type = CHAR_FUNCTION;
    if (TYPE == "LPARENT") PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD
    if (TYPE == "RPARENT") {//空串也是参数表
#ifdef TURN_ON
        file_out << "<参数表>" << endl;
#endif
        PRINT_PRE_WORD
        READ_NEW_WORD
    } else {
        PRINT_PRE_WORD
        //(}
        if (TYPE == "LBRACE") {
            file_error << lastSymbolNum << " l" << endl;
        } else {
            //语法分析-调用参数表,将参数的类型和名称信息保存在line
            CALL_Parameters_table//非空串
            //(int a{
            if (TYPE == "LBRACE") {
                file_error << lastSymbolNum << " l" << endl;
            } else if (TYPE == "RPARENT") {//(int a)
                PRINT_PRE_WORD
                READ_NEW_WORD
            } else error();
        }
    }

    //函数step1: 四元式——生成函数名标签
    fourOp genFunctionLabel;
    genFunctionLabel.op = O_putlabel;
    genFunctionLabel.item1.label = line.name;
    genFunctionLabel.item1.isFunctionLabel = true;
    only_block.fourOpList.push_back(genFunctionLabel);

    //四元式-存入当前ra
    fourOp save_back_ra;
    save_back_ra.op = O_SaveBackRA;
    only_block.fourOpList.push_back(save_back_ra);

    /***
     * 生成参数定义四元式
     */

    if (TYPE != "LBRACE") error();
    //new level
    nowLevel++;
    create_table(nowLevel);
    int parameterNum = line.parameter_name.size();
    for (int i = 0; i < line.parameter_type.size(); i++) {
        variable_line varia;
        varia.kind = K_VARIABLE;
        if (line.parameter_type.at(i) == "INTTK")varia.type = T_INT;
        else varia.type = T_CHAR;
        varia.name = line.parameter_name.at(i);
        varia.dimension = 0;
        varia.dimension1 = 0;
        varia.dimension2 = 0;

        //确定每个参数相对于k0的偏移
        varia.address = varAddress;
        varAddress += 4;
        varia.global = false;

        //把参数填入内符号表,包括参数名称、类型、
        /***
         *  参数定义四元式
         */
        fourOp defParam;

        defParam.op = O_VAR;

        defParam.item1.direct_address = true;
        defParam.item1.addressBaseType = A_VAR;
        defParam.item1.address = varia.address;
        defParam.item1.kind = K_VARIABLE;

        defParam.item2.direct_address = true;
        defParam.item2.addressBaseType = A_VAR;
        defParam.item2.address = varia.address;
        defParam.item2.kind = K_VARIABLE;

        only_block.fourOpList.push_back(defParam);


        set_table_symbol(nowLevel, varia.kind, varia);
    }
    PRE_READ_NEW_WORD

    //CALL_Compound_sentence 里面会分配内存空间
    if (TYPE == "CONSTTK" || TYPE == "INTTK" || TYPE == "CHARTK" || (STATEMENT_CONDITION) || TYPE == "RBRACE") {
        Compound_sentence(line);
    } else error();

    line.size = varAddress;
    //把函数填入外符号表，包括函数名，函数所需地址空间大小, 参数的类型，参数的名称
    set_table_symbol(0, line.kind, line);
    //现在正在分析的函数是:...
    now_function_line = line;

    if (TYPE == "RBRACE") PRINT_PRE_WORD else error();

    //judge if has return
    if (!now_function_has_return)file_error << symbolNum << " h" << endl;


    //drop table and down level
    quit_table(nowLevel);
    nowLevel--;
    PRE_READ_NEW_WORD


#ifdef TURN_ON
    file_out << "<有返回值函数定义>" << endl;
#endif
}

//<function_no_return>
void function_no_return() {//FIRST={void}
    //1.初始化地址空间,以k0为基准
    initVarAddress();
    varAddress += 4;//回退的size大小
    varAddress += 4;//jr $ra的存储地方


    now_function_has_return = 0;
    now_function_type = NONE_FUCTION;
    function_line line;
    line.kind = K_FUNCTION;
    line.has_return = false;
    READ_NEW_WORD
    if (TYPE != "IDENFR") error();
    line.name = SYM;
    line.origin_name = SYM;
    //特殊化符号表
    FUNC_TYPE.insert(PAIR(SYM, "NO RETURN"));
    //
    READ_NEW_WORD
    if (TYPE != "LPARENT") error();
    PRE_READ_NEW_WORD
    if (TYPE == "RPARENT") {//空串也是参数表
#ifdef TURN_ON
        file_out << "<参数表>" << endl;
#endif
        PRINT_PRE_WORD
        READ_NEW_WORD
    } else {
        PRINT_PRE_WORD
        //(}
        if (TYPE == "LBRACE") {
            file_error << lastSymbolNum << " l" << endl;
        } else {

            CALL_Parameters_table//非空串
            //(int a{
            if (TYPE == "LBRACE") {
                file_error << lastSymbolNum << " l" << endl;
            } else if (TYPE == "RPARENT") {//int a
                PRINT_PRE_WORD
                READ_NEW_WORD
            } else error();
        }

    }

    //函数step1: 四元式——生成函数名标签
    fourOp genFunctionLabel;
    genFunctionLabel.op = O_putlabel;
    genFunctionLabel.item1.label = line.name;
    genFunctionLabel.item1.isFunctionLabel = true;
    only_block.fourOpList.push_back(genFunctionLabel);

    //四元式-存入当前ra
    fourOp save_back_ra;
    save_back_ra.op = O_SaveBackRA;
    only_block.fourOpList.push_back(save_back_ra);

    if (TYPE != "LBRACE") error();

    set_table_symbol(nowLevel, line.kind, line);
    //new level
    nowLevel++;
    create_table(nowLevel);
    for (int i = 0; i < line.parameter_type.size(); i++) {
        variable_line varia;
        varia.kind = K_VARIABLE;
        if (line.parameter_type.at(i) == "INTTK")varia.type = T_INT;
        else varia.type = T_CHAR;
        varia.name = line.parameter_name.at(i);

        varia.dimension = 0;
        varia.dimension1 = 0;
        varia.dimension2 = 0;

        //确定每个参数相对于k0的偏移
        varia.address = varAddress;
        varAddress += 4;
        varia.global = false;
        //2.填参数的符号表,从8开始

        //把参数填入内符号表,包括参数名称、类型、
        /***
         *  参数定义四元式
         */
        fourOp defParam;

        defParam.op = O_VAR;

        defParam.item1.direct_address = true;
        defParam.item1.addressBaseType = A_VAR;
        defParam.item1.address = varia.address;
        defParam.item1.kind = K_VARIABLE;

        defParam.item2.direct_address = true;
        defParam.item2.addressBaseType = A_VAR;
        defParam.item2.address = varia.address;
        defParam.item2.kind = K_VARIABLE;
        only_block.fourOpList.push_back(defParam);

        set_table_symbol(nowLevel, varia.kind, varia);
    }
    PRE_READ_NEW_WORD

    //CALL_Compound_sentence
    if (TYPE == "CONSTTK" || TYPE == "INTTK" || TYPE == "CHARTK" || (STATEMENT_CONDITION) || TYPE == "RBRACE") {
        //3.调用复合语,里面的变量定义增加line的size
        Compound_sentence(line);
    } else error();

    if (TYPE == "RBRACE") PRINT_PRE_WORD else error();

    //judge return

    //down level
    quit_table(nowLevel);
    nowLevel--;
    PRE_READ_NEW_WORD

    //四元式-从函数退出(无返回值函数结尾必有退出)
    fourOp changeSpace;
    changeSpace.op = O_back;
    only_block.fourOpList.push_back(changeSpace);
#ifdef TURN_ON
    file_out << "<无返回值函数定义>" << endl;
#endif
}

//<main_function>
void main_function() {//FIRST={void}
    //初始化地址空间,以k0为基准
    initVarAddress();
    //四元式-产生main标签
    fourOp genMainLabel;
    genMainLabel.op = O_putlabel;
    genMainLabel.item1.label = "main";
    genMainLabel.item1.isFunctionLabel = true;
    only_block.fourOpList.push_back(genMainLabel);


    //将main加入符号表
    function_line main_line;
    main_line.name = "main";
    main_line.kind = K_FUNCTION;
    main_line.has_return = false;
    now_function_line = main_line;

    now_function_has_return = 0;
    now_function_type = NONE_FUCTION;
    READ_NEW_WORD
    if (TYPE != "MAINTK") error();
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    //void main({
    if (TYPE == "LBRACE") {
        file_error << lastSymbolNum << " l" << endl;
    } else if (TYPE == "RPARENT") {//void main()
        READ_NEW_WORD
    }

    nowLevel++;
    create_table(nowLevel);
    PRE_READ_NEW_WORD
    //CALL_Compound_sentence 并在里面填符号表
    if (TYPE == "CONSTTK" || TYPE == "INTTK" || TYPE == "CHARTK" || (STATEMENT_CONDITION) || TYPE == "RBRACE") {
        Compound_sentence(main_line);
    } else error();

    if (TYPE == "RBRACE") PRINT_PRE_WORD else error();
    nowLevel--;

    //四元式-退出程序
    fourOp exit;
    exit.op = O_EXIT;
    only_block.fourOpList.push_back(exit);
#ifdef TURN_ON
    file_out << "<主函数>" << endl;
#endif
}

void Declaration_header(function_line &line) {//FIRST = {INT/CHAR}
    line.kind = K_FUNCTION;
    if (TYPE == "INTTK")line.type = T_INT;
    else line.type = T_CHAR;
    READ_NEW_WORD
    if (TYPE != "IDENFR") error();
    line.name = SYM;
    line.origin_name = SYM;
    //特殊化符号表
    FUNC_TYPE.insert(PAIR(SYM, "WITH RETURN"));
    //
    if (!BACKSTRACK.empty()) {
        get_data(BACKSTRACK.front());
        BACKSTRACK.pop();
    } else
        PRE_READ_NEW_WORD

#ifdef TURN_ON
    file_out << "<声明头部>" << endl;
#endif
}


void Parameters_table(function_line &line) {//FIRST={CHAR/INT} 注：有空串，交给外部处理
    // sample
    //char a,int b,...;

    Type_idenfr(line);
    if (TYPE == "IDENFR") PRINT_PRE_WORD else error();
    line.parameter_name.push_back(SYM);
    PRE_READ_NEW_WORD
    while (TYPE == "COMMA") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Type_idenfr
        if (TYPE == "IDENFR") PRINT_PRE_WORD else error();
        line.parameter_name.push_back(SYM);
        PRE_READ_NEW_WORD
    }

#ifdef TURN_ON
    file_out << "<参数表>" << endl;
#endif
}

void Type_idenfr(function_line &line) {//FIRST={INT/CHAR}
    line.parameter_type.push_back(TYPE);
    PRE_READ_NEW_WORD
    //类型标识符不输出
}

//<Compound_sentence>----------------------------------------------------

void Compound_sentence(function_line &unsavedSize_functionLine) {//FIRST={const/int/char/语句列/空}
    if (TYPE == "CONSTTK") {
        PRINT_PRE_WORD
        constant_description();
    }

    if (TYPE == "INTTK" || TYPE == "CHARTK") {
        PRINT_PRE_WORD
        variable_description();
    }

    //获得函数块变量定义所需的内存空间，并存入符号表
    unsavedSize_functionLine.size = getVarAddress();
    now_function_line.size = getVarAddress();
    set_table_symbol(nowLevel, K_FUNCTION, unsavedSize_functionLine);

    //语句列 ???????????????????????????????????????????
    if (TYPE == "RBRACE") {
#ifdef TURN_ON
        file_out << "<语句列>" << endl;
#endif
    } else {
        PRINT_PRE_WORD
        CALL_STATEMENT_LIST
    }

#ifdef TURN_ON
    file_out << "<复合语句>" << endl;
#endif
}

void Statement_list() {//语句列可能为空,但是进入这个不为空
    Statement();
    while (STATEMENT_CONDITION) {
        PRINT_PRE_WORD
        Statement();
    }

#ifdef TURN_ON
    file_out << "<语句列>" << endl;
#endif
}

void Statement() {//语句不为空，至少为;  //不确定，前一个应该PRE_READ
    if (TYPE == "WHILETK" || TYPE == "FORTK")
        Loop_statement();
    else if (TYPE == "IFTK")
        Condition_statement();
    else if (TYPE == "IDENFR") {
        BACKSTRACK.push(PAIR(TYPE, SYM));
        PRE_READ_NEW_WORD
        BACKSTRACK.push(PAIR(TYPE, SYM));
        if (TYPE == "LPARENT") {//？？区分有无函数调用
            get_data(BACKSTRACK.front());
            BACKSTRACK.pop();
            if (FUNC_TYPE[SYM] == "WITH RETURN")Function_with_return_statement();//???
            else if (FUNC_TYPE[SYM] == "NO RETURN")Function_no_return_statement();
            else cout << "grammer-statement: use no type function!";

            if (TYPE == "SEMICN") {
                PRINT_PRE_WORD
                PRE_READ_NEW_WORD
            } else file_error << lastSymbolNum << " k" << endl;

        } else if (TYPE == "LBRACK" || TYPE == "ASSIGN") {
            get_data(BACKSTRACK.front());
            BACKSTRACK.pop();
            Assign_statement();
            if (TYPE == "SEMICN") {
                PRINT_PRE_WORD
                PRE_READ_NEW_WORD
            } else file_error << lastSymbolNum << " k" << endl;
        } else error();
    } else if (TYPE == "SCANFTK") {
        Read_statement();
        if (TYPE == "SEMICN") {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else file_error << lastSymbolNum << " k" << endl;
    } else if (TYPE == "PRINTFTK") {
        Write_statement();
        if (TYPE == "SEMICN") {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else file_error << lastSymbolNum << " k" << endl;
    } else if (TYPE == "SWITCHTK") {
        Switch_statement();
    } else if (TYPE == "SEMICN") {
        PRE_READ_NEW_WORD
    } else if (TYPE == "RETURNTK") {
        return_statement returnStatement;
        returnStatement.subroutine();
        //四元式-存入返回结果
        if (returnStatement.if_has_return == true) {//有返回值
            fourOp saveReturnValue;
            saveReturnValue.op = O_SaveBackValue;
            saveReturnValue.item1 = returnStatement.return_value;
            only_block.fourOpList.push_back(saveReturnValue);
        } else if (returnStatement.if_has_return == false) {
            //main函数返回,则直接退出
            if (now_function_line.name == "main") {
                //四元式-退出程序
                fourOp exit;
                exit.op = O_EXIT;
                only_block.fourOpList.push_back(exit);

                if (TYPE == "SEMICN") {
                    PRINT_PRE_WORD
                    PRE_READ_NEW_WORD
                } else file_error << lastSymbolNum << " k" << endl;
                return;
            }
        }
        //四元式-回退并跳转
        fourOp changeSpace;
        changeSpace.op = O_back;
        only_block.fourOpList.push_back(changeSpace);


        if (TYPE == "SEMICN") {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else file_error << lastSymbolNum << " k" << endl;
    } else if (TYPE == "LBRACE") {
        PRE_READ_NEW_WORD

        if (TYPE == "RBRACE") {//空
#ifdef TURN_ON
            file_out << "<语句列>" << endl;
#endif
            PRINT_PRE_WORD
        } else {
            PRINT_PRE_WORD
            CALL_STATEMENT_LIST
            if (TYPE == "RBRACE") PRINT_PRE_WORD else error();
        }
        //
        PRE_READ_NEW_WORD
    } else file_error << lastSymbolNum << " k" << endl;//空语句

#ifdef TURN_ON
    file_out << "<语句>" << endl;
#endif
}

void Loop_statement() {//FIRST={while/for}
    if (TYPE == "WHILETK") {
        //四元式-放置while标签
        string label_while = "while_b" + to_string(branchNum++);
        fourOp putWhileLabel;
        putWhileLabel.op = O_putlabel;
        putWhileLabel.item1.label = label_while;
        only_block.fourOpList.push_back(putWhileLabel);

        //语法分析
        READ_NEW_WORD
        if (TYPE != "LPARENT")error();
        READ_NEW_WORD

        //条件语句调用
        condition while_condition;
        if (TYPE == "PLUS" || TYPE == "MINU" || TYPE == "IDENFR" || TYPE == "LPARENT" || TYPE == "INTCON" ||
            TYPE == "CHARCON") {
            while_condition.subroutine();
        } else cout << "grammer-while: error!" << endl;

        //四元式-有条件跳转语句
        string end_label = "while_end" + to_string(branchNum++);
        while_condition.branch_op.item3.label = end_label;
        while_condition.branch_op.notation = "#while语句,在源代码第" + to_string(lastSymbolNum) + "行";
        only_block.fourOpList.push_back(while_condition.branch_op);

        //语法分析
        if (TYPE != "RPARENT") {
            file_error << lastSymbolNum << " l" << endl;
        } else {
            PRINT_PRE_WORD
            READ_NEW_WORD
        }

        //语句列
        CALL_STATEMENT

        //四元式-无条件跳转至while起始标签
        fourOp j_while;
        j_while.op = O_j;
        j_while.item1.label = label_while;
        only_block.fourOpList.push_back(j_while);

        //放置end标签
        fourOp putEndLabel;
        putEndLabel.op = O_putlabel;
        putEndLabel.item1.label = end_label;
        only_block.fourOpList.push_back(putEndLabel);

    } else if (TYPE == "FORTK") {
        //sample for(i=0;i<10;i=i+1) i++;

        //语法分析-    标识符=表达式
        READ_NEW_WORD
        if (TYPE != "LPARENT")error();
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();

        variable_line initial_variable;
        if (check_table_symbol(SYM)) {//取待增加的标识符
            symbolLine &temp = get_table_symbol(SYM);//check if const
            if (temp.kind == K_CONSTANT) {
                file_error << lastSymbolNum << " j" << endl;
                cout << "grammer-for-init:不能对常量赋值!" << endl;
            } else {
                initial_variable = dynamic_cast<variable_line &>(temp);
            }
        }


        //四元式-for循环语句变量初始化的赋值四元式
        fourOp forInitVariable;
        //1.操作类型有了
        forInitVariable.op = O_ASSIGN;
        //2.top操作符有了
        forInitVariable.item1.kind = K_VARIABLE;
        forInitVariable.item1.address = initial_variable.address;
        if (initial_variable.global) {
            forInitVariable.item1.addressBaseType = A_GP;
        } else {
            forInitVariable.item1.addressBaseType = A_VAR;
        }

        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        //调用表达式
        expression subexpression;
        if (CALL_Expression) {
            subexpression.subroutine();
            expressionValue = subexpression.valueType;
        } else {
            error();
        }

        //3.left操作符有了
        forInitVariable.item2 = subexpression.op;

        //四元式-放入变量初始化四元式
        only_block.fourOpList.push_back(forInitVariable);

        if (TYPE == "SEMICN") {
            PRINT_PRE_WORD
            READ_NEW_WORD
        } else file_error << lastSymbolNum << " k" << endl;

        //四元式-放置for标签
        string for_label = "for_b" + to_string(branchNum++);
        fourOp putForLabel;
        putForLabel.op = O_putlabel;
        putForLabel.item1.label = for_label;
        only_block.fourOpList.push_back(putForLabel);

        //语法分析- 调用条件
        condition for_condition;
        if (TYPE == "PLUS" || TYPE == "MINU" || TYPE == "IDENFR" || TYPE == "LPARENT" || TYPE == "INTCON" ||
            TYPE == "CHARCON") {
            for_condition.subroutine();
        } else error();

        //四元式-有条件跳转至end
        string end_label = "for_end" + to_string(branchNum++);
        for_condition.branch_op.item3.label = end_label;
        for_condition.branch_op.notation = "#for语句,在源代码第" + to_string(lastSymbolNum) + "行";
        only_block.fourOpList.push_back(for_condition.branch_op);

        //四元式-加减步长的赋值语句
        fourOp assignFoot;
        assignFoot.op = O_ASSIGN;

        //四元式-"标识符+/-步长" 赋值语句右边表达式的计算
        fourOp caculateFoot;
        if (TYPE == "SEMICN") {
            PRINT_PRE_WORD
            READ_NEW_WORD
        } else file_error << lastSymbolNum << " k" << endl;

        if (TYPE != "IDENFR")error();
        variable_line variable1;
        if (check_table_symbol(SYM)) {//取待增加的标识符
            symbolLine &temp = get_table_symbol(SYM);//check if const
            if (temp.kind == K_CONSTANT) {
                file_error << lastSymbolNum << " j" << endl;
                cout << "grammer-for-foot:不能对常量赋值!" << endl;
            } else {
                variable1 = dynamic_cast<variable_line &>(temp);
            }
        }


        //1.top操作数有了,假设被赋值的操作数只能使int类型,且不能为数组
        assignFoot.item1.kind = K_VARIABLE;
        assignFoot.item1.address = variable1.address;
        if (variable1.global) {
            assignFoot.item1.addressBaseType = A_GP;
        } else {
            assignFoot.item1.addressBaseType = A_VAR;
        }

        //1.以及caculateFoot的top
        caculateFoot.item1.kind = K_VARIABLE;
        caculateFoot.item1.setTempAddress();

        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        if (TYPE != "IDENFR") error();

        if (check_table_symbol(SYM)) {//获取第二个标识符
            symbolLine &temp = get_table_symbol(SYM);//check if const
            if (temp.kind == K_CONSTANT) {
                constant_line right_constant = dynamic_cast<constant_line &>(temp);
                //2.left操作数有了,为constant类型
                caculateFoot.item2.kind = K_CONSTANT;
                caculateFoot.item2.value = right_constant.value;
            } else if (temp.kind == K_VARIABLE) {
                variable_line right_variable = dynamic_cast<variable_line &>(temp);
                //2.left操作数有了,为variable类型
                caculateFoot.item2.kind = K_VARIABLE;
                caculateFoot.item2.address = right_variable.address;
                if (right_variable.global) {
                    caculateFoot.item2.addressBaseType = A_GP;
                } else {
                    caculateFoot.item2.addressBaseType = A_VAR;
                }
            } else cout << "grammer-for-foot: 未知的标识符类型" << endl;
        }

        READ_NEW_WORD
        //3.op操作数有了
        if (TYPE == "PLUS" || TYPE == "MINU") {
            if (TYPE == "PLUS") {
                caculateFoot.op = O_PLUS;
            } else if (TYPE == "MINU") {
                caculateFoot.op = O_MINU;
            } else cout << "grammer-for-foot: error!" << endl;
        } else {
            error();
        }
        READ_NEW_WORD

        //4.right操作数有了
        CALL_Foot
        caculateFoot.item3.kind = K_CONSTANT;
        caculateFoot.item3.value = constant_integer_value;

        //5.还不能放，要等语句列放完后才能放

        if (TYPE != "RPARENT") {
            file_error << lastSymbolNum << " l" << endl;
        } else {
            PRINT_PRE_WORD
            READ_NEW_WORD
        }

        //代码生成-语句列
        CALL_STATEMENT

        //四元式-放入"标识符+步长"四元式
        only_block.fourOpList.push_back(caculateFoot);
        assignFoot.item2 = caculateFoot.item1;
        only_block.fourOpList.push_back(assignFoot);

        //四元式-无条件跳转至for标签
        fourOp jumpfor;
        jumpfor.op = O_j;
        jumpfor.item1.label = for_label;
        only_block.fourOpList.push_back(jumpfor);

        //四元式-放置for_end标签
        fourOp putEndLabel;
        putEndLabel.op = O_putlabel;
        putEndLabel.item1.label = end_label;
        only_block.fourOpList.push_back(putEndLabel);

    } else error();

#ifdef TURN_ON
    file_out << "<循环语句>" << endl;
#endif
}

void Condition_statement() {//FIRST={if}
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    //获得condition类
    condition if_condition;
    if (TYPE == "PLUS" || TYPE == "MINU" || TYPE == "IDENFR" || TYPE == "LPARENT" || TYPE == "INTCON" ||
        TYPE == "CHARCON") {
        if_condition.subroutine();
    } else error();

    if_condition.branch_op.notation = "#if条件语句,源代码第" + to_string(lastSymbolNum) + "行";
    only_block.fourOpList.push_back(if_condition.branch_op);

    if (TYPE != "RPARENT") {
        file_error << lastSymbolNum << " l" << endl;
    } else {
        PRINT_PRE_WORD
        READ_NEW_WORD
    }


    CALL_STATEMENT//为真执行语句

    if (TYPE == "ELSETK") {
        //if-else-end结构

        //四元式——无条件跳转至end
        string end_label = "end" + to_string(branchNum++);
        fourOp jumpend;
        jumpend.op = O_j;
        jumpend.item1.label = end_label;
        only_block.fourOpList.push_back(jumpend);

        //放置else标签
        fourOp putElseLabel;
        putElseLabel.op = O_putlabel;
        putElseLabel.item1.label = if_condition.branch_op.item3.label;
        putElseLabel.notation = "#else标签，源代码第" + to_string(symbolNum) + "行";
        only_block.fourOpList.push_back(putElseLabel);

        //else语句
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_STATEMENT//调用语句

        //放置end标签
        fourOp putEndLabel;
        putEndLabel.op = O_putlabel;
        putEndLabel.item1.label = end_label;
        putElseLabel.notation = "#end标签，源代码第" + to_string(symbolNum);
        only_block.fourOpList.push_back(putEndLabel);

    } else {
        //if-else结构
        //放置else标签
        fourOp putElseLabel;
        putElseLabel.op = O_putlabel;
        putElseLabel.item1.label = if_condition.branch_op.item3.label;
        putElseLabel.notation = "#if退出标签，源代码第" + to_string(symbolNum) + "行";
        only_block.fourOpList.push_back(putElseLabel);
    }

#ifdef TURN_ON
    file_out << "<条件语句>" << endl;
#endif
}


void Function_with_return_statement() {//FIRST={标识符}

    function_line line;
    if (check_table_symbol(SYM)) {
        line = dynamic_cast<function_line &>(get_table_symbol(SYM));
    }
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    PRE_READ_NEW_WORD_FROM_ALL

    //1.传入参数,在值参数表调用里完成,记得将要保存的地方+size的偏移
    //值参数表
    //function()
    if (TYPE == "RPARENT") {//空也为值参数表
#ifdef TURN_ON
        file_out << "<值参数表>" << endl;
#endif
        PRINT_PRE_WORD

        if (line.parameter_type.size() != 0) {
            file_error << lastSymbolNum << " d" << endl;
        }

        PRE_READ_NEW_WORD
    } else if (TYPE != "SEMICN") {
        //调用值参数表
        PRINT_PRE_WORD
        CALL_Value_parameter_table
        if (TYPE == "RPARENT") {//function(a)
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else {//function(a;
            file_error << lastSymbolNum << " l" << endl;
        }
    } else {//function(;
        //直接分号，错误
        file_error << lastSymbolNum << " l" << endl;
    }

    //2.调用函数前增加空间并保存要回退的空间
    //四元式-增加空间并填入要回退的空间
    fourOp addSpace;
    addSpace.op = O_addSpace;
    addSpace.item1.size = now_function_line.size;
    only_block.fourOpList.push_back(addSpace);

    //3.跳转至函数入口处
    //四元式-jal指令
    fourOp jumpToFunction;
    jumpToFunction.op = O_JumpToFunction;
    jumpToFunction.item1.label = line.origin_name;
    only_block.fourOpList.push_back(jumpToFunction);


#ifdef TURN_ON
    file_out << "<有返回值函数调用语句>" << endl;
#endif
}

//void Function_statement() {
//    function_line line;
//    if (check_table_symbol(SYM)) {
//        line = dynamic_cast<function_line &>(get_table_symbol(SYM));
//    }
//    READ_NEW_WORD
//    if (TYPE != "LPARENT")error();
//    PRE_READ_NEW_WORD
//    //值参数表
//    if (TYPE == "RPARENT") {//空也为值参数表
//#ifdef TURN_ON
//        file_out << "<值参数表>" << endl;
//#endif
//        PRINT_PRE_WORD
//        if (line.parameter_type.size() != 0) {
//            file_error << lastSymbolNum << " d" << endl;
//        }
//        PRE_READ_NEW_WORD
//    } else if (TYPE != "SEMICN") {
//        //调用值参数表
//        PRINT_PRE_WORD
//        CALL_Value_parameter_table
//        if (TYPE == "RPARENT") {
//            PRINT_PRE_WORD
//            PRE_READ_NEW_WORD
//        } else file_error << lastSymbolNum << " l" << endl;
//    } else {
//        file_error << lastSymbolNum << " l" << endl;
//    }
//
//#ifdef TURN_ON
//    file_out << "<无返回值函数调用语句>" << endl;
//#endif
//}

void Function_no_return_statement() {//FIRST={IDENFR}
    function_line line;
    if (check_table_symbol(SYM)) {
        line = dynamic_cast<function_line &>(get_table_symbol(SYM));
    }
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    PRE_READ_NEW_WORD
    //值参数表
    if (TYPE == "RPARENT") {//空也为值参数表
#ifdef TURN_ON
        file_out << "<值参数表>" << endl;
#endif
        PRINT_PRE_WORD
        if (line.parameter_type.size() != 0) {
            file_error << lastSymbolNum << " d" << endl;
        }
        PRE_READ_NEW_WORD
    } else if (TYPE != "SEMICN") {
        //调用值参数表
        PRINT_PRE_WORD

        //1.将参数填入相应空间,(此时是上一级空间，填入地址要加size偏移)
        CALL_Value_parameter_table
        if (TYPE == "RPARENT") {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else file_error << lastSymbolNum << " l" << endl;
    } else {
        file_error << lastSymbolNum << " l" << endl;
    }

    //2.进行函数调用:jal跳转,增加空间,保存size和当前ra
    //四元式-增加空间并填入要回退的空间
    fourOp addSpace;
    addSpace.op = O_addSpace;
    addSpace.item1.size = now_function_line.size;
    only_block.fourOpList.push_back(addSpace);

    //3.跳转至函数入口处
    //四元式-jal指令
    fourOp jumpToFunction;
    jumpToFunction.op = O_JumpToFunction;
    jumpToFunction.item1.label = line.origin_name;
    only_block.fourOpList.push_back(jumpToFunction);
#ifdef TURN_ON
    file_out << "<无返回值函数调用语句>" << endl;
#endif
}

void Assign_statement() {//FIRST{标识符} SECOND{{/=}
    //initTempVarOffset();未考虑超出地址问题
    variable_line variable;
    if (check_table_symbol(SYM)) {//if exist
        symbolLine &temp = get_table_symbol(SYM);//check if const
        if (temp.kind == K_CONSTANT) {
            file_error << lastSymbolNum << " j" << endl;
        } else {
            variable = dynamic_cast<variable_line &>(temp);
        }
    }
    //load address
    fourOp assignFourOp;
    assignFourOp.op = O_ASSIGN;
    operand top_op;
    top_op.address = variable.address;
    if (variable.global) {
        top_op.addressBaseType = A_GP;
    } else {
        top_op.addressBaseType = A_VAR;
    }
    top_op.kind = K_VARIABLE;
    assignFourOp.item1 = top_op;

    PRE_READ_NEW_WORD_FROM_ALL
    if (TYPE == "ASSIGN") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        //调用表达式
        expression subexpression;
        if (CALL_Expression) {
            subexpression.subroutine();
            expressionValue = subexpression.valueType;
            assignFourOp.item2 = subexpression.op;
        } else {
            error();
        }//调用表达式

        //add assign fourOp
        only_block.fourOpList.push_back(assignFourOp);
    } else if (TYPE == "LBRACK") {
        /***
         * 四元式-数组元素赋值
         */
        assignFourOp.op = O_ASSIGN;

        PRINT_PRE_WORD
        READ_NEW_WORD
        //调用表达式
        expression subexpression1;
        if (CALL_Expression) {
            subexpression1.subroutine();
            expressionValue = subexpression1.valueType;
        } else {
            error();
        }//调用表达式
        //check array subscript
        if (expressionValue == 0) {
            file_error << lastSymbolNum << " i" << endl;
        }
        //check ]
        if (TYPE == "RBRACK") {
            PRINT_PRE_WORD
            PRE_READ_NEW_WORD
        } else file_error << lastSymbolNum << " m" << endl;

        expression subexpression2;
        if (TYPE == "LBRACK") {
            PRINT_PRE_WORD
            READ_NEW_WORD
            //调用表达式
            if (CALL_Expression) {
                subexpression2.subroutine();
                expressionValue = subexpression2.valueType;
            } else {
                error();
            }//调用表达式
            //check array subscript
            if (expressionValue == 0) {
                file_error << lastSymbolNum << " i" << endl;
            }
            //check ]
            if (TYPE == "RBRACK") {
                PRINT_PRE_WORD
                PRE_READ_NEW_WORD
            } else file_error << lastSymbolNum << " m" << endl;
        }

        //2.item1操作数有了
        assignFourOp.item1 = getArrayOp(variable.address, variable.global, variable.dimension, variable.dimension1,
                                        variable.dimension2, subexpression1, subexpression2);


        if (TYPE == "ASSIGN") PRINT_PRE_WORD else error();

        READ_NEW_WORD
        //调用表达式
        expression subexpression;
        if (CALL_Expression) {
            subexpression.subroutine();
            expressionValue = subexpression.valueType;
        } else {
            error();
        }

        //3.item2操作数有了
        assignFourOp.item2 = subexpression.op;

        //4.放入
        only_block.fourOpList.push_back(assignFourOp);
    } else error();

#ifdef TURN_ON
    file_out << "<赋值语句>" << endl;
#endif
}

void Read_statement() {
    //prepare fourOp
    fourOp read_fourOp;
    read_fourOp.op = O_SCANF;

    //sample-> scanf(s1);
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    if (TYPE != "IDENFR")error();

    //why? don't understand dynamic cast
    variable_line getVariable;
    if (check_table_symbol(SYM)) {//if exist
        symbolLine &temp = get_table_symbol(SYM);//check
        if (temp.kind == K_CONSTANT) {
            file_error << lastSymbolNum << " j" << endl;
        } else {
            variable_line getVariable = dynamic_cast<variable_line &>(temp);
            //now only can read to variable
            //prepare fourOp's l_op
            read_fourOp.item1.kind = K_VARIABLE;
            read_fourOp.item1.address = getVariable.address;
            if (getVariable.global) {
                read_fourOp.item1.addressBaseType = A_GP;
            } else {
                read_fourOp.item1.addressBaseType = A_VAR;
            }
//            if(getVariable.global){
//                read_fourOp.item2.addressBaseType = A_GP;
//            }else {
//                read_fourOp.item2.addressBaseType = A_VAR;
//            }
            read_fourOp.item1.valueType = getVariable.type;
        }
    }


    //generate fourOp
    only_block.fourOpList.push_back(read_fourOp);

    READ_NEW_WORD
    if (TYPE != "RPARENT") {
        file_error << lastSymbolNum << " l" << endl;
    } else
        PRE_READ_NEW_WORD

#ifdef TURN_ON
    file_out << "<读语句>" << endl;
#endif
}

void Write_statement() {//FIRST={printf}
    //sample-> printf("str", a+b+c);

    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    if (TYPE == "STRCON") {
        //generate fourOp of string
        fourOp string_fourOp;
        string_fourOp.op = O_PRINTF;
        string_fourOp.item2.set_stringOp(SYM);
        string_fourOp.item3.valueType = 1;
        string_fourOp.notation = "#写语句,在源代码第" + to_string(symbolNum) + "行";
        CALL_Character_set
        if (TYPE == "COMMA") {
            //no enter
            string_fourOp.item3.valueType = 0;
            only_block.fourOpList.push_back(string_fourOp);

            PRINT_PRE_WORD
            READ_NEW_WORD
            //调用表达式
            expression subexpression;
            if (CALL_Expression) {
                subexpression.subroutine();
                expressionValue = subexpression.valueType;
            } else {
                error();
            }

            //generate printf<expression> fourOp
            fourOp printfExpression_fourOp;
            if (subexpression.valueType == 0) {// printf character
                printfExpression_fourOp.item2 = subexpression.op;
                printfExpression_fourOp.item2.valueType = 0;
            } else {//printf integer
                printfExpression_fourOp.item2 = subexpression.op;
                printfExpression_fourOp.item2.valueType = 1;
            }
            printfExpression_fourOp.op = O_PRINTF;
            printfExpression_fourOp.item3.valueType = 1;
            only_block.fourOpList.push_back(printfExpression_fourOp);
        } else if (TYPE == "RPARENT") {
            only_block.fourOpList.push_back(string_fourOp);
        } else {
            only_block.fourOpList.push_back(string_fourOp);
            error();
        }
    } else {//调用表达式
        expression subexpression;
        if (CALL_Expression) {
            subexpression.subroutine();
            expressionValue = subexpression.valueType;
        } else {
            error();
        }
        //generate printf<expression> fourOp
        fourOp printfExpression_fourOp;
        printfExpression_fourOp.notation = "#写语句,在源代码第" + to_string(symbolNum) + "行";
        if (subexpression.valueType == 0) {// printf character
            printfExpression_fourOp.item2 = subexpression.op;
            printfExpression_fourOp.item2.valueType = 0;
        } else {//printf integer
            printfExpression_fourOp.item2 = subexpression.op;
            printfExpression_fourOp.item2.valueType = 1;
        }
        printfExpression_fourOp.op = O_PRINTF;
        printfExpression_fourOp.item3.valueType = 1;//enter
        only_block.fourOpList.push_back(printfExpression_fourOp);
    }

    if (TYPE == "RPARENT") {
        PRINT_PRE_WORD
        PRE_READ_NEW_WORD
    } else file_error << lastSymbolNum << " l" << endl;


#ifdef TURN_ON
    file_out << "<写语句>" << endl;
#endif
}

void Switch_statement() {//first={swicth}
    //sample-> swicth(identify){case a+b:b+d; default;}
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    //调用表达式
    expression subexpression;
    if (CALL_Expression) {
        subexpression.subroutine();
        expressionValue = subexpression.valueType;
    } else {
        error();
    }//调用表达式
    int switchType = expressionValue;
    if (TYPE == "RPARENT") {
        PRINT_PRE_WORD
        READ_NEW_WORD
    } else file_error << lastSymbolNum << " l" << endl;

    if (TYPE != "LBRACE")error();
    READ_NEW_WORD

    /***
     * 在刚计算完表达式的结果时产生将表达式计算结果存入内存的四元式，
     * 这样第一条case用表达式，进行消除，第二条以后用内存里的，仍然满足'一次产生，一次使用'的约束
     */

    //四元式-先产生个swicth最终的label吧
    string swicthEndLable = "switch_end" + to_string(branchNum++);

    //语法分析-情况表调用
    if (TYPE == "CASETK") {
        Case_table(switchType, subexpression.op, swicthEndLable);
    } else error();

    PRINT_PRE_WORD
    CALL_Loss_default//缺省调用
    if (TYPE == "RBRACE") PRINT_PRE_WORD else error();
    int note_swicthEnd = symbolNum;

    PRE_READ_NEW_WORD

    //四元式-放置switch_end标签
    fourOp putSwitchEndLabel;
    putSwitchEndLabel.op = O_putlabel;
    putSwitchEndLabel.item1.label = swicthEndLable;
    putSwitchEndLabel.notation = "#switch分支结束,源代码在第" + to_string(note_swicthEnd) + "行";
    only_block.fourOpList.push_back(putSwitchEndLabel);

#ifdef TURN_ON
    file_out << "<情况语句>" << endl;
#endif
}

//void Return_statement() {//FIRST={return}
//    //sample-> return / return(a+b)
//    now_function_has_return = 1;
//    PRE_READ_NEW_WORD
//    if (TYPE == "LPARENT") {
//        PRINT_PRE_WORD
//        READ_NEW_WORD
//        if (TYPE != "RPARENT") {//return (dsad...
//            //调用表达式
//            expression subexpression;
//            if (CALL_Expression) {
//                subexpression.subroutine();
//                expressionValue = subexpression.valueType;
//            } else {
//                error();
//            }//调用表达式
//            if (expressionValue == 0) {//char{
//                if (now_function_type == NONE_FUCTION)file_error << lastSymbolNum << " g" << endl;
//                if (now_function_type == INT_FUNCTION)file_error << lastSymbolNum << " h" << endl;
//
//            } else {
//                if (now_function_type == NONE_FUCTION)
//                    if (now_function_type == NONE_FUCTION)
//                        file_error << lastSymbolNum << " g" << endl;
//                if (now_function_type == CHAR_FUNCTION)file_error << lastSymbolNum << " h" << endl;
//            }
//        } else {//return ();
//            if (now_function_type == INT_FUNCTION || now_function_type == CHAR_FUNCTION)
//                file_error << lastSymbolNum << " h" << endl;
//        }
//
//        if (TYPE == "RPARENT") {//return ();
//            PRINT_PRE_WORD
//            PRE_READ_NEW_WORD
//        } else file_error << lastSymbolNum << " l" << endl;
//
//    } else {//return ;
//        if (now_function_type == INT_FUNCTION || now_function_type == CHAR_FUNCTION)
//            file_error << lastSymbolNum << " h" << endl;
//    }
//
//#ifdef TURN_ON
//    file_out << "<返回语句>" << endl;
//#endif
//}

//some small sub program--------------------------------
//代码生成: 生成四元式并返回标签
void Condition() {//FIRST = {+/-/IDENFR/"/'/(}
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
    if (TYPE == "LSS" || TYPE == "LEQ" || TYPE == "GRE" || TYPE == "GEQ" || TYPE == "NEQ" || TYPE == "EQL") {
        PRINT_PRE_WORD
        //跳转四元式——操作类型
        if (TYPE == "LSS") {
            branch.op = O_blt;
        } else if (TYPE == "LEQ") {
            branch.op = O_ble;
        } else if (TYPE == "GRE") {
            branch.op = O_bgt;
        } else if (TYPE == "GEQ") {
            branch.op = O_bge;
        } else if (TYPE == "NEQ") {
            branch.op = O_bne;
        } else if (TYPE == "EQL") {
            branch.op = O_beq;
        } else cout << "error in: grammer->condition" << endl;
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


    //跳转四元式——注释

    int value2 = expressionValue;
    if (value1 == 0 || value2 == 0) {
        file_error << lastSymbolNum << " f" << endl;
    }


#ifdef TURN_ON
    file_out << "<条件>" << endl;
#endif
}

void Foot() {//FIRST{整型常量}
    //调用无符号整数
    CALL_Nosymbol_constant_integer

#ifdef TURN_ON
    file_out << "<步长>" << endl;
#endif
}

void Value_parameter_table(function_line &line) {//FIRST={+/-/IDENFR/(/整型常量/'}  注意!!!这个可能为空
    //sample->
    int parameter_position = 0;
    //fix bug:参数表容器
    vector<expression> parameters_result_tables;

    //调用表达式
    expression subexpression;
    if (CALL_Expression) {
        subexpression.subroutine();
        expressionValue = subexpression.valueType;
    } else {
        error();
    }

    //加入参数容器
    parameters_result_tables.push_back(subexpression);

    if (!(parameter_position >= line.parameter_type.size() ||
          line.parameter_type.at(parameter_position) == "CHARTK" && expressionValue == 0 ||
          line.parameter_type.at(parameter_position) == "INTTK" && expressionValue == 1)) {
        file_error << lastSymbolNum << " e" << endl;
    }

    while (TYPE == "COMMA") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        //调用表达式
        expression subexpression;
        if (CALL_Expression) {
            subexpression.subroutine();
            expressionValue = subexpression.valueType;
        } else {
            error();
        }

        //加入参数表容器
        parameters_result_tables.push_back(subexpression);

//        //四元式-传递参数
//        fourOp give_parameters;
//        //(1)操作数
//        give_parameters.op = O_GiveParameters;
//        //(2)存储空间
//        give_parameters.item1.value = save_position + now_function_line.size;//加上空间更改后的大小
//        save_position +=4;
//        //(3)存储内容
//        give_parameters.item2 = subexpression.op;
//        only_block.fourOpList.push_back(give_parameters);

        //语法分析
        parameter_position++;
        if (!(parameter_position >= line.parameter_type.size() ||
              line.parameter_type.at(parameter_position) == "CHARTK" && expressionValue == 0 ||
              line.parameter_type.at(parameter_position) == "INTTK" && expressionValue == 1)) {
            file_error << lastSymbolNum << " e" << endl;
        }
    }

    if (parameter_position != line.parameter_type.size() - 1) {
        file_error << lastSymbolNum << " d" << endl;
    }

    //统一处理参数分配情况
    int save_position = 8;
    for (int i = 0; i < parameters_result_tables.size(); i++) {
        expression subexpression = parameters_result_tables.at(i);
        //1.传递参数,从空间8开始,加上函数块内存偏移
        //四元式-传递参数
        fourOp give_parameters;
        //(1)操作数
        give_parameters.op = O_GiveParameters;
        //(2)存储空间
        give_parameters.item1.value = save_position + now_function_line.size;//加上空间更改后的大小
        save_position += 4;
        //(3)存储内容
        give_parameters.item2 = subexpression.op;
        only_block.fourOpList.push_back(give_parameters);
    }

#ifdef TURN_ON
    file_out << "<值参数表>" << endl;
#endif
}


void Case_table(int switch_expression_Type, operand expression_op, string switchEndLabel) {//FIRST={case}
    int order = 1;
    CALL_Case_sub_table
    while (TYPE == "CASETK") {
        PRINT_PRE_WORD
        order ++;
        CALL_Case_sub_table
    }

#ifdef TURN_ON
    file_out << "<情况表>" << endl;
#endif
}


void Case_sub_sentence(int switch_expression_Type, operand expression_op, string switchEndLabel,int order) {//FIRST={case}
    /***
     * 特殊化处理:第一个情况子语句负责保存switch中表达式的值
     */

    //四元式-生成swicth的比较判断跳转语句
    fourOp compare_case;
    compare_case.op = O_bne;
    //1.item1操作数有了
    compare_case.item1 = expression_op;
    if(order == 1){
        compare_case.item1.shouldSave = true;
    }

    READ_NEW_WORD
    CALL_Constant_value
    //2.item2操作数有了
    compare_case.item2.kind = K_CONSTANT;
    compare_case.item2.value = constant_value;//char问题
    //3.产生一个label
    string next_label = "case" + to_string(branchNum++);
    compare_case.item3.label = next_label;
    //4.放入四元式
    only_block.fourOpList.push_back(compare_case);

    //语法分析-每一个case里的语句列
    if (switch_expression_Type != constant_value_type)file_error << lastSymbolNum << " o" << endl;
    if (TYPE == "COLON") PRINT_PRE_WORD else error();
    READ_NEW_WORD
    CALL_STATEMENT

    //四元式-无条件跳转至end
    fourOp jumpSwitchEnd;
    jumpSwitchEnd.op = O_j;
    jumpSwitchEnd.item1.label = switchEndLabel;
    only_block.fourOpList.push_back(jumpSwitchEnd);

    //四元式-生成case比较失败的下一个label
    fourOp putNextLabel;
    putNextLabel.op = O_putlabel;
    putNextLabel.item1.label = next_label;
    putNextLabel.notation = "#switch语句的分支,源代码在第" + to_string(symbolNum) + "行";
    only_block.fourOpList.push_back(putNextLabel);

#ifdef TURN_ON
    file_out << "<情况子语句>" << endl;
#endif
}


void Loss_default() {//FIRST={default}

    READ_NEW_WORD
    if (TYPE != "COLON")error();
    READ_NEW_WORD
    CALL_STATEMENT

#ifdef TURN_ON
    file_out << "<缺省>" << endl;
#endif
}


void Character_set() {
    PRE_READ_NEW_WORD
#ifdef TURN_ON
    file_out << "<字符串>" << endl;
#endif
}


int Constant_integer() {//FIRST={+/-/整型常量}
    bool symbolIsPlus = true;
    if (TYPE == "PLUS" || TYPE == "MINU") {
        if (TYPE == "PLUS")symbolIsPlus = true;
        else symbolIsPlus = false;
        READ_NEW_WORD
    }
    //调用无符号整数
    CALL_Nosymbol_constant_integer
    if (symbolIsPlus) return constant_integer_value;
    else return -constant_integer_value;
#ifdef TURN_ON
    file_out << "<整数>" << endl;
#endif
}

int string_transfer_integer(string alpha) {
    return stoi(alpha);
}

int Nosymbol_constant_integer() {
    int value = string_transfer_integer(SYM);
    PRE_READ_NEW_WORD
#ifdef TURN_ON
    file_out << "<无符号整数>" << endl;
#endif
    return value;
}


int Constant_value() {//FIRST={+/-/整型常量/字符常量}
    int type = 0;
    if (TYPE == "CHARCON") {
        type = 0;
        constant_value = SYM[0];
        PRE_READ_NEW_WORD
    } else if (TYPE == "PLUS" || TYPE == "MINU" || TYPE == "INTCON") {
        type = 1;
        if (CALL_Constant_integer) {
            constant_value = Constant_integer();
        } else error();
    } else error();

#ifdef TURN_ON
    file_out << "<常量>" << endl;
#endif
    return type;
}
