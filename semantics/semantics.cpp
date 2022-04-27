#include<iostream>
#include<array>
#include "bean/Symbol.h"
#include "../syntax/bean/TreeNode.h"
#include <stdlib.h>
#include<vector>
#include"../lex/bean/Token.h"
using namespace std;

int level = -1;
int scope[1000] = { 0 }; //scope stack ptrs
int scopeNum = -1;
int initOff = 0;
vector<Symbol*> sl;
list<string> semanticError;

void createTable()
//initial the scope stack
{
    level++;
    scopeNum++;
    scope[scopeNum] = sl.size();
    initOff = 7;
}

void destroyTabel() {
    level--;
}

bool enter(TreeNode* leaf, Info* information) {
    int line = 0;
    if (findEntry(leaf->name))
        semanticError.push_back((leaf->tk->line + '0') + *leaf->name.data() + "repeated declaration\n");
    else {
        Symbol* s = (Symbol*) malloc(sizeof(Symbol));
        s->name = leaf->name;
        s->information = information;
        s->more;
        sl.push_back(s);
    }
}

bool findEntry(string name)
{
    for (int i = scope[scopeNum];i < sl.size();i++)
        if (sl[i]->name.compare(name) == 0)
            return true;
    return false;

}

void printSymbTable() {
    printf("+--------SymbolTable----------+");
    for (Symbol* s : sl)
        printf("|%-15s|%5d|%-15s|", s->name,
            s->information->size, type_name[s->information->kind]);
}

void typeSaveTable(TreeNode* t) {
    if (t == NULL)
        return;
    //cout << t->name << endl;
    if (t->name == "TypeDecList") {
        //cout << t->childIndex;
        string name = t->child[0]->child[0]->name;
        string tempstr = t->child[2]->child[0]->child[0]->name;

        string type;
        if (tempstr == "integer" || tempstr == "char")
            type = t->child[2]->child[0]->child[0]->tk->wd.str;
        else
            type = t->child[2]->child[0]->child[0]->child[0]->tk->wd.str;


        string kind = "typeKind";
        

        //cout << name;
        enter(name, kind, type);
    }

    for (int i = 0; i < 10; i++) {
        typeSaveTable(t->child[i], st);
    }
}

void varSaveTable(TreeNode* t, SymbolTable* st) {
    if (t == NULL)
        return;

    if (t->name == "VarDecList") {
        string type;
        if (t->child[0]->child[0]->name == "BaseType") {
            type = t->child[0]->child[0]->child[0]->tk->wd.str;
        } else {
            type = t->child[0]->child[0]->tk->wd.str;
        }

        TreeNode* p = t->child[1];
        while (p->name == "VarIDList") {
            //cout << t->childIndex;
            string name = p->child[0]->name;
            Token* tk = p->child[0]->tk;
            string kind = "varKind ";

            st->addRecord(name, kind, type, tk);

            if (p->child[1]->child[1] == NULL)
                break;
            else
                p = p->child[1]->child[1];
        }
    }

    for (int i = 0; i < 10; i++) {
        varSaveTable(t->child[i], st);
    }
}

void procSaveTable(TreeNode* t, SymbolTable* st, SymbolTable* nextst) {
    if (t == NULL)
        return;

    if (t->name == "ProcDeclaration") {
        nextst = new SymbolTable();//新的表
        string name = t->child[1]->child[0]->name;
        Token* tk = t->child[1]->child[0]->tk;
        string kind = "procKind";
        string type = t->child[0]->tk->wd.str;

        st->addRecord(name, kind, type, tk, nextst);

        TreeNode* temp = t->child[3];
        if (temp->child[0] == NULL) {
            varSaveTable(t->child[6], nextst);
            return;
        }

        if (temp->child[0]->child[0]->child[0]->child[0]->name == "BaseType") {
            type = temp->child[0]->child[0]->child[0]->child[0]->child[0]->tk->wd.str;
            name = temp->child[0]->child[0]->child[1]->child[0]->name;
            tk = temp->child[0]->child[0]->child[1]->child[0]->tk;
            kind = "varKind ";
            nextst->addRecord(name, kind, type, tk);
            nextst->paramcount++;

            temp = temp->child[0]->child[1];//parammore
        } else {
            type = temp->child[0]->child[0]->child[0]->child[0]->tk->wd.str;
            name = temp->child[0]->child[0]->child[1]->child[0]->name;
            tk = temp->child[0]->child[0]->child[1]->child[0]->tk;
            kind = "varKind ";
            nextst->addRecord(name, kind, type, tk);
            nextst->paramcount++;

            temp = temp->child[0]->child[1];
        }

        while (temp->child[1]) {
            temp = temp->child[1];//param dec list
            if (temp->child[0]->child[0]->child[0]->name == "BaseType") {
                type = temp->child[0]->child[0]->child[0]->child[0]->tk->wd.str;
                name = temp->child[0]->child[1]->child[0]->name;
                tk = temp->child[0]->child[1]->child[0]->tk;
                kind = "varKind ";
                nextst->addRecord(name, kind, type, tk);
                nextst->paramcount++;

                temp = temp->child[1];
            } else {
                type = temp->child[0]->child[0]->child[0]->tk->wd.str;
                name = temp->child[0]->child[1]->child[0]->name;
                tk = temp->child[0]->child[1]->child[0]->tk;
                kind = "varKind ";
                nextst->addRecord(name, kind, type, tk);
                nextst->paramcount++;

                temp = temp->child[1];
            }
        }

        varSaveTable(t->child[6], nextst);
    }

    for (int i = 0; i < 10; i++) {
        procSaveTable(t->child[i], st, nextst);
    }
}
//

void semanticsAnalysis() {
    for (int i = 0; t->child[i] != NULL; i++) {
        TreeNode* temp = t->child[i];
        if (temp->name == "TypeDec")
            typeSaveTable(temp);
        else if (temp->name == "VarDec")
            varSaveTable(temp);
        else if (temp->name == "ProcDec") {
            SymbolTable* p = NULL;
            procSaveTable(temp, smbltable, p);
        }
    }

}