//
// Created by 像我这样优秀的人 on 2020/9/30.
//

#ifndef HM3_GRAMMER_ANALYSIS_H
#define HM3_GRAMMER_ANALYSIS_H
#include <string>
#include <iostream>
#include <queue>
#include <map>

#include "lexical_analysis.h"
#include "SymbolTable.h"
#include "../middle/Operand.h"
using std::string;
using std::pair;
using std::queue;
#define PRE_READ_NEW_WORD  get_data(get_a_word());

#define PRE_READ_NEW_WORD_FROM_ALL {if(BACKSTRACK.empty()) \
{PRE_READ_NEW_WORD} else \
{get_data(BACKSTRACK.front()); BACKSTRACK.pop();}}
#define READ_NEW_WORD {if(BACKSTRACK.empty()) \
{PRE_READ_NEW_WORD} else \
{get_data(BACKSTRACK.front()); BACKSTRACK.pop();} \
file_out<<TYPE<<' '<<SYM<<endl;}
#define PRINT_PRE_WORD {file_out<<TYPE<<' '<<SYM<<endl;}

void grammer_analysis_RUN();//
void grammer_initialize();


//Global dynamic data


void get_data(pair<string,string> word);


//sub program
void program();
void constant_description();
void variable_description();
void function_with_return();
void function_no_return();
void main_function();

//sub <constant_description>
#define CALL_Constant_define {if(TYPE=="INTTK"||TYPE=="CHARTK")Constant_define();else error();}
void Constant_define();

//sub <variable_description>
void Variable_define(variable_line& line);
void Variable_define_with_initial(variable_line& line);
void Variable_define_no_initial(variable_line& first);
void variable(variable_line& line);


//sub <function_with_return> and <function_no_return>
void Declaration_header(function_line& line);
#define CALL_Parameters_table if(TYPE=="INTTK"||TYPE=="CHARTK") Parameters_table(line); else error();
void Parameters_table(function_line& line);//FIRST={CHAR/INT} 注：有空串，交给外部处理
#define CALL_Type_idenfr if(TYPE=="INTTK"||TYPE=="CHARTK") Type_idenfr(line); else error();
void Type_idenfr(function_line& line);//FIRST={INT/CHAR}


//sentence
#define CALL_Compound_sentence if(TYPE=="CONSTTK"||TYPE=="INTTK"||TYPE=="CHARTK"||(STATEMENT_CONDITION)||TYPE=="RBRACE") Compound_sentence(unsavedSize_functionLine);else error();
void Compound_sentence(function_line &unsavedSize_functionLine);//FIRST={const/int/char/语句列}
#define STATEMENT_CONDITION (TYPE == "WHILETK" || TYPE == "FORTK" || TYPE == "IFTK"  \
|| TYPE == "IDENFR"|| TYPE == "SCANFTK" || TYPE == "PRINTFTK"  \
|| TYPE == "SWITCHTK" || TYPE == "RETURNTK" || TYPE == "LBRACE" || TYPE=="SEMICN")
#define CALL_STATEMENT_LIST if(STATEMENT_CONDITION) Statement_list(); else error();
void Statement_list();
#define CALL_STATEMENT if(STATEMENT_CONDITION) Statement(); else error();
void Statement();
void Loop_statement();
void Condition_statement();
#define Call_function_with_return_statement if(TYPE=="IDENFR") Function_with_return_statement(); else error();
//void Function_statement();
void  Function_with_return_statement();//FIRST={标识符}
void  Function_no_return_statement();
void Assign_statement();
void Read_statement();
void Write_statement();
void Switch_statement();
//void Return_statement();

//some small sub program
#define CALL_Condition if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE == "CHARCON") Condition(); else error();
void Condition();//FIRST = {+/-/IDENFR/"/'/(}
#define CALL_Foot if(TYPE=="INTCON") Foot(); else error();
void Foot();//FIRST{整型常量}
#define CALL_Value_parameter_table if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Value_parameter_table(line); else file_error<<lastSymbolNum<<" l"<<endl;
void Value_parameter_table(function_line& line);//FIRST={+/-/IDENFR/(/整型常量/'}  注意!!!这个可能为空

#define CALL_Case_table {if(TYPE=="CASETK") Case_table(switchType, subexpression.op);else error();}
void Case_table(int switch_expression_Type, operand expression_op, string switchEndLabel);//FIRST={case}
#define CALL_Case_sub_table {if(TYPE=="CASETK") Case_sub_sentence(switch_expression_Type, expression_op, switchEndLabel);else error();}
void Case_sub_sentence(int switch_expression_Type, operand expression_op, string switchEndLabel);//FIRST={case}
#define CALL_Loss_default {if(TYPE=="DEFAULTTK") Loss_default();else file_error<<symbolNum<<" p"<<endl;}
void Loss_default();//FIRST={default}


#define CALL_Expression (TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON")

#define CALL_Item (TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON")

#define CALL_Factor (TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON")


#define CALL_Character_set if(TYPE == "STRCON") Character_set(); else error();
void Character_set();
#define CALL_Constant_integer (TYPE=="PLUS"||TYPE=="MINU"||TYPE=="INTCON")
int Constant_integer();//FIRST={+/-/整型常量}
#define CALL_Constant_value {if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="INTCON"||TYPE=="CHARCON") constant_value_type=Constant_value(); else error();}
int Constant_value();//FIRST={+/-/整型常量/字符常量}
#define CALL_Nosymbol_constant_integer {if(TYPE=="INTCON") constant_integer_value=Nosymbol_constant_integer(); else error();}
int Nosymbol_constant_integer();//FIRST={整型常量}






#endif //HM3_GRAMMER_ANALYSIS_H
