//
// Created by 像我这样优秀的人 on 2020/9/30.
//

#include "grammer_analysis.h"

extern ofstream file_out;

#define PRE_READ_NEW_WORD_FROM_LEX get_data(get_a_word());
#define PRE_READ_NEW_WORD_FROM_ALL {if(BACKSTRACK.empty()) \
{PRE_READ_NEW_WORD_FROM_LEX} else \
{get_data(BACKSTRACK.front()); BACKSTRACK.pop();}}
#define READ_NEW_WORD {if(BACKSTRACK.empty()) \
{PRE_READ_NEW_WORD_FROM_LEX} else \
{get_data(BACKSTRACK.front()); BACKSTRACK.pop();} \
file_out<<TYPE<<' '<<SYM<<endl;}
#define PRINT_PRE_WORD {file_out<<TYPE<<' '<<SYM<<endl;}

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
    PRE_READ_NEW_WORD_FROM_LEX
    program();
}

void grammer_initialize() {
    TYPE.clear();
    SYM.clear();
    while (!BACKSTRACK.empty())BACKSTRACK.pop();

}

//<program>
void program() {
    if (TYPE == "CONSTTK") {
        PRINT_PRE_WORD
        constant_description();
    }

    if (TYPE == "CHARTK" || TYPE == "INTTK"){//规则改变
        program_cycle_Variable_Function();
    }

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
            PRE_READ_NEW_WORD_FROM_LEX
            if (TYPE == "IDENFR") {
                BACKSTRACK.push(PAIR(TYPE, SYM));
                PRE_READ_NEW_WORD_FROM_LEX
                if (TYPE == "ASSIGN" || TYPE == "COMMA" || TYPE == "SEMICN" || TYPE == "LBRACK") {
                    BACKSTRACK.push(PAIR(TYPE, SYM));
                    READ_NEW_WORD
                    //变量说明定义
                    variable_description_key = true;
                    //变量定义
                    Variable_define();
                    if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
                    PRE_READ_NEW_WORD_FROM_LEX
                } else if (TYPE == "LPARENT") {
                    BACKSTRACK.push(PAIR(TYPE, SYM));
                    get_data(BACKSTRACK.front()); BACKSTRACK.pop();
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
            PRE_READ_NEW_WORD_FROM_LEX
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
    if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD_FROM_LEX
    while (TYPE == "CONSTTK") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Constant_define
        if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    }

#ifdef TURN_ON
    file_out << "<常量说明>" << endl;
#endif
}

#define CALL_Constant_define {if(TYPE=="INTTK"||TYPE=="CHARTK")Constant_define();else error();}

void Constant_define() {//FIRST={INT/CHAR}
    if (TYPE == "INTTK") {
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        CALL_Constant_integer
        while (TYPE == "COMMA") {
            PRINT_PRE_WORD
            READ_NEW_WORD
            if (TYPE != "IDENFR")error();
            READ_NEW_WORD
            if (TYPE != "ASSIGN")error();
            READ_NEW_WORD
            CALL_Constant_integer
        }
    } else if (TYPE == "CHARTK") {
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        if (TYPE != "CHARCON")error();
        PRE_READ_NEW_WORD_FROM_LEX
        while (TYPE == "COMMA") {
            PRINT_PRE_WORD
            READ_NEW_WORD
            if (TYPE != "IDENFR")error();
            READ_NEW_WORD
            if (TYPE != "ASSIGN")error();
            READ_NEW_WORD
            if (TYPE != "CHARCON")error();
            PRE_READ_NEW_WORD_FROM_LEX
        }
    }

#ifdef TURN_ON
    file_out << "<常量定义>" << endl;
#endif
}

//<variable description>--------------------------------------------------
void variable_description() {//FIRST={INT/CHAR}
    Variable_define();
    if(TYPE=="SEMICN")PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD_FROM_ALL
    while (TYPE == "CHARTK" || TYPE == "INTTK") {
        PRINT_PRE_WORD
        Variable_define();
        if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    }

#ifdef TURN_ON
    file_out << "<变量说明>" << endl;
#endif
}

void Variable_define() {//FIRST = {INT/CHAR}
    READ_NEW_WORD
    variable();

    if (TYPE == "SEMICN") {
#ifdef TURN_ON
        file_out << "<变量定义无初始化>" << endl;
#endif
    } else if (TYPE == "COMMA") {
        Variable_define_no_initial();
    } else if (TYPE == "ASSIGN") Variable_define_with_initial();
    else error();

#ifdef TURN_ON
    file_out << "<变量定义>" << endl;
#endif
}

void Variable_define_no_initial() {//FIRST={,}
    while (TYPE == "COMMA") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        variable();
    }

#ifdef TURN_ON
    file_out << "<变量定义无初始化>" << endl;
#endif
}

void Variable_define_with_initial() {//FIRST={=} 这里有大问题！
    PRINT_PRE_WORD
    PRE_READ_NEW_WORD_FROM_LEX
    if(TYPE=="LBRACE"){
        PRINT_PRE_WORD
        PRE_READ_NEW_WORD_FROM_LEX
        if(TYPE=="LBRACE"){
            PRINT_PRE_WORD
            READ_NEW_WORD CALL_Constant_value
            while(TYPE=="COMMA"){
                PRINT_PRE_WORD
                READ_NEW_WORD CALL_Constant_value
            }
            if(TYPE=="RBRACE")PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
            while(TYPE=="COMMA"){
                PRINT_PRE_WORD
                READ_NEW_WORD if(TYPE!="LBRACE")error();
                READ_NEW_WORD CALL_Constant_value
                while(TYPE=="COMMA"){
                    PRINT_PRE_WORD
                    READ_NEW_WORD CALL_Constant_value
                }
                if(TYPE=="RBRACE")PRINT_PRE_WORD else error();
                PRE_READ_NEW_WORD_FROM_LEX
            }
            if(TYPE=="RBRACE")PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
        }else{
            PRINT_PRE_WORD CALL_Constant_value
            while(TYPE=="COMMA"){
                PRINT_PRE_WORD
                READ_NEW_WORD CALL_Constant_value
            }
            if(TYPE=="RBRACE")PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
        }
    }else {
		PRINT_PRE_WORD
		CALL_Constant_value 
	}

#ifdef TURN_ON
    file_out << "<变量定义及初始化>" << endl;
#endif
}

void variable() {//FIRST={IDENFR}
    if (TYPE == "IDENFR") {
        if(!BACKSTRACK.empty()){
            get_data(BACKSTRACK.front()); BACKSTRACK.pop();//处理的不行！
        }else PRE_READ_NEW_WORD_FROM_LEX
        if (TYPE == "LBRACK") {
            PRINT_PRE_WORD
            READ_NEW_WORD
            CALL_Nosymbol_constant_integer
            if (TYPE == "RBRACK") PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
            if (TYPE == "LBRACK") {
                PRINT_PRE_WORD
                READ_NEW_WORD
                CALL_Nosymbol_constant_integer
                if (TYPE == "RBRACK") PRINT_PRE_WORD else error();
                PRE_READ_NEW_WORD_FROM_LEX
            }
        }
    } else error();
}

//function--------------------------------------------
//<function_with_return>
void function_with_return() { //FIRST = {char/int}
    Declaration_header();
    if (TYPE == "LPARENT") PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD_FROM_LEX
    if (TYPE == "RPARENT") {//空串也是参数表
#ifdef TURN_ON
        file_out << "<参数表>" << endl;
#endif
        PRINT_PRE_WORD
    } else {
        PRINT_PRE_WORD
        CALL_Parameters_table//非空串
        if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
    }

    READ_NEW_WORD
    if (TYPE != "LBRACE") error();
    PRE_READ_NEW_WORD_FROM_LEX  CALL_Compound_sentence
    if (TYPE == "RBRACE") PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD_FROM_LEX

#ifdef TURN_ON
    file_out << "<有返回值函数定义>" << endl;
#endif
}

//<function_no_return>
void function_no_return() {//FIRST={void}
    READ_NEW_WORD
    if (TYPE != "IDENFR") error();
    //特殊化符号表
    FUNC_TYPE.insert(PAIR(SYM, "NO RETURN"));
    //
    READ_NEW_WORD
    if (TYPE !=  "LPARENT") error();
    PRE_READ_NEW_WORD_FROM_LEX
    if (TYPE == "RPARENT") {//空串也是参数表
#ifdef TURN_ON
        file_out << "<参数表>" << endl;
#endif
        PRINT_PRE_WORD
    } else {
        PRINT_PRE_WORD
        CALL_Parameters_table//非空串
        if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
    }

    READ_NEW_WORD
    if (TYPE != "LBRACE") error();
    PRE_READ_NEW_WORD_FROM_LEX  CALL_Compound_sentence
    if (TYPE == "RBRACE") PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD_FROM_LEX
    
#ifdef TURN_ON
    file_out << "<无返回值函数定义>" << endl;
#endif
}

//<main_function>
void main_function() {//FIRST={void}
    READ_NEW_WORD
    if (TYPE != "MAINTK") error();
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    if (TYPE != "RPARENT")error();
    READ_NEW_WORD
    if (TYPE != "LBRACE")error();
    PRE_READ_NEW_WORD_FROM_LEX CALL_Compound_sentence
    if(TYPE=="RBRACE")PRINT_PRE_WORD else error();

#ifdef TURN_ON
    file_out << "<主函数>" << endl;
#endif
}

void Declaration_header() {//FIRST = {INT/CHAR}
    READ_NEW_WORD
    if (TYPE != "IDENFR") error();
    //特殊化符号表
    FUNC_TYPE.insert(PAIR(SYM, "WITH RETURN"));
    //
    if(!BACKSTRACK.empty()){
        get_data(BACKSTRACK.front()); BACKSTRACK.pop();
    }else PRE_READ_NEW_WORD_FROM_LEX

#ifdef TURN_ON
    file_out << "<声明头部>" << endl;
#endif
}

#define CALL_Parameters_table if(TYPE=="INTTK"||TYPE=="CHARTK") Parameters_table(); else error();

void Parameters_table() {//FIRST={CHAR/INT} 注：有空串，交给外部处理
    // sample
    //char a,int b,...;
    Type_idenfr();
    if (TYPE == "IDENFR") PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD_FROM_LEX
    while (TYPE == "COMMA") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Type_idenfr
        if (TYPE == "IDENFR") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    }

#ifdef TURN_ON
    file_out << "<参数表>" << endl;
#endif
}

#define CALL_Type_idenfr if(TYPE=="INTTK"||TYPE=="CHARTK") Type_idenfr(); else error();

void Type_idenfr() {//FIRST={INT/CHAR}

    PRE_READ_NEW_WORD_FROM_LEX
    //类型标识符不输出
}

//<Compound_sentence>----------------------------------------------------

#define CALL_Compound_sentence if(TYPE=="CONSTTK"||TYPE=="INTTK"||TYPE=="CHARTK"||(STATEMENT_CONDITION)) Compound_sentence();else error();

void Compound_sentence() {//FIRST={const/int/char/语句列/空}
    if (TYPE == "CONSTTK") {
        PRINT_PRE_WORD
        constant_description();
    }

    if (TYPE == "INTTK" || TYPE == "CHARTK") {
        PRINT_PRE_WORD
        variable_description();
    }

    //语句列 ???????????????????????????????????????????
    if (TYPE == "RBRACE") {
#ifdef TURN_ON
        file_out << "<语句列>" << endl;
#endif
    } else{
        PRINT_PRE_WORD
        CALL_STATEMENT_LIST
    }

#ifdef TURN_ON
    file_out << "<复合语句>" << endl;
#endif
}

#define CALL_STATEMENT_LIST if(STATEMENT_CONDITION) Statement_list(); else error();

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
        PRE_READ_NEW_WORD_FROM_LEX
        BACKSTRACK.push(PAIR(TYPE, SYM));
        if (TYPE == "LPARENT") {//？？区分有无函数调用
            get_data(BACKSTRACK.front()); BACKSTRACK.pop();
            if(FUNC_TYPE[SYM] == "WITH RETURN")Function_with_return_statement();//???
            else if(FUNC_TYPE[SYM] == "NO RETURN")Function_no_return_statement();
            else error();

            if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
        } else if (TYPE == "LBRACK" || TYPE == "ASSIGN") {
            get_data(BACKSTRACK.front()); BACKSTRACK.pop();
            Assign_statement();
            if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
        } else error();
    } else if (TYPE == "SCANFTK") {
        Read_statement();
        if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    } else if (TYPE == "PRINTFTK") {
        Write_statement();
        if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    } else if (TYPE == "SWITCHTK") {
        Switch_statement();
    } else if (TYPE == "SEMICN") {
        PRE_READ_NEW_WORD_FROM_LEX
    } else if (TYPE == "RETURNTK") {
        Return_statement();
        if (TYPE == "SEMICN") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    } else if (TYPE == "LBRACE") {
        PRE_READ_NEW_WORD_FROM_LEX

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
        PRE_READ_NEW_WORD_FROM_LEX
    } else error();

#ifdef TURN_ON
    file_out << "<语句>" << endl;
#endif
}

void Loop_statement() {//FIRST={while/for}
    if (TYPE == "WHILETK") {
        READ_NEW_WORD
        if (TYPE != "LPARENT")error();
        READ_NEW_WORD
        CALL_Condition
        if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
        READ_NEW_WORD
        CALL_STATEMENT
    } else if (TYPE == "FORTK") {
        //sample for(i=0;i<10;i=i+1) i++;
        READ_NEW_WORD
        if (TYPE != "LPARENT")error();
        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        CALL_Expression
        if (TYPE == "SEMICN") PRINT_PRE_WORD else error();

        READ_NEW_WORD
        CALL_Condition
        if (TYPE == "SEMICN") PRINT_PRE_WORD else error();

        READ_NEW_WORD
        if (TYPE != "IDENFR")error();
        READ_NEW_WORD
        if (TYPE != "ASSIGN")error();
        READ_NEW_WORD
        if (TYPE != "IDENFR") error();
        READ_NEW_WORD
        if (!(TYPE == "PLUS" || TYPE == "MINU"))error();
        READ_NEW_WORD
        CALL_Foot
        if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
        READ_NEW_WORD
        CALL_STATEMENT
    } else error();

#ifdef TURN_ON
    file_out << "<循环语句>" << endl;
#endif
}

void Condition_statement() {//FIRST={if}

    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    CALL_Condition//调用条件
    if (TYPE == "RPARENT") PRINT_PRE_WORD else error();

    READ_NEW_WORD
    CALL_STATEMENT//条件判断调用语句

    if (TYPE == "ELSETK") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_STATEMENT//调用语句
    }

#ifdef TURN_ON
    file_out << "<条件语句>" << endl;
#endif
}

#define Call_function_with_return_statement if(TYPE=="IDENFR") Function_with_return_statement(); else error();

void Function_with_return_statement() {//FIRST={标识符}
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    PRE_READ_NEW_WORD_FROM_ALL
    //值参数表
    if (TYPE == "RPARENT") {//空也为值参数表
#ifdef TURN_ON
        file_out << "<值参数表>" << endl;
#endif
        PRINT_PRE_WORD
        PRE_READ_NEW_WORD_FROM_LEX
    } else {
        //调用值参数表
        PRINT_PRE_WORD
        CALL_Value_parameter_table
        if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    }

#ifdef TURN_ON
    file_out << "<有返回值函数调用语句>" << endl;
#endif
}

void Function_no_return_statement() {//FIRST={IDENFR}
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    PRE_READ_NEW_WORD_FROM_LEX
    //值参数表
    if (TYPE == "RPARENT") {//空也为值参数表
#ifdef TURN_ON
        file_out << "<值参数表>" << endl;
#endif
        PRINT_PRE_WORD
        PRE_READ_NEW_WORD_FROM_LEX
    } else {
        //调用值参数表
        PRINT_PRE_WORD
        CALL_Value_parameter_table
        if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    }

#ifdef TURN_ON
    file_out << "<无返回值函数调用语句>" << endl;
#endif
}

void Assign_statement() {//FIRST{标识符} SECOND{{/=}
    //sample -> a[10] = 'b';
    PRE_READ_NEW_WORD_FROM_ALL
    if (TYPE == "ASSIGN") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Expression//调用表达式
    } else if (TYPE == "LBRACK") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Expression//调用表达式
        if (TYPE == "RBRACK") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
        if (TYPE == "LBRACK") {
            PRINT_PRE_WORD
            READ_NEW_WORD
            CALL_Expression//调用表达式
            if (TYPE == "RBRACK") PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
        }

        if (TYPE == "ASSIGN") PRINT_PRE_WORD else error();

        READ_NEW_WORD
        CALL_Expression//调用表达式
    } else error();

#ifdef TURN_ON
    file_out << "<赋值语句>" << endl;
#endif
}

void Read_statement() {
    //sample-> scanf(s1);
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    if (TYPE != "IDENFR")error();
    READ_NEW_WORD
    if (TYPE != "RPARENT")error();
    PRE_READ_NEW_WORD_FROM_LEX

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
        CALL_Character_set
        if (TYPE == "COMMA") {
            PRINT_PRE_WORD
            READ_NEW_WORD
            CALL_Expression
        } else if (TYPE == "RPARENT") {
        } else error();
    } else CALL_Expression

    if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
    PRE_READ_NEW_WORD_FROM_LEX

#ifdef TURN_ON
    file_out << "<写语句>" << endl;
#endif
}

void Switch_statement() {//first={swicth}
    //sample-> swicth(identify){case a+b:b+d; default;}
    READ_NEW_WORD
    if (TYPE != "LPARENT")error();
    READ_NEW_WORD
    CALL_Expression//调用表达式
    if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
    READ_NEW_WORD
    if (TYPE != "LBRACE")error();
    READ_NEW_WORD
    CALL_Case_table//情况表调用
    PRINT_PRE_WORD
    CALL_Loss_default//缺省调用
    if (TYPE == "RBRACE") PRINT_PRE_WORD else error();

    PRE_READ_NEW_WORD_FROM_LEX
#ifdef TURN_ON
    file_out << "<情况语句>" << endl;
#endif
}

void Return_statement() {//FIRST={return}
    //sample-> return / return(a+b)
    PRE_READ_NEW_WORD_FROM_LEX
    if (TYPE == "LPARENT") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Expression//调用表达式
        if (TYPE == "RPARENT") PRINT_PRE_WORD else error();
        PRE_READ_NEW_WORD_FROM_LEX
    }

#ifdef TURN_ON
    file_out << "<返回语句>" << endl;
#endif
}

//some small sub program--------------------------------

#define CALL_Condition if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE == "CHARCON") Condition(); else error();
void Condition() {//FIRST = {+/-/IDENFR/"/'/(}
    //sample (a+b)==c+d
    //调用表达式
    CALL_Expression
    if (TYPE == "LSS" || TYPE == "LEQ" || TYPE == "GRE" || TYPE == "GEQ" || TYPE == "NEQ" || TYPE == "EQL")PRINT_PRE_WORD else error();
    READ_NEW_WORD
    CALL_Expression//调用表达式

#ifdef TURN_ON
    file_out << "<条件>" << endl;
#endif
}

#define CALL_Foot if(TYPE=="INTCON") Foot(); else error();

void Foot() {//FIRST{整型常量}
    //调用无符号整数
    CALL_Nosymbol_constant_integer

#ifdef TURN_ON
    file_out << "<步长>" << endl;
#endif
}

#define CALL_Value_parameter_table if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE =="CHARCON") Value_parameter_table(); else error();
void Value_parameter_table() {//FIRST={+/-/IDENFR/(/整型常量/'}  注意!!!这个可能为空
    //sample->
    CALL_Expression
    while (TYPE == "COMMA") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        CALL_Expression
    }

#ifdef TURN_ON
    file_out << "<值参数表>" << endl;
#endif
}

#define CALL_Case_table {if(TYPE=="CASETK") Case_table();else error();}

void Case_table() {//FIRST={case}
    CALL_Case_sub_table
    while (TYPE == "CASETK") {
        PRINT_PRE_WORD
        CALL_Case_sub_table
    }

#ifdef TURN_ON
    file_out << "<情况表>" << endl;
#endif
}

#define CALL_Case_sub_table {if(TYPE=="CASETK") Case_sub_sentence();else error();}

void Case_sub_sentence() {//FIRST={case}

    READ_NEW_WORD
    CALL_Constant_value
    if (TYPE == "COLON") PRINT_PRE_WORD else error();
    READ_NEW_WORD
    CALL_STATEMENT

#ifdef TURN_ON
    file_out << "<情况子语句>" << endl;
#endif
}

#define CALL_Loss_default {if(TYPE=="DEFAULTTK") Loss_default();else error();}

void Loss_default() {//FIRST={default}

    READ_NEW_WORD
    if (TYPE != "COLON")error();
    READ_NEW_WORD
    CALL_STATEMENT

#ifdef TURN_ON
    file_out << "<缺省>" << endl;
#endif
}

#define CALL_Expression {if(TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Expression(); else error();}

void Expression() {//FIRST = {+/-/IDENFR/"/'/(/数字常量}
    //sample-> (((1))*a+b)
    if (TYPE == "PLUS" || TYPE == "MINU") READ_NEW_WORD
    //调用项
    CALL_Item
    while (TYPE == "PLUS" || TYPE == "MINU") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        //调用项
        CALL_Item
    }

#ifdef TURN_ON
    file_out << "<表达式>" << endl;
#endif
}

#define CALL_Item {if(TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Item(); else error();}

void Item() {//FIRST={IDENFR/(/+/-/整型常量/'}
    //sample-> (a+b)*5
    //调用因子
    CALL_Factor
    while (TYPE == "MULT" || TYPE == "DIV") {
        PRINT_PRE_WORD
        READ_NEW_WORD
        //调用因子
        CALL_Factor
    }

#ifdef TURN_ON
    file_out << "<项>" << endl;
#endif
}

#define CALL_Factor {if(TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Factor(); else error();}

void Factor() {//FIRST={IDENFR/(/+/-/数字常量/'}

    if (TYPE == "IDENFR") {
        //sample-> s1/f1(a,b,c)
        BACKSTRACK.push(PAIR(TYPE, SYM));
        PRE_READ_NEW_WORD_FROM_LEX
        BACKSTRACK.push(PAIR(TYPE, SYM));
        if (TYPE == "LPARENT") {
            PRE_READ_NEW_WORD_FROM_ALL
            //调用有返回值函数语句
            Call_function_with_return_statement
        }else if(TYPE=="LBRACK"){
            PRE_READ_NEW_WORD_FROM_ALL
            READ_NEW_WORD
            READ_NEW_WORD CALL_Expression
            if(TYPE=="RBRACK")PRINT_PRE_WORD else error();
            PRE_READ_NEW_WORD_FROM_LEX
            if(TYPE=="LBRACK") {
                PRINT_PRE_WORD
                READ_NEW_WORD CALL_Expression
                if(TYPE=="RBRACK")PRINT_PRE_WORD else error();
                PRE_READ_NEW_WORD_FROM_LEX
            }
        }else {
            PRE_READ_NEW_WORD_FROM_ALL
            PRE_READ_NEW_WORD_FROM_ALL
        }
    } else if (TYPE == "LPARENT") {
        READ_NEW_WORD
        //调用表达式
        CALL_Expression
        if (TYPE != "RPARENT")error();
        PRINT_PRE_WORD
        PRE_READ_NEW_WORD_FROM_LEX
    } else if (TYPE == "INTCON" || TYPE == "PLUS" || TYPE == "MINU") {
        //调用整数
        CALL_Constant_integer
    } else if (TYPE == "CHARCON") {
        PRE_READ_NEW_WORD_FROM_LEX
    } else error();

#ifdef TURN_ON
    file_out << "<因子>" << endl;
#endif
}

#define CALL_Character_set if(TYPE == "STRCON") Character_set(); else error();

void Character_set() {
    PRE_READ_NEW_WORD_FROM_LEX
#ifdef TURN_ON
    file_out << "<字符串>" << endl;
#endif
}

#define CALL_Constant_integer {if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="INTCON") Constant_integer(); else error();}

void Constant_integer() {//FIRST={+/-/整型常量}
    if (TYPE=="PLUS"||TYPE=="MINU") READ_NEW_WORD
    //调用无符号整数
    CALL_Nosymbol_constant_integer

#ifdef TURN_ON
    file_out << "<整数>" << endl;
#endif
}


void Nosymbol_constant_integer() {

    PRE_READ_NEW_WORD_FROM_LEX
#ifdef TURN_ON
    file_out << "<无符号整数>" << endl;
#endif
}

#define CALL_Constant_value {if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="INTCON"||TYPE=="CHARCON") Constant_value(); else error();}

void Constant_value() {//FIRST={+/-/整型常量/字符常量}

    if (TYPE == "CHARCON") {
        PRE_READ_NEW_WORD_FROM_LEX
    } else if (TYPE=="PLUS"||TYPE=="MINU" || TYPE == "INTCON") {
        CALL_Constant_integer
    } else error();

#ifdef TURN_ON
    file_out << "<常量>" << endl;
#endif
}

void error(){
    throw "wrong";
}