//
// Created by 像我这样优秀的人 on 2020/9/27.
//
#include "lexical_analysis.h"

string lower_word(string word);

bool is_character(char letter);

bool is_string(char letter);

bool is_num(char letter);

bool is_delimiter(char letter);

bool is_letter(char letter);

bool is_identifer(char letter);

void lexicalAnalysis_readFile(ifstream *file) {
    inFile = file;
    //initialize
    lexical_str.clear();
    lexical_type.clear();
    c = ' ';
}



bool finish_readFile() {
    if ((inFile)->eof())return true;
    else return false;
}

pair<string, string> get_a_word() {
    //initialize
    lexical_str.clear();
    lexical_type.clear();
    skip_blank();
    //first letter
    if (c == '\'') {//character
        while ((c = file_getChar()) && is_character(c)) {
            lexical_str.append(1, c);
        }
        if (c == '\'' && lexical_str.length() == 1) {//true
            lexical_type = "CHARCON";
            c = file_getChar();
        } else {//handle false
            wrong_handle();
        }
    } else if (c == '\"') {//string
        while ((c = file_getChar()) && is_string(c)) {
            lexical_str.append(1, c);
        }
        if (c == '\"' && lexical_str.length() >= 1) {//true
            lexical_type = "STRCON";
            c = file_getChar();
        } else {//wrong handle
            wrong_handle();
        }
    } else if (is_num(c)) {//number 001,1+,213-,123(,  +1?
        do {
            lexical_str.append(1, c);
        } while ((c = file_getChar()) && is_num(c));

        if (is_delimiter(c) || c == ' ' || c == '\n') {//true
            lexical_type = "INTCON";
        } else {
            wrong_handle();
        }
    } else if (is_letter(c)) {//identifier
        do {
            lexical_str.append(1, c);
        } while ((c = file_getChar()) && is_identifer(c));
        if (belongTo_Keyword(lower_word(lexical_str))) {
            lexical_type = KeyWord.at(lower_word(lexical_str));
        } else {
            lexical_type = "IDENFR";
        }
    } else if (is_delimiter(c)) {//delimiter
        lexical_str.append(1, c);
        if (c == '<' || c == '>' || c == '=' || c == '!') {//need pre-read
            c = file_getChar();
            if (c == '=') {
                lexical_str.append(1, c);
                c = file_getChar();
            }
        } else {
            c = file_getChar();
        }
        if (belongTo_Delimiter(lexical_str))lexical_type = Delimiter.at(lexical_str);
    } else {//wrong handle
        wrong_handle();
    }

    //output only in hm2

#ifdef TURN_ON
    //file_out << lexical_type << ' ' << lexical_str << endl;
#ifdef DEBUG
    cout << lexical_type << ' ' << lexical_str << endl;
#endif
#endif
    //return a pair (lexical_str,lexical_type)
    return PAIR(lexical_type, lexical_str);
}

char file_getChar() {
    char temp = (*inFile).get();
    return temp;
}

bool isLegal(char letter) {
    if (is_string(letter) || is_delimiter(letter) || letter == ' ' || letter == '\n' || letter == '\t')return true;
    return false;
}

void skip_blank() {
    while (c == ' ' || c == '\n' || c=='\r' || c=='\t') {
        c = (*inFile).get();
    }
}

bool belongTo_Keyword(string word) {
    if (KeyWord.find(word) != KeyWord.end()) {
        return true;
    } else {
        return false;
    }
}

string lower_word(string word) {
    string lower_word = "";
    for (int i = 0; i < word.length(); i++) {
        if (word.at(i) >= 'A' && word.at(i) <= 'Z') {
            lower_word.append(1, word.at(i) - 'A' + 'a');
        } else {
            lower_word.append(1, word.at(i));
        }
    }
    return lower_word;
}

bool belongTo_Delimiter(string word) {
    if (Delimiter.find(word) != Delimiter.end()) {
        return true;
    } else {
        return false;
    }
}

void wrong_handle() {
    c = file_getChar();
    throw "wrong in analysis!";
}

bool is_character(char letter) {
    if (letter == '+' || letter == '-')return true;
    if (letter == '*' || letter == '/')return true;
    if (is_letter(letter)||is_num(letter))return true;
    return false;
}

bool is_string(char letter) {
    if (letter == 32 || letter == 33 || letter >= 35 && letter <= 126)return true;
    return false;
}

bool is_num(char letter) {
    if (letter >= '0' && letter <= '9')return true;
    return false;
}

bool is_letter(char letter) {
    if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z' || letter == '_')return true;
    return false;
}

bool is_identifer(char letter) {
    if (is_num(letter) || is_letter(letter))return true;
    return false;
}

bool is_delimiter(char letter) {
    if (letter == '+' || letter == '-' || letter == '*' || letter == '/' ||
        letter == '<' || letter == '=' || letter == '>' || letter == '!' ||
        letter == ':' || letter == ';' || letter == ',' || letter == '(' ||
        letter == ')' || letter == '[' || letter == ']' || letter == '{' || letter == '}')
        return true;
    return false;
}
