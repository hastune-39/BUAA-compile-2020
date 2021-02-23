//
// Created by 像我这样优秀的人 on 2020/10/24.
//

#include "include/SymbolTable.h"
#include <iostream>
#include <ostream>
#include "fstream"

using std::cout;
using std::ofstream;
using std::ostream;
using std::endl;
//fill symbolLine in the specify table
//1.if there is no table, create
//2.check if conflict in the nowLevel,all kinds
//3.fill the symbolLine
string lower_word(string word);

extern ofstream file_error;
extern int lastSymbolNum;
extern int nowLevel;
map<int, symbolTable> Tables;
void set_table_symbol(int tableLevel, int kind, symbolLine &line) {
    int checkLevel = tableLevel - 1;
    while (checkLevel >= 0) {
        if (Tables.find(checkLevel) == Tables.end()) {
            cout << "skip tables!";
        }
        checkLevel--;
    }

    if (Tables.find(tableLevel) == Tables.end()) {
        Tables.insert(pair<int, symbolTable>(tableLevel, symbolTable()));
    }

    if (Tables.at(tableLevel).has_Line(lower_word(line.name))) {
        //error: throw Name_redefine();
        file_error << lastSymbolNum << " b" << endl;
    } else {
        line.name = lower_word(line.name);
        switch (kind) {
            case K_CONSTANT:
                Tables.at(tableLevel).insert_constantLine(line);
                break;
            case K_VARIABLE:
                Tables.at(tableLevel).insert_variableLine(line);
                break;
            case K_FUNCTION:
                Tables.at(tableLevel).insert_functionLine(line);
            default:;
        }

    }
}

bool check_table_symbol(string name) {
    int searchLevel = nowLevel;
    while (searchLevel >= 0) {
        if (Tables.find(searchLevel)!=Tables.end()&&Tables.at(searchLevel).has_Line(lower_word(name))) {
            return true;
        }
        searchLevel--;
    }
    if (searchLevel < 0) {
        //error:throw Name_undefine();
        file_error << lastSymbolNum << " c" << endl;
    }
    return false;
}

//used when identifr use
symbolLine & get_table_symbol(string name) {
    int searchLevel = nowLevel;
    while (searchLevel >= 0) {
        if (Tables.find(searchLevel)!=Tables.end()&&Tables.at(searchLevel).has_Line(lower_word(name))) {
            return Tables.at(searchLevel).get_Line(lower_word(name));
        }
        searchLevel--;
    }
//    if (searchLevel < 0) {
//        //error:throw Name_undefine();
//        file_error<<lastSymbolNum<<"c"<<endl;
//    }
//conflict with check_table_symbol
    variable_line* temp = new variable_line;
    (*temp).type=T_INT;(*temp).kind=K_VARIABLE;(*temp).dimension=0;
    return *temp;
}

void create_table(int tableLevel){
    Tables.insert(pair<int, symbolTable>(tableLevel, symbolTable()));
}

void quit_table(int tableLevel) {
    try {
        Tables.erase(tableLevel);
    } catch (exception e) {
        cout << "drop unexisted table!";
    }

}
