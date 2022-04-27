
#include <fstream>
#include <string.h>
#include "scanner.h"
#include <iostream>
#include <vector>
#include<algorithm>
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
string OP = "+-*/();,[]=<";
string OP_Type[] = { "PLUS", "MINUS", "TIMES", "OVER", "LPAREN", "RPAREN",
                "SEMI", "COMMA", "LMIDPAREN", "RMIDPAREN" ,"EQ", "LT" };
vector<string> reserved;
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

vector<Token>* getList(char in[],char out[]) {
    for (int i = 2; i <= 22; i++)           //初始化reserve
        reserved.push_back(Lextype[i]);
    for (int i = 0;i < reserved.size();i++) {
        string s = reserved[i];
        transform(s.begin(), s.end(), reserved[i].begin(), ::tolower);
    }
    TokenList* tl = new TokenList();                //整个链表存放在堆区 是指向其的指针
    for (int i = 0; i < 26; i++)
        L.push_back('a' + i);
    for (int i = 0; i < 10; i++)
        N.push_back('0' + i);
    ifstream infile;
    infile.open(in);
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
        while (i <= strlen(tmp)) {
            switch (s) {
            case START:
            {
                if (contain(L, tmp[i]))
                    s = INID;
                else if (contain(N, tmp[i]))
                    s = INNUM;
                else if (contain(OP, tmp[i])) {
                    word.clear();
                    word.push_back(tmp[i]);
                    tl->add(Token(line, OP_Type[findIndex(word[0], OP)], word));
                }
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
                while (i <= strlen(tmp) && (contain(L, tmp[i]) || contain(N, tmp[i])))
                    word.push_back(tmp[i++]);
                if (find(reserved.begin(), reserved.end(), word) != reserved.end()) {
                    string lex;
                    for (int j = 0; j < word.size(); j++)
                        lex.push_back(word[j] - 32); //转换成大写
                    tl->add(Token(line, lex, word));
                } else
                    tl->add(Token(line, "ID", word));
                s = START;
                break;
            case INNUM:
                while (i <= strlen(tmp) && contain(N, tmp[i]))
                    word.push_back(tmp[i++]);
                tl->add(Token(line, "INTC", word));
                s = START;
                break;

            case INASSIGN:
                if (i <= strlen(tmp) && tmp[i] == '=') {
                    word.push_back(tmp[i]);
                    tl->add(Token(line, "ASSIGN", word));
                    i++;
                } else
                    tl->add(Token(line, "ERROR", word));
                s = START;
                break;

            case INCOMMENT:
                while (i <= strlen(tmp) && tmp[i] != '}' && tmp[i] != EOF)               //注释不生产单词
                    i++;
                s = START;
                break;
            case INRANGE:
                if (i <= strlen(tmp) && tmp[i] == '.') {
                        word.push_back(tmp[i]);
                        tl->add(Token(line, "UNDERANGE", word));
                        i++;
                }
                tl->add(Token(line, "DOT", word));
                s = START;
                break;

            case INCHAR:
                int f = i;                      //记录起始位置
                while (i <= strlen(tmp) && (contain(L, tmp[i]) || contain(N, tmp[i++])))
                    word.push_back(tmp[i++]);
                if (i < strlen(tmp) && tmp[i] == '\'') {
                    word.push_back(tmp[i++]);
                    tl->add(Token(line, "CHARC", word));
                } else {
                    tl->add(Token(line, "ERROR", word));
                    i = f;    //状态回溯
                }
                s = START;
                break;
            }
        }
    }
    tl->add(Token(line, "ENDFILE", "EOF"));
    ofstream outfile;
    outfile.open(out);
    if (!outfile.is_open())
        printf("输出失败！");
    else {
        Token* out = tl->getList()->data();
        for (int i = 0;i < tl->getList()->size();i++) {
            outfile << out->getLineShow() << " " << out->getLex() << " " << out->getSem() << endl;
            cout << out->getLineShow() << " " << out->getLex() << " " << out->getSem() << endl;
            out++;
        }
    }

    infile.close();
    outfile.close();
    return tl->getList();
}
