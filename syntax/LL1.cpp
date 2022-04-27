#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream> 
#include "LL1.h"
#include "bean/Util.h"
using namespace std;



void readtoken() {
    //if (INDEX == sum)
        //return;
    currenttoken = &tokenList[INDEX];
    //cout << "index:" << INDEX << endl;
    //cout << currenttoken->line << " " << currenttoken->wd.tok << " " << currenttoken->wd.str << endl;
    INDEX++;
}
// 错误输出函数
void printErrorMsg(string reason) {
    string temp;
    temp += reason;
    temp += '\n';
    // ofstream output(OUTerrorLL1);
    // output << temp;
    cout << temp;
    exit(0);
}
// 与printTree没有本质上的区别 因为string在多次覆写之后出现了乱码 故将两次输出拆分成两个函数
// 打印LL1树时如果行数比较多可能会有乱码影响美观 由于时间紧张暂时未能找到问题 我怀疑可能是全角字符导致的
void printTreeLL1(TreeNode* t, int layer, bool islastson, char out[100]) {
    if (t == NULL)
        return;

    int last = 31;
    while (last >= 0) {
        if (t->child[last] != NULL)
            break;
        // 找到最后一个儿子节点
        last--;
    }
    // 如果没有儿子 last == -1
    // 否则last指向最后一个


    for (int i = 0; i < layer; i++) {
        outstrLL1[strlineLL1] += "   ";
    }

    if (layer == 0) {
        outstrLL1[strlineLL1] += "   ";
        outstrLL1[strlineLL1] += t->name;
        //cout << t->name;
    } else {
        if (islastson == true) {
            outstrLL1[strlineLL1] += "└ ";
            outstrLL1[strlineLL1] += t->name;
            //cout << "└ " << t->name;
        } else {
            outstrLL1[strlineLL1] += "├ ";
            outstrLL1[strlineLL1] += t->name;
            //cout << "├ " << t->name;
        }
    }

    outstrLL1[strlineLL1] += "\n";
    strlineLL1++;
    //outstr += "\n";
    //cout << endl;

    for (int i = 0; i < 10; i++) {
        if (i != last)
            printTreeLL1(t->child[i], layer + 1, false, out);
        else
            printTreeLL1(t->child[i], layer + 1, true, out);
    }

    if (layer == 0) {
        // 首先对字符串进行修改
        ///*
        for (int i = 1; i < strlineLL1; i++) {
            int j = 0;
            while (outstrLL1[i][j] != '\n') {
                if (outstrLL1[i - 1][j] == '\n')
                    break;
                if (outstrLL1[i - 1][j] == ch2[0] && outstrLL1[i - 1][j + 1] == ch2[1]) // 这个很重要 不加的话会出问题
                {
                    ; // 上一行是拐
                } else if (outstrLL1[i - 1][j] == ch1[0] && outstrLL1[i - 1][j + 1] == ch1[1] && outstrLL1[i][j] == ' ') {
                    // 上一行是卜 这一行是空格
                    outstrLL1[i][j] = ch3[0];
                    outstrLL1[i][j + 1] = ch3[1];
                } else if (outstrLL1[i - 1][j] == ch3[0] && outstrLL1[i][j] == ' ') {
                    // 上一行是棍 这一行是空格
                    outstrLL1[i][j] = ch3[0];
                    outstrLL1[i][j + 1] = ch3[1];
                }
                j++;
            }
        }
        //*/

        ofstream output(out);
        //char header[3] = { (char)0xEF,(char)0xBB,(char)0xBF };
        //output << header[0] << header[1] << header[2];
        // FILE*fp = fopen("C:\\Users\\Yichuan\\Desktop\\tree.txt", "wt+,ccs=UTF-8");
        for (int i = 0; i < strlineLL1; i++) {
            output << outstrLL1[i];
            //fwrite(&outstr[i], sizeof(char), outstr[i].length(), fp);
        }
        output.close();
        for (int i = 0; i < strlineLL1; i++) {
            cout << outstrLL1[i];
        }
    }
}

/****************************************************************/
/****************************************************************/

// 对process中的一部分代码进行了封装 减小一点工作量
void processAddChild(int istmnl, int lex, string name, int childindex) {
    anlsstack->push(istmnl, lex); // 压栈 逆着压 确保最左在最上
    anlsstack->top->p = new TreeNode(name); // 新建树结点
    currentTree->child[childindex] = anlsstack->top->p; // 连接在父结点上
}

// 将书上104个process封装在一个函数中
void process(int num) {
    if (num == 1) {
        int count = 4;
        processAddChild(1, DOT, "DOT", --count);
        processAddChild(0, ProgramBody, "ProgramBody", --count);
        processAddChild(0, DeclarePart, "DeclarePart", --count);
        processAddChild(0, ProgramHead, "ProgramHead", --count);
    } else if (num == 2) {
        int count = 2;
        processAddChild(0, ProgramName, "ProgramName", --count);
        processAddChild(1, PROGRAM, "PROGRAM", --count);
    } else if (num == 3) {
        int count = 1;
        //		processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(1, ID, "ID", --count);
    } else if (num == 4) {
        int count = 3;
        processAddChild(0, ProcDec, "ProcDec", --count);
        processAddChild(0, VarDec, "VarDec", --count);
        processAddChild(0, TypeDec, "TypeDec", --count);
    } else if (num == 5) {
        ;
    } else if (num == 6) {
        int count = 1;
        processAddChild(0, TypeDeclaration, "TypeDeclaration", --count);
    } else if (num == 7) {
        int count = 2;
        processAddChild(0, TypeDecList, "TypeDecList", --count);
        processAddChild(1, TYPE, "TYPE", --count);
    } else if (num == 8) {
        int count = 5;
        processAddChild(0, TypeDecMore, "TypeDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, TypeName, "TypeName", --count);
        processAddChild(1, EQ, "EQ", --count);
        processAddChild(0, TypeId, "TypeID", --count);
    } else if (num == 9) {
        ;
    } else if (num == 10) {
        int count = 1;
        processAddChild(0, TypeDecList, "TypeDecList", --count);
    } else if (num == 11) {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    } else if (num == 12) {
        int count = 1;
        processAddChild(0, BaseType, "BaseType", --count);
    } else if (num == 13) {
        int count = 1;
        processAddChild(0, StructureType, "StructureType", --count);
    } else if (num == 14) {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    } else if (num == 15) {
        int count = 1;
        processAddChild(1, INTEGER, "INTEGER", --count);
    } else if (num == 16) {
        int count = 1;
        processAddChild(1, CHAR, "CHAR", --count);
    } else if (num == 17) {
        int count = 1;
        processAddChild(0, ArrayType, "ArrayType", --count);
    } else if (num == 18) {
        int count = 1;
        processAddChild(0, RecType, "RecType", --count);
    } else if (num == 19) {
        int count = 8;
        processAddChild(0, BaseType, "BaseType", --count);
        processAddChild(1, OF, "OF", --count);
        processAddChild(1, RMIDPAREN, "RMIDPAREN", --count);
        processAddChild(0, Top, "Top", --count);
        processAddChild(1, UNDERANGE, "UNDERANGE", --count);
        processAddChild(0, Low, "Low", --count);
        processAddChild(1, LMIDPAREN, "LMIDPAREN", --count);
        processAddChild(1, ARRAY, "ARRAY", --count);
    } else if (num == 20) {
        int count = 1;
        processAddChild(1, INTC, "INTC", --count);
    } else if (num == 21) {
        int count = 1;
        processAddChild(1, INTC, "INTC", --count);
    } else if (num == 22) {
        int count = 3;
        processAddChild(1, END, "END", --count);
        processAddChild(0, FieldDecList, "FieldDecList", --count);
        processAddChild(1, RECORD, "RECORD", --count);
    } else if (num == 23) {
        int count = 4;
        processAddChild(0, FieldDecMore, "FieldDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, IdList, "IDList", --count);
        processAddChild(0, BaseType, "BaseType", --count);
    } else if (num == 24) {
        int count = 4;
        processAddChild(0, FieldDecMore, "FieldDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, IdList, "IDList", --count);
        processAddChild(0, ArrayType, "ArrayType", --count);
    } else if (num == 25) {
        ;
    } else if (num == 26) {
        int count = 1;
        processAddChild(0, FieldDecList, "FieldDecList", --count);
    } else if (num == 27) {
        int count = 2;
        processAddChild(0, IdMore, "IDMore", --count);
        processAddChild(1, ID, "ID", --count);
    } else if (num == 28) {
        ;
    } else if (num == 29) {
        int count = 2;
        processAddChild(0, IdList, "IDList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    } else if (num == 30) {
        ;
    } else if (num == 31) {
        int count = 1;
        processAddChild(0, VarDeclaration, "VarDeclaration", --count);
    } else if (num == 32) {
        int count = 2;
        processAddChild(0, VarDecList, "VarDecList", --count);
        processAddChild(1, VAR, "VAR", --count);
    } else if (num == 33) {
        int count = 4;
        processAddChild(0, VarDecMore, "VarDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, VarIdList, "VarIDList", --count);
        processAddChild(0, TypeName, "TypeName", --count);
    } else if (num == 34) {
        ;
    } else if (num == 35) {
        int count = 1;
        processAddChild(0, VarDecList, "VarDecList", --count);
    } else if (num == 36) {
        int count = 2;
        processAddChild(0, VarIdMore, "VarIDMore", --count);
        processAddChild(1, ID, "ID", --count);
    } else if (num == 37) {
        ;
    } else if (num == 38) {
        int count = 2;
        processAddChild(0, VarIdList, "VarIDList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    } else if (num == 39) {
        ;
    } else if (num == 40) {
        int count = 1;
        processAddChild(0, ProcDeclaration, "ProcDeclaration", --count);
    } else if (num == 41) {
        int count = 9;
        processAddChild(0, ProcDecMore, "ProcDecMore", --count);
        processAddChild(0, ProcBody, "ProcBody", --count);
        processAddChild(0, ProcDecPart, "ProcDecPart", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, ParamList, "ParamList", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
        processAddChild(0, ProcName, "ProcName", --count);
        processAddChild(1, PROCEDURE, "PROCEDURE", --count);
    } else if (num == 42) {
        ;
    } else if (num == 43) {
        int count = 1;
        processAddChild(0, ProcDeclaration, "ProcDeclaration", --count);
    } else if (num == 44) {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    } else if (num == 45) {
        ;
    } else if (num == 46) {
        int count = 1;
        processAddChild(0, ParamDecList, "ParamDecList", --count);
    } else if (num == 47) {
        int count = 2;
        processAddChild(0, ParamMore, "ParamMore", --count);
        processAddChild(0, Param, "Param", --count);
    } else if (num == 48) {
        ;
    } else if (num == 49) {
        int count = 2;
        processAddChild(0, ParamDecList, "ParamDecList", --count);
        processAddChild(1, SEMI, "SEMI", --count);
    } else if (num == 50) {
        int count = 2;
        processAddChild(0, FormList, "FormList", --count);
        processAddChild(0, TypeName, "TypeName", --count);
    } else if (num == 51) {
        int count = 3;
        processAddChild(0, FormList, "FormList", --count);
        processAddChild(0, TypeName, "TypeName", --count);
        processAddChild(1, VAR, "VAR", --count);
    } else if (num == 52) {
        int count = 2;
        processAddChild(0, FidMore, "FidMore", --count);
        processAddChild(1, ID, "ID", --count);
    } else if (num == 53) {
        ;
    } else if (num == 54) {
        int count = 2;
        processAddChild(0, FormList, "FormList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    } else if (num == 55) {
        int count = 1;
        processAddChild(0, DeclarePart, "DeclarePart", --count);
    } else if (num == 56) {
        int count = 1;
        processAddChild(0, ProgramBody, "ProgramBody", --count);
    } else if (num == 57) {
        int count = 3;
        processAddChild(1, END, "END", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, BEGIN, "BEGIN", --count);
    } else if (num == 58) {
        int count = 2;
        processAddChild(0, StmMore, "StmMore", --count);
        processAddChild(0, Stm, "Stm", --count);
    } else if (num == 59) {
        ;
    } else if (num == 60) {
        int count = 2;
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, SEMI, "SEMI", --count);
    } else if (num == 61) {
        int count = 1;
        processAddChild(0, ConditionalStm, "ConditionalStm", --count);
    } else if (num == 62) {
        int count = 1;
        processAddChild(0, LoopStm, "LoopStm", --count);
    } else if (num == 63) {
        int count = 1;
        processAddChild(0, InputStm, "InputStm", --count);
    } else if (num == 64) {
        int count = 1;
        processAddChild(0, OutputStm, "OutputStm", --count);
    } else if (num == 65) {
        int count = 1;
        processAddChild(0, ReturnStm, "ReturnStm", --count);
    } else if (num == 66) {
        int count = 2;
        processAddChild(0, AssCall, "AssCall", --count);
        processAddChild(1, ID, "ID", --count);
    } else if (num == 67) {
        int count = 1;
        processAddChild(0, AssignmentRest, "AssignmentRest", --count);
    } else if (num == 68) {
        int count = 1;
        processAddChild(0, CallStmRest, "CallStmRest", --count);
    } else if (num == 69) {
        int count = 3;
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, ASSIGN, "ASSIGN", --count);
        processAddChild(0, VariMore, "VariMore", --count);
    } else if (num == 70) {
        int count = 7;
        processAddChild(1, FI, "FI", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, ELSE, "ELSE", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, THEN, "THEN", --count);
        processAddChild(0, RelExp, "RelExp", --count);
        processAddChild(1, IF, "IF", --count);
    } else if (num == 71) {
        int count = 5;
        //processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(1, ENDWH, "ENDWH", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, DO, "DO", --count);
        processAddChild(0, RelExp, "RelExp", --count);
        processAddChild(1, WHILE, "WHILE", --count);
    } else if (num == 72) {
        int count = 4;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, InVar, "InVar", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
        processAddChild(1, READ, "READ", --count);
    } else if (num == 73) {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    } else if (num == 74) {
        int count = 4;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
        processAddChild(1, WRITE, "WRITE", --count);
    } else if (num == 75) {
        int count = 1;
        processAddChild(1, RETURN, "RETURN", --count);
    } else if (num == 76) {
        int count = 3;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, ActParamList, "ActParamList", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
    } else if (num == 77) {
        ;
    } else if (num == 78) {
        int count = 2;
        processAddChild(0, ActParamMore, "ActParamMore", --count);
        processAddChild(0, Exp, "Exp", --count);
    } else if (num == 79) {
        ;
    } else if (num == 80) {
        int count = 2;
        processAddChild(0, ActParamList, "ActParamList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    } else if (num == 81) {
        int count = 2;
        processAddChild(0, OtherRelE, "OtherRelE", --count);
        processAddChild(0, Exp, "Exp", --count);
    } else if (num == 82) {
        int count = 2;
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(0, CmpOp, "CmpOp", --count);
    } else if (num == 83) {
        int count = 2;
        processAddChild(0, OtherTerm, "OtherTerm", --count);
        processAddChild(0, Term, "Term", --count);
    } else if (num == 84) {
        ;
    } else if (num == 85) {
        int count = 2;
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(0, AddOp, "AddOp", --count);
    } else if (num == 86) {
        int count = 2;
        processAddChild(0, OtherFactor, "OtherFactor", --count);
        processAddChild(0, Factor, "Factor", --count);
    } else if (num == 87) {
        ;
    } else if (num == 88) {
        int count = 2;
        processAddChild(0, Term, "Term", --count);
        processAddChild(0, MultOp, "MultOp", --count);
    } else if (num == 89) {
        int count = 3;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
    } else if (num == 90) {
        int count = 1;
        processAddChild(1, INTC, "INTC", --count);
    } else if (num == 91) {
        int count = 1;
        processAddChild(0, Variable, "Variable", --count);
    } else if (num == 92) {
        int count = 2;
        processAddChild(0, VariMore, "VariMore", --count);
        processAddChild(1, ID, "ID", --count);
    } else if (num == 93) {
        ;
    } else if (num == 94) {
        int count = 3;
        processAddChild(1, RMIDPAREN, "RMIDPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LMIDPAREN, "LMIDPAREN", --count);
    } else if (num == 95) {
        int count = 2;
        processAddChild(0, FieldVar, "FieldVar", --count);
        processAddChild(1, DOT, "DOT", --count);
    } else if (num == 96) {
        int count = 2;
        processAddChild(0, FieldVarMore, "FieldVarMore", --count);
        processAddChild(1, ID, "ID", --count);
    } else if (num == 97) {
        ;
    } else if (num == 98) {
        int count = 3;
        processAddChild(1, RMIDPAREN, "RMIDPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LMIDPAREN, "LMIDPAREN", --count);
    } else if (num == 99) {
        int count = 1;
        processAddChild(1, LT, "LT", --count);
    } else if (num == 100) {
        int count = 1;
        processAddChild(1, EQ, "EQ", --count);
    } else if (num == 101) {
        int count = 1;
        processAddChild(1, PLUS, "PLUS", --count);
    } else if (num == 102) {
        int count = 1;
        processAddChild(1, MINUS, "MINUS", --count);
    } else if (num == 103) {
        int count = 1;
        processAddChild(1, TIMES, "TIMES", --count);
    } else if (num == 104) {
        int count = 1;
        processAddChild(1, OVER, "OVER", --count);
    }
}

//LL1表的初始化
void InitLL1Table() {
    int i, j;

    for (i = 0; i < 104; i++)
        for (j = 0; j < 104; j++)
            LL1Table[i][j] = 0;

    LL1Table[Program][PROGRAM] = 1;

    LL1Table[ProgramHead][PROGRAM] = 2;

    LL1Table[ProgramName][ID] = 3;

    LL1Table[DeclarePart][TYPE] = 4;
    LL1Table[DeclarePart][VAR] = 4;
    LL1Table[DeclarePart][PROCEDURE] = 4;
    LL1Table[DeclarePart][BEGIN] = 4;

    LL1Table[TypeDec][VAR] = 5;
    LL1Table[TypeDec][PROCEDURE] = 5;
    LL1Table[TypeDec][BEGIN] = 5;

    LL1Table[TypeDec][TYPE] = 6;

    LL1Table[TypeDeclaration][TYPE] = 7;

    LL1Table[TypeDecList][ID] = 8;

    LL1Table[TypeDecMore][VAR] = 9;
    LL1Table[TypeDecMore][PROCEDURE] = 9;
    LL1Table[TypeDecMore][BEGIN] = 9;


    LL1Table[TypeDecMore][ID] = 10;

    LL1Table[TypeId][ID] = 11;

    LL1Table[TypeName][INTEGER] = 12;
    LL1Table[TypeName][CHAR] = 12;

    LL1Table[TypeName][ARRAY] = 13;
    LL1Table[TypeName][RECORD] = 13;

    LL1Table[TypeName][ID] = 14;

    LL1Table[BaseType][INTEGER] = 15;

    LL1Table[BaseType][CHAR] = 16;

    LL1Table[StructureType][ARRAY] = 17;

    LL1Table[StructureType][RECORD] = 18;

    LL1Table[ArrayType][ARRAY] = 19;

    LL1Table[Low][INTC] = 20;

    LL1Table[Top][INTC] = 21;

    LL1Table[RecType][RECORD] = 22;

    LL1Table[FieldDecList][INTEGER] = 23;
    LL1Table[FieldDecList][CHAR] = 23;

    LL1Table[FieldDecList][ARRAY] = 24;

    LL1Table[FieldDecMore][END] = 25;

    LL1Table[FieldDecMore][INTEGER] = 26;
    LL1Table[FieldDecMore][CHAR] = 26;
    LL1Table[FieldDecMore][ARRAY] = 26;

    LL1Table[IdList][ID] = 27;

    LL1Table[IdMore][SEMI] = 28;

    LL1Table[IdMore][COMMA] = 29;

    LL1Table[VarDec][PROCEDURE] = 30;
    LL1Table[VarDec][BEGIN] = 30;

    LL1Table[VarDec][VAR] = 31;

    LL1Table[VarDeclaration][VAR] = 32;

    LL1Table[VarDecList][INTEGER] = 33;
    LL1Table[VarDecList][CHAR] = 33;
    LL1Table[VarDecList][ARRAY] = 33;
    LL1Table[VarDecList][RECORD] = 33;
    LL1Table[VarDecList][ID] = 33;

    LL1Table[VarDecMore][PROCEDURE] = 34;
    LL1Table[VarDecMore][BEGIN] = 34;


    LL1Table[VarDecMore][INTEGER] = 35;
    LL1Table[VarDecMore][CHAR] = 35;
    LL1Table[VarDecMore][ARRAY] = 35;
    LL1Table[VarDecMore][RECORD] = 35;
    LL1Table[VarDecMore][ID] = 35;

    LL1Table[VarIdList][ID] = 36;

    LL1Table[VarIdMore][SEMI] = 37;

    LL1Table[VarIdMore][COMMA] = 38;

    LL1Table[ProcDec][BEGIN] = 39;

    LL1Table[ProcDec][PROCEDURE] = 40;

    LL1Table[ProcDeclaration][PROCEDURE] = 41;

    LL1Table[ProcDecMore][BEGIN] = 42;

    LL1Table[ProcDecMore][PROCEDURE] = 43;

    LL1Table[ProcName][ID] = 44;

    LL1Table[ParamList][RPAREN] = 45;

    LL1Table[ParamList][INTEGER] = 46;
    LL1Table[ParamList][CHAR] = 46;
    LL1Table[ParamList][ARRAY] = 46;
    LL1Table[ParamList][RECORD] = 46;
    LL1Table[ParamList][ID] = 46;
    LL1Table[ParamList][VAR] = 46;

    LL1Table[ParamDecList][INTEGER] = 47;
    LL1Table[ParamDecList][CHAR] = 47;
    LL1Table[ParamDecList][ARRAY] = 47;
    LL1Table[ParamDecList][RECORD] = 47;
    LL1Table[ParamDecList][ID] = 47;
    LL1Table[ParamDecList][VAR] = 47;

    LL1Table[ParamMore][RPAREN] = 48;

    LL1Table[ParamMore][SEMI] = 49;

    LL1Table[Param][INTEGER] = 50;
    LL1Table[Param][CHAR] = 50;
    LL1Table[Param][ARRAY] = 50;
    LL1Table[Param][RECORD] = 50;
    LL1Table[Param][ID] = 50;

    LL1Table[Param][VAR] = 51;

    LL1Table[FormList][ID] = 52;

    LL1Table[FidMore][SEMI] = 53;
    LL1Table[FidMore][RPAREN] = 53;

    LL1Table[FidMore][COMMA] = 54;

    LL1Table[ProcDecPart][TYPE] = 55;
    LL1Table[ProcDecPart][VAR] = 55;
    LL1Table[ProcDecPart][PROCEDURE] = 55;
    LL1Table[ProcDecPart][BEGIN] = 55;

    LL1Table[ProcBody][BEGIN] = 56;

    LL1Table[ProgramBody][BEGIN] = 57;

    LL1Table[StmList][ID] = 58;
    LL1Table[StmList][IF] = 58;
    LL1Table[StmList][WHILE] = 58;
    LL1Table[StmList][RETURN] = 58;
    LL1Table[StmList][READ] = 58;
    LL1Table[StmList][WRITE] = 58;

    LL1Table[StmMore][END] = 59;
    LL1Table[StmMore][ENDWH] = 59;
    LL1Table[StmMore][ELSE] = 59;
    LL1Table[StmMore][FI] = 59;

    LL1Table[StmMore][SEMI] = 60;

    LL1Table[Stm][IF] = 61;

    LL1Table[Stm][WHILE] = 62;

    LL1Table[Stm][READ] = 63;

    LL1Table[Stm][WRITE] = 64;

    LL1Table[Stm][RETURN] = 65;

    LL1Table[Stm][ID] = 66;

    LL1Table[AssCall][ASSIGN] = 67;
    LL1Table[AssCall][LMIDPAREN] = 67;
    LL1Table[AssCall][DOT] = 67;


    LL1Table[AssCall][LPAREN] = 68;

    LL1Table[AssignmentRest][ASSIGN] = 69;
    LL1Table[AssignmentRest][LMIDPAREN] = 69;
    LL1Table[AssignmentRest][DOT] = 69;

    LL1Table[ConditionalStm][IF] = 70;


    LL1Table[LoopStm][WHILE] = 71;

    LL1Table[InputStm][READ] = 72;

    LL1Table[InVar][ID] = 73;

    LL1Table[OutputStm][WRITE] = 74;

    LL1Table[ReturnStm][RETURN] = 75;

    LL1Table[CallStmRest][LPAREN] = 76;

    LL1Table[ActParamList][RPAREN] = 77;

    LL1Table[ActParamList][ID] = 78;
    LL1Table[ActParamList][INTC] = 78;
    LL1Table[ActParamList][LPAREN] = 78;

    LL1Table[ActParamMore][RPAREN] = 79;

    LL1Table[ActParamMore][COMMA] = 80;

    LL1Table[RelExp][LPAREN] = 81;
    LL1Table[RelExp][INTC] = 81;
    LL1Table[RelExp][ID] = 81;

    LL1Table[OtherRelE][LT] = 82;
    LL1Table[OtherRelE][EQ] = 82;

    LL1Table[Exp][LPAREN] = 83;
    LL1Table[Exp][INTC] = 83;
    LL1Table[Exp][ID] = 83;

    LL1Table[OtherTerm][LT] = 84;
    LL1Table[OtherTerm][EQ] = 84;
    LL1Table[OtherTerm][THEN] = 84;
    LL1Table[OtherTerm][DO] = 84;
    LL1Table[OtherTerm][RPAREN] = 84;
    LL1Table[OtherTerm][END] = 84;
    LL1Table[OtherTerm][SEMI] = 84;
    LL1Table[OtherTerm][COMMA] = 84;
    LL1Table[OtherTerm][ENDWH] = 84;
    LL1Table[OtherTerm][ELSE] = 84;
    LL1Table[OtherTerm][FI] = 84;
    LL1Table[OtherTerm][RMIDPAREN] = 84;


    LL1Table[OtherTerm][PLUS] = 85;
    LL1Table[OtherTerm][MINUS] = 85;

    LL1Table[Term][LPAREN] = 86;
    LL1Table[Term][INTC] = 86;
    LL1Table[Term][ID] = 86;

    LL1Table[OtherFactor][PLUS] = 87;
    LL1Table[OtherFactor][MINUS] = 87;
    LL1Table[OtherFactor][LT] = 87;
    LL1Table[OtherFactor][EQ] = 87;
    LL1Table[OtherFactor][THEN] = 87;
    LL1Table[OtherFactor][ELSE] = 87;
    LL1Table[OtherFactor][FI] = 87;
    LL1Table[OtherFactor][DO] = 87;
    LL1Table[OtherFactor][ENDWH] = 87;
    LL1Table[OtherFactor][RPAREN] = 87;
    LL1Table[OtherFactor][END] = 87;
    LL1Table[OtherFactor][SEMI] = 87;
    LL1Table[OtherFactor][COMMA] = 87;
    LL1Table[OtherFactor][RMIDPAREN] = 87;

    LL1Table[OtherFactor][TIMES] = 88;
    LL1Table[OtherFactor][OVER] = 88;

    LL1Table[Factor][LPAREN] = 89;

    LL1Table[Factor][INTC] = 90;

    LL1Table[Factor][ID] = 91;

    LL1Table[Variable][ID] = 92;

    LL1Table[VariMore][ASSIGN] = 93;
    LL1Table[VariMore][TIMES] = 93;
    LL1Table[VariMore][OVER] = 93;
    LL1Table[VariMore][PLUS] = 93;
    LL1Table[VariMore][MINUS] = 93;
    LL1Table[VariMore][LT] = 93;
    LL1Table[VariMore][EQ] = 93;
    LL1Table[VariMore][THEN] = 93;
    LL1Table[VariMore][ELSE] = 93;
    LL1Table[VariMore][FI] = 93;
    LL1Table[VariMore][DO] = 93;
    LL1Table[VariMore][ENDWH] = 93;
    LL1Table[VariMore][RPAREN] = 93;
    LL1Table[VariMore][END] = 93;
    LL1Table[VariMore][SEMI] = 93;
    LL1Table[VariMore][COMMA] = 93;
    LL1Table[VariMore][RMIDPAREN] = 93;

    LL1Table[VariMore][LMIDPAREN] = 94;

    LL1Table[VariMore][DOT] = 95;

    LL1Table[FieldVar][ID] = 96;

    LL1Table[FieldVarMore][ASSIGN] = 97;
    LL1Table[FieldVarMore][TIMES] = 97;
    LL1Table[FieldVarMore][OVER] = 97;
    LL1Table[FieldVarMore][PLUS] = 97;
    LL1Table[FieldVarMore][MINUS] = 97;
    LL1Table[FieldVarMore][LT] = 97;
    LL1Table[FieldVarMore][EQ] = 97;
    LL1Table[FieldVarMore][THEN] = 97;
    LL1Table[FieldVarMore][ELSE] = 97;
    LL1Table[FieldVarMore][FI] = 97;
    LL1Table[FieldVarMore][DO] = 97;
    LL1Table[FieldVarMore][ENDWH] = 97;
    LL1Table[FieldVarMore][RPAREN] = 97;
    LL1Table[FieldVarMore][END] = 97;
    LL1Table[FieldVarMore][SEMI] = 97;
    LL1Table[FieldVarMore][COMMA] = 97;

    LL1Table[FieldVarMore][LMIDPAREN] = 98;

    LL1Table[CmpOp][LT] = 99;

    LL1Table[CmpOp][EQ] = 100;

    LL1Table[AddOp][PLUS] = 101;

    LL1Table[AddOp][MINUS] = 102;

    LL1Table[MultOp][TIMES] = 103;

    LL1Table[MultOp][OVER] = 104;

}

TreeNode* programLL1() {
    INDEX = 0;
    // readtoken 再次使用 重新赋值为0

    NonTerminal stacktopN;

    Terminal stacktopT;

    InitLL1Table(); // 初始化LL1分析表

    TreeNode* root = NULL; // 语法树的根

    anlsstack->push(0, Program); // 分析栈 压入初始结点Program
    anlsstack->top->p = new TreeNode("Program"); // 新建树节点
    root = anlsstack->top->p; // 指向根 便于最后返回整棵树

    readtoken(); // 读出INDEX=0对应token

    while (!anlsstack->isEmpty) // 循环结束条件
    {
        if (anlsstack->top->ntflag == 1) // 终极符
        {
            stacktopT = anlsstack->top->t; // 记录一下终极符是什么
            //cout << "tok:" << currenttoken->wd.tok << endl;
            if (stacktopT == currenttoken->wd.tok) // 比较一下 如果正确.

            {
                //cout << "current_token:" << currenttoken->wd.str << endl;
                anlsstack->top->p->name = currenttoken->wd.str; // 把节点的名字用实际代码命名
                anlsstack->top->p->tk = currenttoken; // 因为是终极符 所以已经可以把token给树节点了
                // 上面这步很重要
                //cout << "top:" << anlsstack->top->p->name << endl;
                anlsstack->pop(); // 弹出分析栈
                //cout << "top:"<< anlsstack->top->ntflag<<" "<< "n:" << anlsstack->top->n<<" " << "t:" << anlsstack->top->t << endl;
                //cout << "current_token:"<<currenttoken->wd.str << endl;
                //cout << "--------------------------------------------------"<<endl;
                readtoken(); // 再往后读一个token
                //cout << "--------------------------------------------------"<<endl;
            } else {
                //终极符不匹配
                string temp = "Line ";
                temp += to_string(currenttoken->line);
                temp += " ";
                temp += "\"";
                temp += currenttoken->wd.str;
                temp += "\" ";
                temp += "& T:";
                temp += to_string(currenttoken->wd.tok);
                temp += " NotMatch Error!\n";
                printErrorMsg(temp);
            }
        } else // 非终极符
        {
            stacktopN = anlsstack->top->n; // 存一下非终极符

            int num = LL1Table[stacktopN][currenttoken->wd.tok]; // LL1
            //cout << "num:" << num << endl;
            currentTree = anlsstack->top->p; // 记录一下将要被弹出的树节点
            // 后面将要扩展它的子树


            anlsstack->pop(); // 弹出分析栈

            process(num);

            //readtoken(); // 再往后读一个token
        }
    }
    if (currenttoken->wd.tok != ENDFILE) {
        string temp;
        temp += "NotEndFile Error!\n";
        printErrorMsg(temp);
    }

    return  root;
}

/****************************************************************/
/****************************************************************/
TreeNode* LL1(char* in,char* out) {
    int a;
    string t1, t2;
    ifstream infile;
    infile.open(in,ios::in);
    if (!infile.is_open()) {
        printf("打开文件失败！");
        return NULL;
    }
    while (!infile.eof()) {
        infile >> a >> t1 >> t2;
        tokenList[sum].line = a;
        tokenList[sum].wd.str = t2;
        for (int i = 0; i < 42; i++) {
            if (reservedWords[i].str == t1)
                tokenList[sum].wd.tok = reservedWords[i].tok;
        }
        //cout << tokenList[sum].line << " " << t1 << " " <<tokenList[sum].wd.str << "\n";
        sum++;
    }
    treeroot = programLL1();

    return treeroot;
}
