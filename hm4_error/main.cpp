#include <iostream>
#include "lexical_analysis.h"
#include "grammer_analysis.h"
using namespace std;

ofstream file_out("output.txt");

int main() {
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
    }catch(const char* msg){
        cout << "wrong!!!"<<endl;
    }


    //close file
    file_in.close();
    file_out.close();
    return 0;
}
