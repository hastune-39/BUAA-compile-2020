//
// Created by 像我这样优秀的人 on 2020/11/13.
//

#ifndef HM5_CODE_ADDRESSSPACE_H
#define HM5_CODE_ADDRESSSPACE_H

//这里给字符串溜了1000,变量区为0x10000000
#define VarRegister (0x100000+0x10010000)
//#define TempRegister (0x10000000+0x10010000)
#define TempRegister (0x200000+0x10010000)
//给20个保护寄存器的部分
#define ProtectReg (0xFFFB0 + 0x10010000)
#define varBase (0)
#define varSize 1024

#define tempVarBase (0)
#define tempVarSize 1024
#endif //HM5_CODE_ADDRESSSPACE_H
