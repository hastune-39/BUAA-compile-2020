//
// Created by 像我这样优秀的人 on 2020/11/14.
//

#ifndef HM5_CODE_ENUM_H
#define HM5_CODE_ENUM_H
enum SYMBOL_KIND {
    K_CONSTANT, K_VARIABLE, K_ARRAY,
    K_FUNCTION,
    K_PARAMETER,
    K_STRING,
    K_NONE
};

enum SYMBOL_TYPE {
    T_CHAR,T_INT
};

enum function_type {
    INT_FUNCTION,
    CHAR_FUNCTION,
    NONE_FUCTION
};

enum opType{
    /**0**/O_VAR,/**1**/O_arraySpaceAlloc,
    /**2**/O_PLUS,/**3**/O_MINU,/**4**/O_MULT,/**5**/O_DIV,/**6**/O_ASSIGN,
    /**7**/O_bgt, /**8**/O_bge, /**9**/O_blt, /**10**/O_ble, /**11**/O_beq, /**12**/O_bne,/**13**/O_j, /**14**/O_putlabel,
    /**15**/O_SCANF, /**16**/O_PRINTF,
    /**17**/O_SaveBackValue,/**18**/O_SaveBackRA, /**19**/O_back,/**20**/O_addSpace,
    /**21**/O_GiveParameters, /**22**/O_JumpToFunction, /**23**/O_GetReturnValue,
    /**24**/O_EXIT,
};

enum varOrTemp{
    V_VAR,
    V_TEMP
};

enum someBaseAddress{
    A_TEMP, A_VAR, A_SP, A_GP
};

enum register_Num{
    R_ra,R_a0
};

enum register_Type{
    no_reg, s_reg, t_reg
};
#endif //HM5_CODE_ENUM_H
