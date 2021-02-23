//
// Created by 像我这样优秀的人 on 2020/11/14.
//

#include "error.h"
#include <iostream>
#include "fstream"
using std::ofstream;
using std::cout;
using std::endl;
extern ofstream file_out;
void error() {
    file_out <<"NOW WORD IS ERROR!!!--------------------------------------------"<<endl;
    cout << "NOW WORD IS ERROR!!!" << endl;
}