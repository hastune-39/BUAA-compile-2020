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
using std::string;
using std::pair;
using std::queue;

void grammer_analysis_RUN();//
void grammer_initialize();


//Global dynamic data
static string SYM;
static string TYPE;
static queue<pair<string,string>> BACKSTRACK;
static map<string, string> FUNC_TYPE;

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
void Variable_define();
void Variable_define_with_initial();
void Variable_define_no_initial();
void variable();


//sub <function_with_return> and <function_no_return>
void Declaration_header();
#define CALL_Parameters_table if(TYPE=="INTTK"||TYPE=="CHARTK") Parameters_table(); else error();
void Parameters_table();//FIRST={CHAR/INT} 注：有空串，交给外部处理
#define CALL_Type_idenfr if(TYPE=="INTTK"||TYPE=="CHARTK") Type_idenfr(); else error();
void Type_idenfr();//FIRST={INT/CHAR}


//sentence
#define CALL_Compound_sentence if(TYPE=="CONSTTK"||TYPE=="INTTK"||TYPE=="CHARTK"||(STATEMENT_CONDITION)||TYPE=="RBRACE") Compound_sentence();else error();
void Compound_sentence();//FIRST={const/int/char/语句列}
void Compound_sentence_without_return();
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
void Function_with_return_statement();//FIRST={标识符}
void Function_no_return_statement();
void Assign_statement();
void Read_statement();
void Write_statement();
void Switch_statement();
void Return_statement();

//some small sub program
#define CALL_Condition if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE == "CHARCON") Condition(); else error();
void Condition();//FIRST = {+/-/IDENFR/"/'/(}
#define CALL_Foot if(TYPE=="INTCON") Foot(); else error();
void Foot();//FIRST{整型常量}
#define CALL_Value_parameter_table if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Value_parameter_table(); else error();
void Value_parameter_table();//FIRST={+/-/IDENFR/(/整型常量/'}  注意!!!这个可能为空

#define CALL_Case_table {if(TYPE=="CASETK") Case_table();else error();}
void Case_table();//FIRST={case}
#define CALL_Case_sub_table {if(TYPE=="CASETK") Case_sub_sentence();else error();}
void Case_sub_sentence();//FIRST={case}
#define CALL_Loss_default {if(TYPE=="DEFAULTTK") Loss_default();else error();}
void Loss_default();//FIRST={default}


#define CALL_Expression {if(TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Expression(); else error();}
void Expression();//FIRST = {+/-/IDENFR/"/'/(/数字常量}
#define CALL_Item {if(TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Item(); else error();}
void Item();//FIRST={IDENFR/(/+/-/整型常量/'}
#define CALL_Factor {if(TYPE=="PLUS"||TYPE == "MINU"||TYPE=="IDENFR" \
||TYPE=="LPARENT"||TYPE=="INTCON"||TYPE=="CHARCON") Factor(); else error();}
void Factor();//FIRST={IDENFR/(/+/-/数字常量/'}

#define CALL_Character_set if(TYPE == "STRCON") Character_set(); else error();
void Character_set();
#define CALL_Constant_integer {if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="INTCON") Constant_integer(); else error();}
void Constant_integer();//FIRST={+/-/整型常量}
#define CALL_Constant_value {if(TYPE=="PLUS"||TYPE=="MINU"||TYPE=="INTCON"||TYPE=="CHARCON") Constant_value(); else error();}
void Constant_value();//FIRST={+/-/整型常量/字符常量}
#define CALL_Nosymbol_constant_integer {if(TYPE=="INTCON") Nosymbol_constant_integer(); else error();}
void Nosymbol_constant_integer();//FIRST={整型常量}
//wrong
void error();






#endif //HM3_GRAMMER_ANALYSIS_H
