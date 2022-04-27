#ifndef sem
#define sem 1
#include "../syntax/LL1.h"
#include "../syntax/bean/TreeNode.h"
#include<iostream>
#include<fstream>
using namespace std;
/****************************************************************/
// 语义分析
class SymbolTable;
// 符号表的记录节点
struct SymbolRecord {
    string name; // 名字
    string kind; // 三种类型
    string type; // 具体类型
    token* tk; // 指向tokenlist
    SymbolTable* next;
};

void printError(string reason);
void checkParam(TreeNode* t);
void typeSaveTable(TreeNode* t, SymbolTable* st);
void varSaveTable(TreeNode* t, SymbolTable* st);
void procSaveTable(TreeNode* t, SymbolTable* st, SymbolTable* nextst);
void checkProgramBody(TreeNode* t, SymbolTable* st);
void checkAssignAndDeclaration(TreeNode* t);
void checkAssignLeftRight(TreeNode* t);
void checkRange();
void semanticAnalysis(TreeNode* treeroot, string out);
// 符号表
void printErrorMsg(string);
string outputstr;
class SymbolTable {
public:
    SymbolTable() {
        index = 0;
        paramcount = 0;
    }
    SymbolRecord* table[512];
    int index;
    int paramcount;
    void addRecord(string name, string kind, string type, token* tk, SymbolTable* next = NULL) {
        for (int i = 0; i < index; i++) {
            if (name == table[i]->name) {
                string temp = "Line ";
                temp += to_string(tk->line);
                temp += " ";
                temp += "\"";
                temp += name;
                temp += "\" ";
                temp += "RepeatDeclaration Error!\n";
                printError(temp);
                return;
            }
        }
        table[index] = new SymbolRecord();
        char temp[100];

        table[index]->name = name;
        table[index]->kind = kind;
        table[index]->type = type;
        table[index]->tk = tk;
        table[index]->next = next;
        index++;
    }
    void printTable(int layer = 0) {
        for (int i = 0; i < index; i++) {
            for (int i = 0; i < layer; i++) {
                cout << "\t\t\t\t\t";
                outputstr += "\t\t\t\t\t";
            }

            cout << table[i]->name
                << "\t"
                << table[i]->kind
                << "\t"
                << table[i]->type << endl;

            outputstr += table[i]->name;
            outputstr += "\t";
            outputstr += table[i]->kind;
            outputstr += "\t";
            outputstr += table[i]->type;
            outputstr += "\n";

            if (table[i]->next) {
                //cout << endl;
                //outputstr += "\n";
                table[i]->next->printTable(layer + 1);
                cout << endl;
                outputstr += "\n";
            }
        }
    }
};

SymbolTable* smbltable = new SymbolTable();
string bstr[16];
int bindex = 0;
#endif