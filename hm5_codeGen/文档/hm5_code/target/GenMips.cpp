//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#include "../middle/MiddleCode.h"
#include "../middle/BasicBlock.h"
#include <fstream>
#include <iostream>
#include "string"
#include "../middle/Optimize.h"
#include "./MipsOp.h"

using std::ofstream;
using std::endl;
using std::cout;
using std::to_string;

extern basicBlock only_block;
extern ofstream mips_file;
extern basicBlock global_block;
extern vector<basicBlock> rebuild;

void genFourOp(fourOp four);

void gen_space();

void init_register_space();

void genMips() {
    gen_space();

    //genFourOp
    mips_file << ".text" << endl;
    mips_file << "#变量存储基准:" << endl;
    mips_file << "#$gp" << endl;
    mips_file << "li $k0, " + to_string(VarRegister) << endl;
    mips_file << "li $k1, " + to_string(TempRegister) << endl;
    mips_file << "##############start############## " << endl << endl;
    mips_file << "#全局变量初始化:" << endl;
    for (int i = 0; i < global_block.fourOpList.size(); i++) {
        genFourOp(global_block.fourOpList.at(i));
    }
    mips_file << endl << "#跳转到主函数:" << endl;
    mips_file << "j main" << endl; //开局跳到main函数
    for (int blockNum = 0; blockNum < rebuild.size(); blockNum++) {
        for (int i = 0; i < rebuild.at(blockNum).fourOpList.size(); i++) {
            genFourOp(rebuild.at(blockNum).fourOpList.at(i));
        }
    }
}

void gen_space() {
    //basic space assign
    mips_file << ".data" << endl;

    //string space assign
    mips_file << "thisisaenter: .asciiz \"\\n\"" << endl;
    vector<fourOp> fourOp_group = only_block.fourOpList;
    for (int i = 0; i < fourOp_group.size(); i++) {
        fourOp temp = fourOp_group.at(i);
        if (temp.op == O_PRINTF) {
            if (temp.item2.kind == K_STRING) {
                mips_file << "str" << temp.item2.nameNum << ": .asciiz \"" << temp.item2.stringValue << "\"" << endl;
            }
        }
    }
}

string gen_reg_out(int reg_num) {
    switch (reg_num) {
        case s0:
            return "$s0";
        case s1:
            return "$s1";
        case s2:
            return "$s2";
        case s3:
            return "$s3";
        case s4:
            return "$s4";
        case s5:
            return "$s5";
        case s6:
            return "$s6";
        case s7:
            return "$s7";
        case t3:
            return "$t3";
        case t4:
            return "$t4";
        case t5:
            return "$t5";
        case t6:
            return "$t6";
        case t7:
            return "$t7";
        default:
            cout << "unknown reg!" << endl;
    }
    return "";
}

enum type {
    op_none,
    op_cons,
    op_temp,
    op_var,
    op_gp,
    op_reg,
};


void genFourOp(fourOp four) {

    //变量定义及初始化
    if (four.op == O_VAR) {
        /***
         * #int b = 0
           li $t1, 0
           sw $t1, 4
         */
        if (four.item2.kind == K_CONSTANT) {
            mips_file << "li $t1, " << four.item2.value << endl;
            //item1可能用寄存器 把t2赋值给item1用的寄存器 move $s0,$t1
            if (four.item1.use_register) {
                mips_file << "move " << gen_reg_out(four.item1.reg) << ",$t1" << endl;
            } else {
                if (four.item1.addressBaseType == A_GP) {
                    mips_file << "sw $t1, " << four.item1.address << "($gp)" << endl;
                } else if (four.item1.addressBaseType == A_VAR) {
                    mips_file << "sw $t1, " << four.item1.address << "($k0)" << endl;
                }
            }
        } else if (four.item2.kind == K_VARIABLE) {
            /***
             * lw $t1,add($base)
             * sw $t1,add($base)
             */
            mips_file << "lw $t1, " << four.item2.address << "($k0)" << endl;
            if (four.item1.use_register) {
                //move $s0,$t1
                mips_file << "move " << gen_reg_out(four.item1.reg) << ",$t1" << endl;
            } else {
                if (four.item1.addressBaseType == A_GP) {
                    mips_file << "sw $t1, " << four.item1.address << "($gp)" << endl;
                } else if (four.item1.addressBaseType == A_VAR) {
                    mips_file << "sw $t1, " << four.item1.address << "($k0)" << endl;
                }
            }
        } else cout << "err-genFourOp-var" << endl;
    }



    //表达式运算
    if (four.op == O_PLUS || four.op == O_MINU ||
        four.op == O_MULT || four.op == O_DIV) {
        /***
         * 算算总共有哪些类型(对于所有 + - * /)
         * 1.add $t0, temp, cons
         * 2.add $t0, cons, temp
         * 3.add $t0, reg, cons
         * 4.add $t0, cons, reg
         * 5.add $t0, temp, reg
         * 6.add $t0, reg, temp
         * 7.add $t0, temp, temp
         * 8.add $t0, reg, reg
         *
         * 别忘了2,3位还可能是: undirect, gp, var, temp, cons, reg
         *
         * 然后最后都要做一个:
         * sw $t0, tempAddress
         *
         * 所以需要:
         * 1.获得2位的方法
         * 2.获得3位的方法
         */
        /***
         * li $t1, 2
           lw $t2, 4
           add $t0,$t1,$t2
           sw $t0, 1028
         */
        //获得第二个和第三个操作数
        mips_op op2;
        op2.getOp2(four);

        mips_op op3;
        op3.getOp3(four);

        /***
         * 性能低下1，因为每个表达式最终都要存到一个临时变量区
         */
        switch (four.op) {//可能的bug: 表达式运算的顶操作符必须是直接地址,且只是临时变量
            case O_PLUS:
//                mips_file << "addu $t0, $t1, $t2" << endl;
                mips_file << "addu $t0, " << op2.output << ", " << op3.output << endl;
                //存放地址
                mips_file << "sw $t0, " << four.item1.address << "($k1)" << endl;
                break;
            case O_MINU:
//                mips_file << "subu $t0, $t1, $t2" << endl;
                mips_file << "subu $t0, " << op2.output << ", " << op3.output << endl;
                //存放地址
                mips_file << "sw $t0, " << four.item1.address << "($k1)" << endl;
                break;
            case O_MULT:
//                mips_file << "mul $t0, $t1, $t2" << endl;
                mips_file << "mul $t0, " << op2.output << ", " << op3.output << endl;
                //存放地址
                mips_file << "sw $t0, " << four.item1.address << "($k1)" << endl;
                break;
            case O_DIV:
//                mips_file << "div $t1, $t2" << endl;
                mips_file << "div " << op2.output << ", " << op3.output << endl;
                //存放地址
                mips_file << "mflo $t0" << endl;
                mips_file << "sw $t0, " << four.item1.address << "($k1)" << endl;
                break;
            default:
                cout << "wrong in swicth math_op" << endl;
        }
    }


    //赋值语句
    if (four.op == O_ASSIGN) {
        //1.获得op2
        mips_op op2;
        op2.getOp2(four);

        //2.判断op1，这里op1比较特殊，需要判断：
        /***
         * 1.move reg, op2
         * 2.sw op2,op1
         *
         */
        if (four.item1.use_register) {
            //如果定义的变量在寄存器 move reg, op2
            mips_file << "move " << gen_reg_out(four.item1.reg) << ", " << op2.output << endl;
        } else {
            //不使用寄存器
            if (four.item1.direct_address) {
                if (four.item1.addressBaseType == A_GP) {
//                    mips_file << "sw $t1, " << four.item1.address << "($gp)" << endl;
                    mips_file << "sw " << op2.output << ", " << four.item1.address << "($gp)" << endl;
                } else if (four.item1.addressBaseType == A_VAR) {
                    mips_file << "sw " << op2.output << ", " << four.item1.address << "($k0)" << endl;
                }
            } else {
                //非直接一定存储在临时变量区
                mips_file << "lw $t0, " << four.item1.address << "($k1)" << endl;
                //添加偏移
                if (four.item1.addressBaseType == A_GP) {
                    mips_file << "addu $t0, $t0, $gp" << endl;
                } else if (four.item1.addressBaseType == A_VAR) {
                    mips_file << "addu $t0, $t0, $k0" << endl;
                }
                mips_file << "sw " << op2.output << ", " << "0($t0)" << endl;
            }
        }
    }

    /***
     * 条件语句
     */
    //有条件跳转语句
    if (four.op == O_bgt || four.op == O_bge || four.op == O_blt || four.op == O_ble || four.op == O_beq ||
        four.op == O_bne) {
        //输出注释
        mips_file << endl;
        mips_file << four.notation << endl;

        //取出两个表达式
        mips_op op1;
        op1.getOp1(four);

        mips_op op2;
        op2.getOp2(four);

        //跳转语句
        switch (four.op) {
            case O_bgt:
//                mips_file << "bgt $t0, $t1, " + four.item3.label << endl;
                mips_file << "bgt " << op1.output << ", " << op2.output << ", " << four.item3.label << endl;
                break;
            case O_bge:
//                mips_file << "bge $t0, $t1, " + four.item3.label << endl;
                mips_file << "bge " << op1.output << ", " << op2.output << ", " << four.item3.label << endl;
                break;
            case O_blt:
//                mips_file << "blt $t0, $t1, " + four.item3.label << endl;
                mips_file << "blt " << op1.output << ", " << op2.output << ", " << four.item3.label << endl;
                break;
            case O_ble:
//                mips_file << "ble $t0, $t1, " + four.item3.label << endl;
                mips_file << "ble " << op1.output << ", " << op2.output << ", " << four.item3.label << endl;
                break;
            case O_beq:
//                mips_file << "beq $t0, $t1, " + four.item3.label << endl;
                mips_file << "beq " << op1.output << ", " << op2.output << ", " << four.item3.label << endl;
                break;
            case O_bne:
//                mips_file << "bne $t0, $t1, " + four.item3.label << endl;
                mips_file << "bne " << op1.output << ", " << op2.output << ", " << four.item3.label << endl;
                break;
            default:
                cout << "genmips-if: unknown four.op";
        }
    }

    //无条件跳转语句
    if (four.op == O_j) {
        mips_file << "j " + four.item1.label << endl;
    }

    //放置标记语句
    if (four.op == O_putlabel) {
        mips_file << four.item1.label + ": " << four.notation << endl;
    }


    //读语句,类似于赋值语句
    if (four.op == O_SCANF) {
        if (four.item1.valueType == T_CHAR) {
            mips_file << "li $v0, 12" << endl;
        } else if (four.item1.valueType == T_INT) {
            mips_file << "li $v0, 5" << endl;
        } else cout << "wrong in scanf target code!" << endl;
        mips_file << "syscall" << endl;
        //读给局部变量还是全局变量呢?
        if (four.item1.use_register) {
            //使用寄存器 move reg, $v0
            mips_file << "move " << gen_reg_out(four.item1.reg) << ", $v0" << endl;
        } else {
            if (four.item1.addressBaseType == A_GP) {
                mips_file << "sw $v0, " << four.item1.address << "($gp)" << endl;
            } else if (four.item1.addressBaseType == A_VAR) {
                mips_file << "sw $v0, " << four.item1.address << "($k0)" << endl;
            }
        }
    }


    //写语句
    if (four.op == O_PRINTF) {
        mips_file << four.notation << endl;
        if (four.item2.kind == K_STRING) {
            mips_file << "li $v0, 4" << endl;
            mips_file << "la, $a0, str" << four.item2.nameNum << endl;
            mips_file << "syscall" << endl;
        } else if (four.item2.kind == K_CONSTANT || four.item2.kind == K_VARIABLE) {//printf constant
            if (four.item2.valueType == 0) {//print character
                mips_file << "li $v0, 11" << endl;
            } else if (four.item2.valueType == 1) { // print int
                mips_file << "li $v0, 1" << endl;
            }

            if (four.item2.kind == K_CONSTANT) {
                mips_file << "li $a0, " << four.item2.value << endl;
            } else if (four.item2.kind == K_VARIABLE) {
                if (four.item2.use_register) {
                    //使用寄存器
                    mips_file << "move $a0, " << gen_reg_out(four.item2.reg) << endl;
                } else {
                    if (four.item2.direct_address) {
                        if (four.item2.addressBaseType == A_GP) {
                            mips_file << "lw $a0, " << four.item2.address << "($gp)" << endl;
                        } else if (four.item2.addressBaseType == A_VAR) {
                            mips_file << "lw $a0, " << four.item2.address << "($k0)" << endl;
                        } else if (four.item2.addressBaseType == A_TEMP) {
                            mips_file << "lw $a0, " << four.item2.address << "($k1)" << endl;
                        }
                    } else {
                        //间接只能存储在临时变量区
                        mips_file << "lw $a0, " << four.item2.address << "($k1)" << endl;
                        //添加偏移
                        if (four.item2.addressBaseType == A_GP) {
                            mips_file << "addu $a0, $a0, $gp" << endl;
                        } else if (four.item2.addressBaseType == A_VAR) {
                            mips_file << "addu $a0, $a0, $k0" << endl;
                        }
                        mips_file << "lw $a0, 0($a0)" << endl;
                    }
                }
            }
            mips_file << "syscall" << endl;
        } else cout << "No type of l_op kind in printf!" << endl;

        //printf enter
        if (four.item3.valueType == 1) {
            mips_file << "li $v0, 4" << endl;
            mips_file << "la $a0, thisisaenter" << endl;
            mips_file << "syscall" << endl;
        }
    }

//    表达式模板
//    if (four.item?.direct_address) {
//        if(four.item?.addressBaseType == A_GP){
//            mips_file << "lw ?, " << four.item?.address << "($k0)" << endl;
//        }else if(four.item?.addressBaseType == A_VAR){
//            mips_file << "lw ?, " << four.item?.address << "($k0)" << endl;
//        }else if(four.item?.addressBaseType == A_TEMP){
//            mips_file << "lw ?, " << four.item?.address << "($k1)" << endl;
//        }
//
//    } else {
//        //非直接一定在k1
//        mips_file << "lw ?, " << four.item?.address << "($k1)" << endl;
//        //添加相对于k0偏移
//        if(four.item?.addressBaseType == A_GP){
//            mips_file << "add ?, ->?, $gp" << endl;
//        }else if(four.item?.addressBaseType == A_VAR){
//            mips_file << "add ?, ->?, $k0" << endl;
//        }
//        mips_file << "lw ?, 0(-> ?)" << endl;
//    }

    /***
     * 函数调用四元式,把结果存到a0里
     */
    if (four.op == O_SaveBackValue) {
        mips_file << "#函数返回值为: " << endl;
        //li $a0,value  或  lw $a0,address($base)
        if (four.item1.use_register) {
            mips_file << "move $a0, " << gen_reg_out(four.item1.reg) << endl;
        } else {
            if (four.item1.kind == K_CONSTANT) {
                mips_file << "li $a0, " << four.item1.value << endl;
            } else if (four.item1.kind == K_VARIABLE) {
                if (four.item1.direct_address) {
                    if (four.item1.addressBaseType == A_GP) {
                        mips_file << "lw $a0, " << four.item1.address << "($gp)" << endl;
                    } else if (four.item1.addressBaseType == A_VAR) {
                        mips_file << "lw $a0, " << four.item1.address << "($k0)" << endl;
                    } else if (four.item1.addressBaseType == A_TEMP) {
                        mips_file << "lw $a0, " << four.item1.address << "($k1)" << endl;
                    }

                } else {
                    //非直接一定在k1
                    mips_file << "lw $a0, " << four.item1.address << "($k1)" << endl;
                    //添加相对于k0偏移
                    if (four.item1.addressBaseType == A_GP) {
                        mips_file << "addu $a0, $a0, $gp" << endl;
                    } else if (four.item1.addressBaseType == A_VAR) {
                        mips_file << "addu $a0, $a0, $k0" << endl;
                    }
                    mips_file << "lw $a0, 0($a0)" << endl;
                }
            }
        }
    }

    if (four.op == O_EXIT) {
        //li $v0,10
        //syscall
        mips_file << "#主函数退出: " << endl;
        mips_file << "li $v0, 10" << endl;
        mips_file << "syscall" << endl;
    }

    if (four.op == O_back) {
        //lw $t0, 0($k0)
        //sub $k0, $k0, $t0
        mips_file << "#从函数退出中,清除空间并准备跳回... " << endl;
        mips_file << "lw $t0, 0($k0)" << endl;
        mips_file << "lw $ra, 4($k0)" << endl;
        mips_file << "subu $k0, $k0, $t0" << endl;
        mips_file << "subu $k1, $k1, 10000 #奇怪的临时空间减小" << endl;
        mips_file << "jr $ra" << endl;
        return;
    }

    if (four.op == O_addSpace) {
        mips_file << "#准备进入函数,正在分配空间..." << endl;
        mips_file << "addu $k0, $k0, " << four.item1.size << endl;
        mips_file << "addu $k1, $k1, 10000 #奇怪的临时空间分配" << endl;
        mips_file << "li $t0, " << four.item1.size << endl;
        mips_file << "sw $t0, 0($k0)" << endl;
//        mips_file << "sw $ra, 4($k0)" << endl;//没跳转,还不知道ra是啥呢
        return;
    }

    if (four.op == O_JumpToFunction) {
//        if(four.item1.label == "addnum"){
            mips_file << "#跳转函数" << endl;
//        }
        //开一个用来保护寄存器的栈
        //保护
        int base = 0;
        for (set<int>::iterator iter = four.item1.protectReg.begin(); iter != four.item1.protectReg.end(); iter++) {
            base -= 4;
            mips_file << "sw " << gen_reg_out(*iter) << ", " << base << "($sp)" << endl;
        }
        mips_file << "addu $sp,$sp," << to_string(base)<<endl;
        mips_file << "jal " << four.item1.label << endl;
        //恢复
        int backbase = -base -4;
        for (set<int>::iterator iter = four.item1.protectReg.begin(); iter != four.item1.protectReg.end(); iter++) {
            mips_file << "lw " << gen_reg_out(*iter) << ", " << backbase << "($sp)" << endl;
            backbase -= 4;
        }
        mips_file << "addu $sp,$sp," << to_string(-base)<<endl;
        return;
    }

    if (four.op == O_GiveParameters) {
        //sw reg , 8($k0) 已知空间
        mips_op op2;
        op2.getOp2(four);

        //空间已知
//        mips_file << "sw $t1, " << four.item1.value << "($k0)" << endl;
        mips_file << "sw " << op2.output << ", " << four.item1.value << "($k0)" << endl;
        return;
    }

    if (four.op == O_GetReturnValue) {
        mips_file << "#取得有返回值函数调用结果," + four.notation << endl;
        //表达式返回结果一定存储在表达式临时区内
        mips_file << "sw $a0, " << four.item1.address << "($k1)" << endl;
        return;
    }

    if (four.op == O_SaveBackRA) {
        mips_file << "#保存返回地址: " << endl;
        mips_file << "sw $ra, 4($k0)" << endl;
        return;
    }
}