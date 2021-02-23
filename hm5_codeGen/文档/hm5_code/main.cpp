#include <iostream>
#include "include/lexical_analysis.h"
#include "include/grammer_analysis.h"
#include "target/GenMips.h"
#include "middle/Optimize.h"

using namespace std;
ofstream file_out("output.txt");
ofstream file_error("error.txt");
ofstream mips_file("mips.txt");
extern string TYPE;
extern string SYM;
extern map<int, symbolTable> Tables;
extern queue<pair<string,string>> BACKSTRACK;
int main() {
#ifndef test
    //open file_in and outFile
    ifstream file_in("testfile.txt");

    if(!(file_in.is_open())){
        cout<<"error when open file!";
        return 0;
    }

    //run
    try {
        lexicalAnalysis_readFile(&file_in);
        grammer_analysis_RUN();
    }catch(MyException& e){
        cout << "you have an unhandled error!"<<endl;
    }

    //optimize
    start_optimize();
    //test_optimize();
    //generate target code
    genMips();
    //close file
    file_in.close();
    file_out.close();
    return 0;
#endif
#ifdef test
      set_table_symbol(0,K_CONSTANT,constant_line("a",K_CONSTANT,T_INT));
    try {
        check_table_symbol("a");
    } catch (Name_undefine E) {
        cout<<"c"<<endl;
    }
    try {
        check_table_symbol("name");
    } catch (Name_undefine E) {
        cout<<"c"<<endl;
    }
#endif
}
