//
// Created by 像我这样优秀的人 on 2020/12/9.
//

#ifndef HM5_CODE_OPTIMIZE_H
#define HM5_CODE_OPTIMIZE_H

#include "BasicBlock.h"
#include <algorithm>
#include "map"
#include "stack"
#include "Operand.h"

using std::vector;
using std::set;
using std::pair;
using std::map;
using std::stack;

enum sRegPool{
    s0,s1,s2,s3,s4,s5,s6,s7,
    s_end
};

enum tRegPool{
    t3,t4,t5,t6,t7,t8,t9,t_end
};

void start_optimize();


void organize_by_function(basicBlock block);
void devide_the_basicblock();
void add_control_flow();
void active_variable_analysis();
void inline_active_variable_analysis();
void build_conflict_graph(pair<int,int> def, set<pair<int,int>> live);
void color(int sRegNum);
void rebuild_middle_code();
/***
 * test
 */
void test_optimize();
void test_color();

bool condition_branch(int op);
bool belong_to_branch(int op);
bool def_fourOp(int op);
bool use_fourOp(int op);
bool belong_to_var(operand op);

set<pair<int, int>> CUP(set<pair<int, int>> font, set<pair<int, int>> behind);
set<pair<int, int>> SUB(set<pair<int, int>> font, set<pair<int, int>> behind);
set<pair<int, int>> CUPS(int blockNum);

void add_conflict_edge(pair<int,int> start_point, pair<int,int> end_point);
void get_conflict_graph_nodes();

operand update_operand(operand item);
#endif //HM5_CODE_OPTIMIZE_H


/***
 * 临时寄存器分配部分
 */
void alloc_temp_reg(int tRegNum);
operand remove(operand item);
operand generate(operand item);
operand arraygenerate(operand item);

