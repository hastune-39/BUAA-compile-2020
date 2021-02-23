//
// Created by 像我这样优秀的人 on 2020/10/22.
//

#ifndef HM3_ERROR_H
#define HM3_ERROR_H

#include <iostream>
using std::cout;
using std::endl;

static int expressionValue;
using std::exception;

class MyException : public exception {
public:
    const char *what() const throw() {
        return "MyException";
    }
};

/**
 * sub exception
 */
class Illegal_symbol : public MyException {
public:
    const char *what() const throw() {
        return "a";
    }
};

class Name_redefine : public MyException{
public:
    const char *what() const throw() {
        return "b";
    }
};

class Name_undefine : public MyException{
public:
    const char *what() const throw() {
        return "c";
    }
};
/**
 * error check
 */
void error();
#endif //HM3_ERROR_H
