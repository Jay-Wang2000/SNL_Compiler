#include "morphology\bean\Token.h"
#include "morphology\bean\TokenList.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;
typedef enum {
    START,
    INID,
    INNUM,
    DONE,
    INASSIGN,
    INCOMMENT,
    INRANGE,
    INCHAR
} State;
string L;
string N;
string OP = "+-*/();[]=<";
string OP_Type[] = { "EQ", "LT", "PLUS", "MINUS", "TIMES", "OVER", "LPAREN", "RPAREN",
                "SEMI", "COMMA", "LMIDPAREN", "RMIDPAREN" };
string reserved;
string blank = " \t\n";


bool contain(string arr, char c) {
    for (char var : arr)
        if (var == c)
            return true;
    return false;
}

int findIndex(char var, string op) {
    for (int i = 0;i < op.size();i++)
        if (var == op[i])
            return i;
    return -1;
}

vector<Token>* getTList() {
    for (int i = 2; i <= 22; i++)           //初始化reserve
        reserved.append(Lextype[i]);
    for (int i = 0;i < reserved.size();i++)
        reserved[i] += 32;
    TokenList* tl = new TokenList();                //整个链表存放在堆区 是指向其的指针
    for (int i = 0; i < 26; i++)
        L.push_back('a' + i);
    for (int i = 0; i < 10; i++)
        N.push_back('0' + i);
    ifstream infile;
    infile.open("D:\\qtp\\SNL_Compiler\\example\\common\\c1.txt");
    if (!infile.is_open()) {
        printf("打开文件失败！");
        return {};
    }
    int line = 0;
    while (!infile.eof()) { // get next word
        char tmp[1000];
        infile.getline(tmp, 1000, '\n');
        line++;
        State s = START;
        int i = 0;
        string word; //在词法中识别的单个单词
        while (i < strlen(tmp)) {
            switch (s) {
            case START:
            {
                if (contain(L, tmp[i]))
                    s = INID;
                else if (contain(N, tmp[i]))
                    s = INNUM;
                else if (contain(OP, tmp[i]))
                    s = DONE;
                else if (tmp[i] == ':')
                    s = INASSIGN;
                else if (tmp[i] == '{')
                    s = INCOMMENT;
                else if (tmp[i] == '.')
                    s = INRANGE;
                else if (tmp[i] == '\'')
                    s = INCHAR;
                if (contain(blank, tmp[i])) {  //如果是个空字符
                    i++;
                    continue;
                }
                word.clear();
                word.push_back(tmp[i]); //如果不是空格，则说明进入了一个新的单词，初始化
                i++;
                break;
            }

            case INID:
                while (i < strlen(tmp) && (contain(L, tmp[i]) || contain(N, tmp[i++])))
                    word.push_back(tmp[i++]);
                if (reserved.find(word) != string::npos) {
                    string lex;
                    for (int j = 0; j < word.size(); j++)
                        lex.push_back(word[i] - 32); //转换成大写
                    tl->add(Token(line, lex, word));
                } else
                    tl->add(Token(line, "ID", word));
            case INNUM:
                while (i < strlen(tmp) && contain(N, tmp[i]))
                    word.push_back(tmp[i++]);
                tl->add(Token(line, "INTC", word));
                break;

            case DONE:
                tl->add(Token(line, OP_Type[findIndex(word[0], OP)], word));
                break;

            case INASSIGN:
                if (i < strlen(tmp) && tmp[i] == '=') {
                    word.push_back(tmp[i]);
                    tl->add(Token(line, "ASSIGN", word));
                    i++;
                }
                tl->add(Token(line, "ERROR", word));
                break;

            case INCOMMENT:
                while (i < strlen(tmp) && tmp[i] != '}' && tmp[i] != EOF)               //注释不生产单词
                    i++;

                break;
            case INRANGE:
                if (i < strlen(tmp)) {
                    if (tmp[i] == '.') {
                        word.push_back(tmp[i]);
                        tl->add(Token(line, "UNDERANGE", word));
                        i++;
                    } else
                        tl->add(Token(line, "DOT", word));
                }
                break;

            case INCHAR:
                int f = i;                      //记录起始位置
                while (i < strlen(tmp) && (contain(L, tmp[i]) || contain(N, tmp[i++])))
                    word.push_back(tmp[i++]);
                if (i < strlen(tmp) && tmp[i] == '\'') {
                    word.push_back(tmp[i++]);
                    tl->add(Token(line, "CHARC", word));
                } else {
                    tl->add(Token(line, "ERROR", word));
                    i = f;
                }

                break;
            }
        }
    }
    ofstream outfile;
    outfile.open("D:\\qtp\\SNL_Compiler\\out\\Token.txt");
    if (!outfile.is_open())
        printf("输出失败！");
    else {
        Token* out = tl->getList()->data();
        for (int i = 0;i < tl->getList()->size();i++) {
            outfile << out->getLineShow() << " " << out->getLex() << " " << out->getSem() << endl;
            out++;
        }
    }

    infile.close();
    outfile.close();
    return tl->getList();
}

int main() {
    getTList();
}