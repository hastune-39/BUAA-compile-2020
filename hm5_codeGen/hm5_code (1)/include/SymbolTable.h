//
// Created by 像我这样优秀的人 on 2020/10/22.
//

#ifndef HM3_SYMBOLTABLE_H
#define HM3_SYMBOLTABLE_H

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "Error.h"
#include "AddressSpace.h"

using std::string;
using std::map;
using std::vector;
using std::pair;

#include "Enum.h"

//global
static int variable_address = varBase;

/**
 * symbolLine
 */
class symbolLine {
public:
    string name;
    string origin_name;
    int kind;
    int type;
    bool global = true;

    symbolLine() {
    }

    virtual void show() {}
};

//there ars different element classes extends symbolLine
class constant_line : public symbolLine {
public:
    int value;
    constant_line() : symbolLine() {
    }
};

class variable_line : public symbolLine {
public:
    int address;
    int size;
    int dimension;
    int dimension1;
    int dimension2;

    int sp_offset;//函数定义限定
    variable_line() : symbolLine() {
    }
};

class function_line : public symbolLine {
public:
    bool has_return;
    vector<string> parameter_type;
    vector<string> parameter_name;

    int size;

    function_line() : symbolLine() {
    }
};

/**
 * table
 */
class symbolTable {
private:
    //there are many different kind map containers
    map<string, constant_line> constantLines;
    map<string, variable_line> variableLines;
    map<string, function_line> functionLines;
public:
    //insert methods
    void insert_constantLine(symbolLine &line) {
        constant_line unsaved = dynamic_cast<constant_line &> (line);
        constantLines.insert(pair<string, constant_line>(line.name, unsaved));
    }

    void insert_variableLine(symbolLine &line) {
        variable_line unsaved = dynamic_cast<variable_line &>(line);
        variableLines.insert(pair<string, variable_line>(line.name, unsaved));
    }

    void insert_functionLine(symbolLine &line) {
        function_line unsaved = dynamic_cast<function_line &>(line);
        functionLines.insert(pair<string, function_line>(line.name, unsaved));
    }

    //delete methods (not use)

    //judge if has symbolLine in all kinds
    bool has_Line(string name) {
        if (constantLines.find(name) != constantLines.end()) return true;
        if (variableLines.find(name) != variableLines.end()) return true;
        if (functionLines.find(name) != functionLines.end()) return true;
        /**
         * if(** .find(name)!=constantLines.end())
            return true;
            ...
         */


        return false;
    }

    //get methods (needs check first)
    symbolLine &get_Line(string name) {
        if (has_Line(name)) {
            if (constantLines.find(name) != constantLines.end()) {
                symbolLine &back = constantLines.at(name);
                return back;
            }
            if (variableLines.find(name) != variableLines.end()) {
                symbolLine &back = variableLines.at(name);
                return back;
            }
            if (functionLines.find(name) != functionLines.end()) {
                symbolLine &back = functionLines.at(name);
                return back;
            }
            /**
             * if(** .find(name)!=constantLines.end())
                return **.at(name);
                ...
             */
        } else throw Name_undefine();
        symbolLine &back = constantLines.at(name);
        return back;
    }
};




//some functions
void set_table_symbol(int tableLevel, int kind, symbolLine &line);//fill the table
bool check_table_symbol(string name);
extern map<int, symbolTable> Tables;
symbolLine &get_table_symbol(string name);//when find symbol
void create_table(int tableLevel);
void quit_table(int tableLevel);//when quit the table
#endif //HM3_SYMBOLTABLE_H