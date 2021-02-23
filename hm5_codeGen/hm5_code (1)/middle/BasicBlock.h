//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#ifndef HM5_CODE_BASICBLOCK_H
#define HM5_CODE_BASICBLOCK_H
#include "vector"
#include "FourOp.h"
#include "set"
using std::set;
using std::vector;
using std::pair;
class basicBlock{
public:
    int blockNum;
    /***
     * 基本块控制流属性
     */
    set<int> precursor_block;
    set<int> follow_block;

    /***
     * 活跃变量分析
     */
    //由于写的很蠢，所以拿地址当标识符
    set<pair<int, int>> def;
    set<pair<int,int>> use;
    set<pair<int,int>> in;
    set<pair<int,int>> out;

    vector<fourOp> fourOpList;
};
#endif //HM5_CODE_BASICBLOCK_H
