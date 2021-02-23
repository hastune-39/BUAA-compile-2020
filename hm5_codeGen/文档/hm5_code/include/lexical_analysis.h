//
// Created by 像我这样优秀的人 on 2020/9/27.
//

#ifndef HM2_LEXICAL_ANALYSIS_H
#define HM2_LEXICAL_ANALYSIS_H

#include <iostream>
#include <map>
#include <fstream>
#include <ctype.h>
#include <string>
#include <algorithm>

#define TURN_ON
#ifdef TURN_ON

using namespace std;

extern ofstream file_out;
extern ofstream file_error;

#endif
//FILE---------------------------
static ifstream *inFile;//traget at given file
void lexicalAnalysis_readFile(ifstream* file);//pre-read a ' '
bool finish_readFile();


//ANALYSIS--------------------------
//get key word and delimeter table
static string lexical_str;
static char c;//c need pre_read a blank
static string lexical_type;
char file_getChar();//need to transfer_little
bool isLegal(char);
void skip_blank();
#define PAIR(a,b) (pair<string,string>(a,b))
static map<string,string> KeyWord={PAIR("const","CONSTTK"),
                                   PAIR("int","INTTK"),
                                   PAIR("char",	"CHARTK"),
                                   PAIR("void"	,"VOIDTK"),
                                   PAIR("main",	"MAINTK"),
                                   PAIR("if","IFTK"),
                                   PAIR("else",	"ELSETK"),
                                   PAIR("switch",	"SWITCHTK"),
                                   PAIR("case",	"CASETK"),
                                   PAIR("default",	"DEFAULTTK"),
                                   PAIR("while",	"WHILETK"),
                                   PAIR("for",	"FORTK"),
                                   PAIR("scanf",	"SCANFTK"	),
                                   PAIR("printf",	"PRINTFTK"),
                                   PAIR("return"	,"RETURNTK")};
static map<string,string> Delimiter={PAIR("+","PLUS"),
                                     PAIR("-",	"MINU"),
                                     PAIR("*",	"MULT"),
                                     PAIR(	"/",	"DIV"),
                                     PAIR("<",	"LSS"),
                                     PAIR("<=",	"LEQ"),
                                     PAIR(">",	"GRE"),
                                     PAIR(">=",	"GEQ"),
                                     PAIR("==",	"EQL"),
                                     PAIR("!=",	"NEQ"),
                                     PAIR(":",	"COLON"	),
                                     PAIR("=","ASSIGN"),
                                     PAIR(	";",	"SEMICN"),
                                     PAIR(	",",	"COMMA"),
                                     PAIR(	"(",	"LPARENT"),
                                     PAIR(	")",	"RPARENT"),
                                     PAIR(	"[","LBRACK"),
                                     PAIR(	"]",	"RBRACK"),
                                     PAIR("{","LBRACE"),
                                     PAIR("}","RBRACE")};
bool belongTo_Keyword(string word);
bool belongTo_Delimiter(string word);
pair<string, string> get_a_word();//non return NULL

//WRONG-------------------------
void wrong_handle();

#endif //HM2_LEXICAL_ANALYSIS_H
