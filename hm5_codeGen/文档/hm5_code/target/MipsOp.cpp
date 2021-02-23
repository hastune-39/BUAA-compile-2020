//
// Created by 像我这样优秀的人 on 2020/12/18.
//

#include "MipsOp.h"
#include "fstream";
using std::ofstream;

extern ofstream mips_file;
void mips_op::getOp1(fourOp four){
    if (four.item1.kind == K_CONSTANT) {
        mips_file << "li $t0, " << four.item1.value << endl;
        output = "$t0";
    } else if (four.item1.kind == K_VARIABLE) {
        if(four.item1.use_register){
            //使用寄存器
            output = gen_reg_out(four.item1.reg);
        }else{
            //不使用寄存器
            if (four.item1.direct_address) {
                if (four.item1.addressBaseType == A_GP) {
                    mips_file << "lw $t0, " << four.item1.address << "($gp)" << endl;
                } else if (four.item1.addressBaseType == A_VAR) {
                    mips_file << "lw $t0, " << four.item1.address << "($k0)" << endl;
                } else if (four.item1.addressBaseType == A_TEMP) {
                    mips_file << "lw $t0, " << four.item1.address << "($k1)" << endl;
                }

            } else {
                //非直接一定在k1
                mips_file << "lw $t0, " << four.item1.address << "($k1)" << endl;
                //添加相对于k0偏移
                if (four.item1.addressBaseType == A_GP) {
                    mips_file << "addu $t0, $t0, $gp" << endl;
                } else if (four.item1.addressBaseType == A_VAR) {
                    mips_file << "addu $t0, $t0, $k0" << endl;
                }
                mips_file << "lw $t0, 0($t0)" << endl;
            }

            output = "$t0";
        }
    }
}

void mips_op::getOp2(fourOp four){
    /***
     * @param four
     * type: op_cons -> value:常数值
     * type: op_gp/var/temp -> value: address
     * type: op+reg -> value:寄存器值
     * @return 寄存器字符串
     */
    //直接
    if (four.item2.kind == K_CONSTANT) {
        //常数用li的指令条数比用add快
        mips_file << "li $t1, " << four.item2.value << endl;
         output = "$t1";
    } else if (four.item2.kind == K_VARIABLE) {
        //如果使用寄存器
        if(four.item2.use_register){
            output = gen_reg_out(four.item2.reg);
        }else{
            //不使用寄存器
            if (four.item2.direct_address) {//直接
                if ((four.item2.addressBaseType == A_GP)) {
                    mips_file << "lw $t1, " << four.item2.address << "($gp)" << endl;
                } else if (four.item2.addressBaseType == A_VAR) {
                    mips_file << "lw $t1, " << four.item2.address << "($k0)" << endl;
                } else if (four.item2.addressBaseType == A_TEMP) {
                    mips_file << "lw $t1, " << four.item2.address << "($k1)" << endl;
                }
            } else {//非直接
                //真正地址保存在临时变量区内
                mips_file << "lw $t1, " << four.item2.address << "($k1)" << endl;
                //添加偏移 不会有临时非直接,因为非直接是数组,只会从global与var中取
                if (four.item2.addressBaseType == A_GP) {
                    mips_file << "addu $t1, $t1, $gp" << endl;
                } else if (four.item2.addressBaseType == A_VAR) {
                    mips_file << "addu $t1, $t1, $k0" << endl;
                }
                mips_file << "lw $t1, 0($t1)" << endl;//取值
            }
            //返回t1寄存器
            output= "$t1";
        }

    }

}

void mips_op::getOp3(fourOp four) {
    /***
     * @param four
     * type: op_cons -> value:常数值
     * type: op_gp/var/temp -> value: address
     * type: op+reg -> value:寄存器值
     * @return 寄存器字符串
     */
    if (four.item3.kind == K_CONSTANT) {
        mips_file << "li $t2, " << four.item3.value << endl;
        output= "$t2";//常数使用t2寄存器
    } else if (four.item3.kind == K_VARIABLE) {
        if(four.item3.use_register){
            //使用分配的寄存器
            output= gen_reg_out(four.item3.reg);
        }else{
            //使用临时寄存器
            if (four.item3.direct_address) {
                if (four.item3.addressBaseType == A_GP) {
                    mips_file << "lw $t2, " << four.item3.address << "($gp)" << endl;
                } else if (four.item3.addressBaseType == A_VAR) {
                    mips_file << "lw $t2, " << four.item3.address << "($k0)" << endl;
                } else if (four.item3.addressBaseType == A_TEMP) {
                    mips_file << "lw $t2, " << four.item3.address << "($k1)" << endl;
                }
            } else {
                //非直接地址存放在临时变量区!  bug?
                mips_file << "lw $t2, " << four.item3.address << "($k1)" << endl;
                //添加偏移
                if (four.item3.addressBaseType == A_GP) {
                    mips_file << "addu $t2, $t2, $gp" << endl;
                } else if (four.item3.addressBaseType == A_VAR) {
                    mips_file << "addu $t2, $t2, $k0" << endl;
                }
                mips_file << "lw $t2, 0($t2)" << endl;
            }
            //返回t2
            output = "$t2";
        }
    }
}