#ifndef TOKEN
#define TOKEN 1
#include <vector>
#include <string>
using namespace std;
string Lextype[42]{
    "ENDFILE", "ERROR",
    "PROGRAM", "PROCEDURE", "TYPE", "VAR","IF","THEN","ELSE","FI","WHILE",
    "DO","ENDWH","BEGIN","END","READ","WRITE","ARRAY","OF","RECORD",
    "INTEGER","CHAR",
    "RETURN",                                                   //保留字
    "ID","INTC","CHARC",                                          //单词类型
    "ASSIGN","EQ","LT","PLUS","MINUS","TIMES","OVER","LPAREN","RPAREN", //操作符
    "DOT","COLON","SEMI","COMMA","LMIDPAREN","RMIDPAREN","UNDERANGE" };
class Token {
private:
    int lineshow; //记录该单词在原程序中的行数
    string lex;
    string sem;
public:
    Token(int, string, string);
    Token();
    ~Token();
    int getLineShow() {
        return this->lineshow;
    }
    string getLex() {
        return this->lex;
    };
    string getSem() {
        return this->sem;
    }
};

Token::Token(int lineshow, string lex, string sem) {
    this->lineshow = lineshow;
    this->lex = lex;
    this->sem.assign(sem);
}
Token::Token() {
    this->lineshow = 0;
    this->lex = "0";
    this->sem = "0";
}
Token::~Token() {}
#endif
