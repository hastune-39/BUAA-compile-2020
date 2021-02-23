//
// Created by 像我这样优秀的人 on 2020/12/9.
//
#include "Optimize.h"

using std::pair;
using std::set;
using std::cout;
using std::endl;

extern basicBlock global_block;
extern basicBlock only_block;//未划分的基本块
vector<basicBlock> function_block;//按照函数划分的基本块

vector<basicBlock> BlockList;//将中间代码划分为多个基本块
map<pair<int, int>, set<pair<int, int>>> conflict_graph;//冲突图
map<pair<int, int>, int/**染色数值**/> var_color;//寄存器分配
vector<basicBlock> rebuild;//重新导出的多个包含中间代码的基本块

map<int, bool> sReg_use;//全局寄存器的使用情况
/***
 * !!!!!!!!!!!!!!!!!!!!!!!!从头到尾要注意数组的操作，数组有可能direct_address=false 但 address_base_type = var!!!!!!!!!!!!!!!!
 */
void analysis_block(basicBlock block);

void start_optimize() {
    cout << "start analysis by function" << endl;
    organize_by_function(only_block);
    cout << "end analysis by function" << endl;

    //对于每一个function_block进行教科书的处理
    for (int i = 0; i < function_block.size(); i++) {
        //初始化
        BlockList.clear();
        conflict_graph.clear();
        var_color.clear();

        //进行处理
        //改变blocklist
        analysis_block(function_block.at(i));
        //改变blocklist
        cout << "start add control flow..." << endl;
        add_control_flow();
        cout << "finish add control flow..." << endl;
        //改变blocklist
        cout << "start active variable analysis..." << endl;
        active_variable_analysis();
        cout << "end active variable analysis..." << endl;
        //改变blocklist与conflict_graph
        cout << "start inline active variable analysis..." << endl;
        inline_active_variable_analysis();
        cout << "end inline active variable analysis..." << endl;
        //改变var_color
        cout << "start color..." << endl;
        color(s_end);
        cout << "end color..." << endl;

        if (i == 9) {
            cout << "start rebuild middle code..." << endl;
        }

        rebuild_middle_code();
        cout << "end rebuild middle code..." << endl;
    }

    //临时寄存器分配
    cout << "start alloc temp register" << endl;
    alloc_temp_reg(t_end);
    cout << "end alloc temp register" << endl;
}

/***
 * 将整个程序按照函数划分，每两个函数label之间是一个函数
 */
void organize_by_function(basicBlock block) {
    for (int i = 0; i < block.fourOpList.size(); i++) {
        fourOp nowFourOp = block.fourOpList.at(i);
        if (nowFourOp.op == O_putlabel && nowFourOp.item1.isFunctionLabel) {
            //函数基本块的开始
            basicBlock new_function_block;
            new_function_block.fourOpList.push_back(nowFourOp);
            for (i = i + 1; i < block.fourOpList.size(); i++) {
                nowFourOp = block.fourOpList.at(i);
                if (nowFourOp.op == O_putlabel && nowFourOp.item1.isFunctionLabel) {
                    //函数基本块的结束
                    break;
                }
                //非结束
                new_function_block.fourOpList.push_back(block.fourOpList.at(i));
            }
            function_block.push_back(new_function_block);//现在i停留在下一个函数基本块的开始位置
            if (i == block.fourOpList.size())return;//若到底了 直接退出
            i--;//回滚一个
        }
    }
}

/***
 * 基本块定义:一旦基本块中的第一条指令开始执行，块中的其他指令都会执行，直到最后一条指令
 * 因此对于能返回的函数调用，均应当忽略其影响划分基本块
 */


void devide_the_basicblock() {
    analysis_block(global_block);
    analysis_block(only_block);
}

void analysis_block(basicBlock block) {
    int i = 0;
    vector<fourOp> buffer;

    while (i < block.fourOpList.size()) {
        basicBlock nowblock;
        for (int j = 0; j < buffer.size(); j++) {
            nowblock.fourOpList.push_back(buffer.at(j));
        }
        buffer.clear();

        while (true && i < block.fourOpList.size()) {//填满一个基本块
            fourOp nowFourOp = block.fourOpList.at(i);

            if (nowFourOp.op == O_putlabel) {//跳转到的语句为入口语句
                buffer.push_back(nowFourOp);
                i++;
                break;
            } else if (belong_to_branch(nowFourOp.op)) {//跳转语句的下一句为入口语句
                nowblock.fourOpList.push_back(nowFourOp);
                i++;
                break;
            } else {//普通语句均属于该基本块
                nowblock.fourOpList.push_back(nowFourOp);
                i++;
            }
        }
        if (nowblock.fourOpList.size() > 0) {
            nowblock.blockNum = BlockList.size();
            BlockList.push_back(nowblock);
        }
    }
    basicBlock nowblock;
    for (int j = 0; j < buffer.size(); j++) {
        nowblock.fourOpList.push_back(buffer.at(j));
    }
    buffer.clear();
    if (nowblock.fourOpList.size() > 0) {
        nowblock.blockNum = BlockList.size();
        BlockList.push_back(nowblock);
    }
}

/***
 *  1.对于最后一条语句不为跳转语句的block（如下一条语句为label）
 *  ①将i的后继添加i+1,将i+1的前驱添加i
 *  2.对于最后一条语句为跳转语句的block
 *  ①寻找第一条语句为putlabel的基本块，判断其label是否与跳转的label相同，添加前驱与后继
 *  ②观察最后一条语句是否为有条件跳转语句，若是则可能跳转失败，将其紧邻的基本块加入其后继
 */
void add_control_flow() {
    for (int i = 0; i < BlockList.size(); i++) {
        vector<fourOp> fourOpList = BlockList.at(i).fourOpList;
        fourOp lastFourOp = fourOpList.at(fourOpList.size() - 1);
        if (!belong_to_branch(lastFourOp.op)) {//相邻基本块
            if (i + 1 < BlockList.size()) {
                BlockList.at(i).follow_block.insert(i + 1);
                BlockList.at(i + 1).precursor_block.insert(i);
            }
        } else {//跳转基本块
            string branch_label;
            //注意b与j指令label所在的操作数位置
            if (lastFourOp.op == O_j) branch_label = lastFourOp.item1.label;
            else branch_label = lastFourOp.item3.label;
            //找到要跳转的基本块
            for (int j = 0; j < BlockList.size(); j++) {
                vector<fourOp> fourOpList = BlockList.at(j).fourOpList;
                fourOp firstFourOp = fourOpList.at(0);
                if (firstFourOp.op == O_putlabel && firstFourOp.item1.label == branch_label) {
                    BlockList.at(i).follow_block.insert(j);
                    BlockList.at(j).precursor_block.insert(i);
                    break;
                }
            }
            //若为有条件跳转则可能失败
            if (condition_branch(lastFourOp.op)) {
                if (i + 1 < BlockList.size()) {
                    BlockList.at(i).follow_block.insert(i + 1);
                    BlockList.at(i + 1).precursor_block.insert(i);
                }
            }
        }
    }
}


/***
 * 疑惑，只需要考虑局部变量和全局变量么，临时变量不需要分析么？
 */
void active_variable_analysis() {
    /***
     * 1.def与use
     * ①def与use概念
     * def: 基本块内定义先于使用的变量
     * use：基本块内使用先于定义的变量
     * ②可能出现def的四元组
     * var,+,-,*,/,=,scanf-------item1.address
     * ③可能出现use的四元组(先忽略函数调用)
     * var,+,-,*,/,=,b,print
     *
     */
    //1.def与use
    for (int blockNum = 0; blockNum < BlockList.size(); blockNum++) {
        vector<fourOp> fourOpList = BlockList.at(blockNum).fourOpList;
        for (int fourOpNum = 0; fourOpNum < fourOpList.size(); fourOpNum++) {
            fourOp nowFourOp = fourOpList.at(fourOpNum);
            //分析use
            if (use_fourOp(nowFourOp.op)) {
                //注意b用的是1，2
                if (condition_branch(nowFourOp.op) && belong_to_var(nowFourOp.item1)) {
                    int base = nowFourOp.item1.addressBaseType;
                    int addr = nowFourOp.item1.address;
                    //查找是否定义先于使用
                    set<pair<int, int>>::iterator iter = BlockList.at(blockNum).def.find(pair<int, int>(base, addr));
                    if (iter == BlockList.at(blockNum).def.end())//use先于def
                        BlockList.at(blockNum).use.insert(pair<int, int>(base, addr));//被定义
                }

                //除了var,所有use都会用2
                if (nowFourOp.op != O_VAR && belong_to_var(nowFourOp.item2)) {
                    int base = nowFourOp.item2.addressBaseType;
                    int addr = nowFourOp.item2.address;
                    //查找是否定义先于使用
                    set<pair<int, int>>::iterator iter = BlockList.at(blockNum).def.find(pair<int, int>(base, addr));
                    if (iter == BlockList.at(blockNum).def.end())//use先于def
                        BlockList.at(blockNum).use.insert(
                                pair<int, int>(base, addr));//被定义
                }

                //注意math类会用3
                if (belong_to_var(nowFourOp.item3)) {
                    int base = nowFourOp.item3.addressBaseType;
                    int addr = nowFourOp.item3.address;
                    //查找是否定义先于使用
                    set<pair<int, int>>::iterator iter = BlockList.at(blockNum).def.find(pair<int, int>(base, addr));
                    if (iter == BlockList.at(blockNum).def.end())//use先于def
                        BlockList.at(blockNum).use.insert(
                                pair<int, int>(base, addr));//被定义

                }
            }

            //分析def
            if (def_fourOp(nowFourOp.op)) {
                //被定义变量必须在item1,且目前不考虑函数
                if (belong_to_var(nowFourOp.item1)) {//check!
                    int base = nowFourOp.item1.addressBaseType;
                    int addr = nowFourOp.item1.address;
                    //查找是否使用先于定义
                    set<pair<int, int>>::iterator iter = BlockList.at(blockNum).use.find(pair<int, int>(base, addr));
                    if (iter == BlockList.at(blockNum).use.end())//def先于use
                        BlockList.at(blockNum).def.insert(
                                pair<int, int>(base, addr));//被定义

                }
            }
        }
    }

    /***
     * 2.in与out
     * in = use并(out-def); out = in的并
     * 循环直到in与out均不再变化
     */
    bool change = true;
    while (change) {
        change = false;
        //遍历每一个基本块
        for (int i = BlockList.size() - 1; i >= 0; i--) {
            //1.求out[B]
            set<pair<int, int>> out = CUPS(i);
            //2.求out[B]-def[B]
            set<pair<int, int>> def = BlockList.at(i).def;
            set<pair<int, int>> out_sub_def = SUB(out, def);
            //3.求in[B]=use[B]U(out[B]-def[B])
            set<pair<int, int>> use = BlockList.at(i).use;
            set<pair<int, int>> in = CUP(use, out_sub_def);
            //4.将out与in保存，并查看是否发生变化
            if (out.size() > BlockList.at(i).out.size() || in.size() > BlockList.at(i).in.size()) {
                change = true;
            }
            BlockList.at(i).out = out;
            BlockList.at(i).in = in;
        }
    }
}

/***
 * 每一个四元式引入:
 * live,def,use
 * ①一个基本块的最后一个四元式令 live=out[B]
 * ②更新live=useU(live-def)
 * ③上一条四元式的live[i]=live[i+1]转至②
 */
void inline_active_variable_analysis() {

    //对于每一个基本块
    for (int blockNum = 0; blockNum < BlockList.size(); blockNum++) {
        basicBlock nowblock = BlockList.at(blockNum);
        //逆序，对于每一个四元式
        for (int fourOpNum = nowblock.fourOpList.size() - 1; fourOpNum >= 0; fourOpNum--) {
            fourOp nowFourOp = nowblock.fourOpList.at(fourOpNum);

            //0.若四元式为var，先将点加入到冲突图里
            if (nowFourOp.op == O_VAR) {
                if (def_fourOp(nowFourOp.op) && (belong_to_var(nowFourOp.item1))) {
                    pair<int, int> start_point = pair<int, int>(nowFourOp.item1.addressBaseType,
                                                                nowFourOp.item1.address);
                    map<pair<int, int>, set<pair<int, int>>>::iterator find_start_set = conflict_graph.find(
                            start_point);
                    if (find_start_set == conflict_graph.end()) {
                        //还没有建造sttart_point的冲突列表
                        set<pair<int, int>> new_set;
                        conflict_graph.insert(
                                pair<pair<int, int>, set<pair<int, int>>>(start_point, new_set));//map,  nmsl
                    }
                }
            }


            set<pair<int, int>> live;
            //1.初始化live
            if (fourOpNum == nowblock.fourOpList.size() - 1) {
                live = nowblock.out;
            } else {
                live = nowblock.fourOpList.at(fourOpNum + 1).live;
            }
            //2.def
            pair<int, int> def = pair<int, int>(-1, -1);
            if (def_fourOp(nowFourOp.op)) {
                if (belong_to_var(nowFourOp.item1))
                    def = pair<int, int>(nowFourOp.item1.addressBaseType, nowFourOp.item1.address);//def只考虑item1,只有一项
            }
            //3.use
            set<pair<int, int>> use;
            if (use_fourOp(nowFourOp.op)) {//use只考虑item2与item3
                if (condition_branch(nowFourOp.op) && belong_to_var(nowFourOp.item1))
                    use.insert(pair<int, int>(nowFourOp.item1.addressBaseType, nowFourOp.item1.address));
                if (nowFourOp.op != O_VAR && belong_to_var(nowFourOp.item2))
                    use.insert(pair<int, int>(nowFourOp.item2.addressBaseType, nowFourOp.item2.address));
                if (belong_to_var(nowFourOp.item3))
                    use.insert(pair<int, int>(nowFourOp.item3.addressBaseType, nowFourOp.item3.address));
            }
            //4.判断冲突
            //添加冲突图
            if (def != pair<int, int>(-1, -1))build_conflict_graph(def, live);

            //5.更新live=useU(live-def)
            //先减去def
            set<pair<int, int>>::iterator iter = live.find(def);
            if (iter != live.end()) {
                live.erase(iter);
            }
            //再添加use
            live = CUP(live, use);
            //更新live
            nowblock.fourOpList.at(fourOpNum).live = live;
        }

        BlockList.at(blockNum) = nowblock;
    }
}

/***
 * 在四元式活跃变量分析中，def与live的各元素增加冲突边
 * !问题：冲突图包含了所有的变量么(除temp)???????????????????????????????? 前提是每一个变量都需要被def过
 *
 * 注意：只有满足：kind=K_variable 且 direct_address = true 且 addressBaseType = A_VAR|A_GP的才能获得寄存器
 * 完成：参与冲突图构建的只有局部变量与全局变量
 */
void build_conflict_graph(pair<int, int> def, set<pair<int, int>> live) {
    //遍历live
    for (set<pair<int, int>>::iterator iter = live.begin(); iter != live.end(); iter++) {
        //每一个live元素，除了变量自己，与def添加冲突边
        if ((*iter) != def) {
            add_conflict_edge(def, (*iter));
            add_conflict_edge((*iter), def);
        }
    }
}

void add_conflict_edge(pair<int, int> start_point, pair<int, int> end_point) {
    map<pair<int, int>, set<pair<int, int>>>::iterator find_start_set = conflict_graph.find(start_point);
    if (find_start_set == conflict_graph.end()) {
        //还没有建造sttart_point的冲突列表
        set<pair<int, int>> new_set;
        new_set.insert(end_point);
        conflict_graph.insert(pair<pair<int, int>, set<pair<int, int>>>(start_point, new_set));//map,  nmsl
    } else {
        (*find_start_set).second.insert(end_point);
    }
}

/***
 * 染色算法如下:
 * 1.找出所有冲突边小于sRegNum的点，移除并加入到染色栈内
 * 2.当没有小于sRegNum的点时，将其移除
 * 3.重复1，2操作直至只剩一个结点
 */
void color(int sRegNum) {
    //初始化待染色栈与染色情况表
    stack<pair<int, int>> uncolored_stack;
    for (map<pair<int, int>, set<pair<int, int>>>::iterator iter = conflict_graph.begin();
         iter != conflict_graph.end(); iter++) {
        var_color.insert(pair<pair<int, int>/**染色变量**/, int/**染色数值**/>((*iter).first, -1));
    }
    map<pair<int, int>, set<pair<int, int>>> dynamic_graph = conflict_graph;
    //2.移除结点直至剩一个结点
    while (dynamic_graph.size() > 1) {
        bool flag = false;//判断是否有冲突边小于sRegNum的结点

        //①寻找所有冲突边小于sRegNum的结点
        for (map<pair<int, int>, set<pair<int, int>>>::iterator iter = dynamic_graph.begin();
             iter != dynamic_graph.end(); iter++) {
            //找到了
            if ((*iter).second.size() < sRegNum) {
                //删除与该点连接的其它点的这条边
                for (set<pair<int, int>>::iterator conflict_set = (*iter).second.begin();
                     conflict_set != (*iter).second.end(); conflict_set++) {
                    map<pair<int, int>, set<pair<int, int>>>::iterator other_point;
                    other_point = dynamic_graph.find((*conflict_set));
                    if (other_point != dynamic_graph.end()) {
                        //找到并删除
                        (*other_point).second.erase((*iter).first);
                    }
                }

                //将变量加入待染色栈
                uncolored_stack.push((*iter).first);

                //将自己从图中删除
                dynamic_graph.erase(iter);
                flag = true;
                break;
            }
        }

        //②没有小于sRegNum的结点，则随便挑选一个删除
        if (flag == false) {
            map<pair<int, int>, set<pair<int, int>>>::iterator toDeletePoint = dynamic_graph.begin();
            //删除与该点连接的其它点的这条边
            for (set<pair<int, int>>::iterator other_point = (*toDeletePoint).second.begin();
                 other_point != (*toDeletePoint).second.end(); other_point++) {
                //对于每一个连接待删除点的其它点，从他们的set中删除该点
                map<pair<int, int>, set<pair<int, int>>>::iterator other_point_graph = dynamic_graph.find(*other_point);
                if (other_point_graph != dynamic_graph.end()) {
                    //找到了其它点1
                    (*other_point_graph).second.erase((*toDeletePoint).first);
                }
            }

            //大于sRegNum的不进行染色处理，不加入待染色栈

            //将自己从图中删除
            dynamic_graph.erase(toDeletePoint);
        }
    }

    //3.进行染色分配处理
    //分配剩余的一个颜色,要求s_pool必须大于1
    pair<int, int> last_node = (*dynamic_graph.begin()).first;
    var_color.erase(last_node);
    var_color.insert(pair<pair<int, int>, int>(last_node, s0));
    //将待染色栈的结点依次取出并染色
    while (!uncolored_stack.empty()) {
        //①从栈中去除该颜色
        pair<int, int> uncolored_node = uncolored_stack.top();
        uncolored_stack.pop();

        //②构造一个表来判断有哪些颜色还可以用
        map<int, bool> color_can_use;
        for (int i = 0; i < sRegNum; i++)color_can_use.insert(pair<int, bool>(i, true));

        //③找该点连接的其它点，将他们的颜色设置为不可用
        set<pair<int, int>> conflict_set = conflict_graph.at(uncolored_node);
        for (set<pair<int, int>>::iterator other_point = conflict_set.begin();
             other_point != conflict_set.end(); other_point++) {
            int other_point_color = var_color.at(*other_point);
            if (other_point_color != -1) {
                //若其它颜色被染过色,则令该颜色不可用
                color_can_use.erase(other_point_color);
                color_can_use.insert(pair<int, bool>(other_point_color, false));
            }
        }

        //④挑选数值最小的可用颜色给该结点
        for (int i = 0; i < sRegNum; i++) {//若没有可用颜色则不分配
            if (color_can_use.at(i) == true) {
                var_color.erase(uncolored_node);
                var_color.insert(pair<pair<int, int>, int>(uncolored_node, i));
                break;
            }
        }
    }

}

void rebuild_middle_code() {
    //进行临时寄存器分配处理，即重新导出中间代码
    //要注意对于临时变量不进行s寄存器的分配，对于间接数组不进行s寄存器的分配
    basicBlock new_block;
    for (int blockNum = 0; blockNum < BlockList.size(); blockNum++) {
        //对于每个基本块:
        for (int fourOpNum = 0; fourOpNum < BlockList.at(blockNum).fourOpList.size(); fourOpNum++) {
            //对于每个四元式
            fourOp newFourOp = BlockList.at(blockNum).fourOpList.at(fourOpNum);

            if (newFourOp.op == O_JumpToFunction) {
                /***
                 *  如果是调用函数寄存器，需要判断当前函数块中有哪些寄存器被分配了，并进行保护，
                 *  但是这里不会考虑以后再也不会用到的寄存器，若函数快最后一条语句为调用函数语句，
                 *  我也会把在这个函数快内之前分配过的寄存器进行保护，但实际上都不用，即我无法判断现在在用的寄存器。
                 *
                 *  不，我可以，前提是BlockList内每一个四元式存了live信息，live就是后面需要用到的变量
                 *
                 *  进入可以，那返回呢?紧跟在jal后面，但需要保证不会覆盖掉非s0~s7,t3~t7的寄存器
                 */
                 if(newFourOp.live.size() >=8){
                     cout << "here"<<endl;
                 }
                for (set<pair<int, int>>::iterator iter = newFourOp.live.begin();
                     iter != newFourOp.live.end(); iter++) {
                    //var_color不一定有
                    map<pair<int, int>, int/**染色数值**/>::iterator color_iter;
                    color_iter = var_color.find(*iter);
                    if(color_iter != var_color.end() && (*color_iter).second >=0 ){
                        newFourOp.item1.protectReg.insert(pair<int, int>(s_reg, var_color.at(*iter)));
                    }
                }
            }


            newFourOp.item1 = update_operand(newFourOp.item1);
            newFourOp.item2 = update_operand(newFourOp.item2);
            newFourOp.item3 = update_operand(newFourOp.item3);
            //添加新四元式
            new_block.fourOpList.push_back(newFourOp);
        }
    }
    //重新导出到函数划分的基本块
    rebuild.push_back(new_block);
}

/***
 * test
 */
void test_optimize() {
    //染色
    test_color();
}

/***
 * 要求：
 * 1.不能添加自己指向自己的冲突边
 *
 * 做了基础的test... 按照ppt上的
 */
void test_color() {
    cout << "start color test..." << endl;
    //0.初始化冲突图
    map<pair<int, int>, set<pair<int, int>>> newMap;
    conflict_graph = newMap;
    //1.构造测试数据
    pair<int, int> s1 = pair<int, int>(A_VAR, 0);
    pair<int, int> s2 = pair<int, int>(A_VAR, 4);
    pair<int, int> s3 = pair<int, int>(A_VAR, 8);
    pair<int, int> s4 = pair<int, int>(A_VAR, 12);
    pair<int, int> s5 = pair<int, int>(A_VAR, 16);
    pair<int, int> s6 = pair<int, int>(A_VAR, 20);
    //2.构造冲突图
    add_conflict_edge(s1, s2);
    add_conflict_edge(s1, s3);
    add_conflict_edge(s1, s4);
    add_conflict_edge(s2, s1);
    add_conflict_edge(s2, s3);
    add_conflict_edge(s2, s4);
    add_conflict_edge(s2, s5);
    add_conflict_edge(s3, s1);
    add_conflict_edge(s3, s2);
    add_conflict_edge(s3, s4);
    add_conflict_edge(s3, s5);
    add_conflict_edge(s3, s6);
    add_conflict_edge(s4, s1);
    add_conflict_edge(s4, s2);
    add_conflict_edge(s4, s3);
    add_conflict_edge(s5, s2);
    add_conflict_edge(s5, s3);
    add_conflict_edge(s5, s6);
    add_conflict_edge(s6, s3);
    add_conflict_edge(s6, s5);
    //3.染色
    color(3);

    cout << "end color test..." << endl;
}

/***
 * <------------------一些小函数------------------->
 */

bool condition_branch(int op) {
    if (op == O_bgt || op == O_bge || op == O_blt ||
        op == O_ble || op == O_beq || op == O_bne)
        return true;
    else return false;
}

bool belong_to_branch(int op) {
    if (op == O_bgt || op == O_bge || op == O_blt ||
        op == O_ble || op == O_beq || op == O_bne ||
        op == O_j /**||op == O_JumpToFunction || op == O_back**/)
        return true;
    else return false;
}

bool def_fourOp(int op) {
    if (op == O_VAR || op == O_PLUS || op == O_MINU ||
        op == O_MULT || op == O_DIV || op == O_ASSIGN ||
        op == O_SCANF)
        return true;
    else return false;
}

bool use_fourOp(int op) {
    if (op == O_VAR || op == O_PLUS || op == O_MINU ||
        op == O_MULT || op == O_DIV || op == O_ASSIGN ||
        op == O_bgt || op == O_bge || op == O_blt ||
        op == O_ble || op == O_beq || op == O_bne || op == O_PRINTF || op == O_GiveParameters)
        return true;
    else
        return false;
}


bool belong_to_var(operand op) {
    if (op.direct_address == true && op.dimension == 0 &&
        (op.addressBaseType == A_VAR /**|| op.addressBaseType == A_GP**/))
        return true;
    else return false;
}

/***
 * 计算两个集合font与behind的并
 */
set<pair<int, int>> CUP(set<pair<int, int>> font, set<pair<int, int>> behind) {
    set<pair<int, int>>::iterator behind_begin = behind.begin();
    set<pair<int, int>>::iterator behind_end = behind.end();
    font.insert(behind_begin, behind_end);
    return font; //会不会退出函数后没返回到?
}


/***
 * 计算两个集合的差:
 * out[B]-def[B]
 */
set<pair<int, int>> SUB(set<pair<int, int>> out, set<pair<int, int>> def) {
    set<pair<int, int>> res;
    set_difference(out.begin(), out.end(), def.begin(), def.end(), inserter(res, res.end()));
    return res;
}

/***
 * 计算out[B]=后续基本块的in[]的并
 */
set<pair<int, int>> CUPS(int blockNum) {
    set<pair<int, int>> out_B;

    set<int> follow = BlockList.at(blockNum).follow_block;
    for (set<int>::iterator iter = follow.begin(); iter != follow.end(); iter++) {
        out_B = CUP(out_B, BlockList.at((*iter)).in);
    }

    return out_B;
}

operand update_operand(operand item) {
    if (belong_to_var(item)) {

        map<pair<int, int>, int/**染色数值**/>::iterator iter = var_color.find(
                pair<int, int>(item.addressBaseType, item.address));
        if (iter != var_color.end()) {
            //找到了
            int color = (*iter).second;
            if (color >= 0) {
                //该变量可以使用寄存器
                item.use_register = true;
                item.reg_type = s_reg;
                item.reg = color;
            }
        }

    }
    return item;
}

/***
 * 临时寄存器分配部分
 */
map<pair<int, int>, int> var_temp;
map<pair<int, int>, int> array_var_temp;
map<int, bool> temp_use_case;

void alloc_temp_reg(int tRegNum) {
    //0.建立变量情况表(类似于var_color)与临时寄存器使用情况表,以及初始化
    for (int i = 0; i < tRegNum; i++) {
        temp_use_case.insert(pair<int, bool>(i, true));
    }

    //1.扫描每一个块的每一个指令
    for (int blockNum = 0; blockNum < rebuild.size(); blockNum++) {
        for (int fourOpNum = 0; fourOpNum < rebuild.at(blockNum).fourOpList.size(); fourOpNum++) {
            fourOp now_fourOp = rebuild.at(blockNum).fourOpList.at(fourOpNum);
            //2.对于每一个'+ - * /'指令，进行临时变量的产生与消除
            //!注意现阶段认为只有+-*/会产生临时变量,且临时变量的产生一定是直接的!
            if (now_fourOp.op == O_PLUS || now_fourOp.op == O_MINU ||
                now_fourOp.op == O_MULT || now_fourOp.op == O_DIV) {
                //(1)先消除
                now_fourOp.item2 = remove(now_fourOp.item2);
                now_fourOp.item3 = remove(now_fourOp.item3);

                //(2)再产生(根据运算法则，产生的可以占用消除的寄存器)
                if (!now_fourOp.isArraySubPlus)now_fourOp.item1 = generate(now_fourOp.item1);
            }

            //3.对每一个消除指令，进行临时变量的消除
            if (now_fourOp.op == O_ASSIGN) {
                //item2
                now_fourOp.item2 = remove(now_fourOp.item2);
            }
            if (condition_branch(now_fourOp.op)) {
                //item1与item2
                now_fourOp.item1 = remove(now_fourOp.item1);
                now_fourOp.item2 = remove(now_fourOp.item2);
            }
            if (now_fourOp.op == O_GiveParameters) {
                //item2
                now_fourOp.item2 = remove(now_fourOp.item2);
            }
            if (now_fourOp.op == O_PRINTF) {
                //item2
                now_fourOp.item2 = remove(now_fourOp.item2);
            }
            if (now_fourOp.op == O_SaveBackValue) {
                //item
                now_fourOp.item1 = remove(now_fourOp.item1);
            }


            //4.对于函数跳转指令，进行寄存器保护
            if (now_fourOp.op == O_JumpToFunction) {
                //item1.protectReg
                //寻找现在正在被使用的寄存器
                for (map<int, bool>::iterator iter = temp_use_case.begin(); iter != temp_use_case.end(); iter++) {
                    if ((*iter).second == false) {
                        //保护
                        now_fourOp.item1.protectReg.insert(pair<int, int>(t_reg, (*iter).first));
                    }
                }
            }

            //5.写回
            rebuild.at(blockNum).fourOpList.at(fourOpNum) = now_fourOp;
        }
    }
}

operand remove(operand item) {
    if (item.kind == K_VARIABLE && item.addressBaseType == A_TEMP && item.direct_address == true) {
        //判断能不能消除
        map<pair<int, int>, int>::iterator iter = var_temp.find(
                pair<int, int>(item.addressBaseType, item.address));
        if (iter != var_temp.end()) {
            //分配
            item.use_register = true;
            item.reg_type = t_reg;
            item.reg = (*iter).second;
            //消除map
            var_temp.erase(pair<int, int>(item.addressBaseType, item.address));
            temp_use_case.erase((*iter).second);
            temp_use_case.insert(pair<int, bool>((*iter).second, true));
        }
    }
    return item;
}

/***
 * 前提: +-*四元式左边一定是临时变量
 */
operand generate(operand item) {
    if (item.kind == K_VARIABLE && item.addressBaseType == A_TEMP && item.direct_address == true) {
        //判断能不能产生
        for (map<int, bool>::iterator iter = temp_use_case.begin(); iter != temp_use_case.end(); iter++) {
            if ((*iter).second == true) {
                //可以分配
                item.use_register = true;
                item.reg_type = t_reg;
                item.reg = (*iter).first;

                //添加map
                var_temp.insert(
                        pair<pair<int, int>, int>(pair<int, int>(item.addressBaseType, item.address), (*iter).first));
                temp_use_case.erase((*iter).first);
                temp_use_case.insert(pair<int, bool>((*iter).first, false));

                break;
            }
            //不能分配,保持不变
        }
    }
    return item;
}
