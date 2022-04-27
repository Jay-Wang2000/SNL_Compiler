#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include "recursiveDe.h"
#include "../lex/bean/Token.h"
#include "./bean/Util.h"
using namespace std;
char error[100] = "D:\\qtp\\SNL_Compiler\\out\\errorDe.txt"; // 报错
Token Token_List[10004];
int Token_Index = 0;
string outstr[512];
Token Token_Read() {
    return Token_List[Token_Index++];
}

TreeNode* matchToken(string tok) {
    Token tmp = Token_Read();

    if (tmp.getLex() == tok) {

        TreeNode* t = new TreeNode(tmp.getLex());
        t->Add_Child(new TreeNode(tmp.getSem()));
        return t;
    } else
        return NULL;
}
string OUTerror = "D:\\qtp\\SNL_Compiler\\out\\error_recurDe.txt";

void Print_Error(string reason) {
    string temp;
    temp += reason;
    temp += '\n';
    ofstream output(OUTerror);
    output << temp;
    cout << temp;
    output.close();
    exit(0);
}

TreeNode* program() {
    Token_Index = 0;
    TreeNode* t = new TreeNode("Program");
    t->Add_Child(programHead());
    t->Add_Child(declarePart());
    t->Add_Child(programBody());
    return t;
}

TreeNode* programHead() {
    TreeNode* t = new TreeNode("ProgramHead");
    t->Add_Child(matchToken("PROGRAM"));
    t->Add_Child(matchToken("ID"));
    return t;
}

TreeNode* declarePart() {
    TreeNode* t = new TreeNode("Declare");
    t->Add_Child(typeDecPart());
    t->Add_Child(varDecPart());
    t->Add_Child(procDecpart());
    return t;
}

TreeNode* programBody() {
    TreeNode* t = new TreeNode("ProgramBody");
    t->Add_Child(matchToken("BEGIN"));
    t->Add_Child(stmList());
    t->Add_Child(matchToken("END1"));
    return t;
}

TreeNode* typeDecPart() {
    TreeNode* t= new TreeNode("TypeDec");
    if (Token_List[Token_Index].getLex() == "TYPE") {
        t->Add_Child(typeDec());
    }
    return t;
}

TreeNode* varDecPart() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "VAR") {
        t = new TreeNode("VarDec");
        t->Add_Child(varDec());
    }
    return t;
}

TreeNode* procDecpart() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "PROCEDURE") {
        t = new TreeNode("ProcDecPart");
        t->Add_Child(procDec());
    }
    return t;
}

TreeNode* typeDec() {
    TreeNode* t = new TreeNode("TypeDec");
    t->Add_Child(matchToken("TYPE"));
    t->Add_Child(typeDecList());
    return t;
}

TreeNode* typeDecList() {
    TreeNode* t = new TreeNode("TypeDecList");
    t->Add_Child(matchToken("ID"));
    t->Add_Child(matchToken("EQ"));
    t->Add_Child(typeDef());
    t->Add_Child(matchToken("SEMI"));
    t->Add_Child(typeDecMore());
    return t;
}

TreeNode* typeDef() {
    TreeNode* t = new TreeNode("TypeDef");
    if ((Token_List[Token_Index].getLex() == "INTEGER") ||
        (Token_List[Token_Index].getLex() == "CHAR1")) {
        t->Add_Child(baseType());
        return t;
    } else if ((Token_List[Token_Index].getLex() == "ARRAY") ||
        (Token_List[Token_Index].getLex() == "RECORD")) {
        t->Add_Child(structureType());
        return t;
    } else {
        t->Add_Child(matchToken("ID"));
        return t;
    }
}

TreeNode* typeDecMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "ID") {
        t = new TreeNode("TypeDecMore");
        t->Add_Child(typeDecList());
    }

    return t;
}

TreeNode* baseType() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "INTEGER") {
        t = new TreeNode("BaseType");
        t->Add_Child(matchToken("INTEGER"));
        return t;
    } else if (Token_List[Token_Index].getLex() == "CHAR1") {
        t = new TreeNode("BaseType");
        t->Add_Child(matchToken("CHAR1"));
        return t;
    }

    Token_Read();
    string temp = "Line ";
    temp += to_string(Token_List[Token_Index].getLineShow());
    temp += " ";
    temp += "\"";
    temp += Token_List[Token_Index].getLex();
    temp += "\" ";
    temp += "BaseType Error!\n";
    Print_Error(temp);
    return t;
}

TreeNode* structureType() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "ARRAY") {
        t = new TreeNode("StructureType");
        t->Add_Child(arrayType());
        return t;
    }
    if (Token_List[Token_Index].getLex() == "RECORD") {
        t = new TreeNode("StructureType");
        t->Add_Child(recType());
        return t;
    }
    return t;
}

TreeNode* arrayType() {
    TreeNode* t = new TreeNode("ArrayType");
    t->Add_Child(matchToken("ARRAY"));
    t->Add_Child(matchToken("LMIDPAREN"));
    t->Add_Child(matchToken("INTC"));
    t->Add_Child(matchToken("UNDERANGE"));
    t->Add_Child(matchToken("INTC"));
    t->Add_Child(matchToken("RMIDPAREN"));
    t->Add_Child(matchToken("OF"));
    t->Add_Child(baseType());

    return t;
}

TreeNode* recType() {
    TreeNode* t = new TreeNode("RecType");
    t->Add_Child(matchToken("RECORD"));
    t->Add_Child(fieldDecList());
    t->Add_Child(matchToken("END1"));
    return t;
}

TreeNode* fieldDecList() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "INTEGER" ||
        Token_List[Token_Index].getLex() == "CHAR1") {
        t = new TreeNode("FieldDecList");
        t->Add_Child(baseType());
        t->Add_Child(IDList());
        t->Add_Child(matchToken("SEMI"));
        t->Add_Child(fieldDecMore());
        return t;
    } else if (Token_List[Token_Index].getLex() == "ARRAY") {
        t = new TreeNode("FieldDecList");
        t->Add_Child(arrayType());
        t->Add_Child(IDList());
        t->Add_Child(matchToken("SEMI"));
        t->Add_Child(fieldDecMore());
        return t;
    }
    Token_Read();
    string temp = "Line ";
    temp += to_string(Token_List[Token_Index].getLineShow());
    temp += " ";
    temp += "FieldDeclareList Error!\n";
    Print_Error(temp);
    return t;
}

TreeNode* IDList() {
    TreeNode* t = new TreeNode("IDList");
    t->Add_Child(matchToken("ID"));
    t->Add_Child(IDMore());
    return t;
}

TreeNode* fieldDecMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "INTEGER" ||
        Token_List[Token_Index].getLex() == "CHAR1" ||
        Token_List[Token_Index].getLex() == "ARRAY") {
        t = new TreeNode("FieldDecMore");
        t->Add_Child(fieldDecList());
    }
    return t;
}

TreeNode* IDMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "COMMA") {
        t = new TreeNode("IDMore");
        t->Add_Child(matchToken("COMMA"));
        t->Add_Child(IDList());
    }
    return t;
}

TreeNode* varDec() {
    TreeNode* t = new TreeNode("VarDec");
    t->Add_Child(matchToken("VAR"));
    t->Add_Child(varDecList());
    return t;
}

TreeNode* varDecList() {
    TreeNode* t = new TreeNode("VarDecList");
    t->Add_Child(typeDef());
    t->Add_Child(varIDList());
    t->Add_Child(matchToken("SEMI"));
    t->Add_Child(varDecMore());
    return t;
}

TreeNode* varIDList() {
    TreeNode* t = new TreeNode("VarIDList");
    t->Add_Child(matchToken("ID"));
    t->Add_Child(varIDMore());
    return t;
}

TreeNode* varDecMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "INTEGER" ||
        Token_List[Token_Index].getLex() == "CHAR1" ||
        Token_List[Token_Index].getLex() == "ARRAY" ||
        Token_List[Token_Index].getLex() == "RECORD" ||
        Token_List[Token_Index].getLex() == "ID") {
        t = new TreeNode("VarDecMore");
        t->Add_Child(varDecList());
    }
    return t;
}

TreeNode* varIDMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "COMMA") {
        t = new TreeNode("VarIDMore");
        t->Add_Child(matchToken("COMMA"));
        t->Add_Child(varIDList());
    }
    return t;
}

TreeNode* procDec() {
    TreeNode* t = new TreeNode("ProcDec");
    t->Add_Child(matchToken("PROCEDURE"));
    t->Add_Child(matchToken("ID"));
    t->Add_Child(matchToken("LPAREN"));
    t->Add_Child(paramList());
    t->Add_Child(matchToken("RPAREN"));
    t->Add_Child(matchToken("SEMI"));
    t->Add_Child(procDecPart());
    t->Add_Child(procBody());
    t->Add_Child(procDecpart());
    return t;
}

TreeNode* paramList() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "INTEGER" ||
        Token_List[Token_Index].getLex() == "CHAR1" ||
        Token_List[Token_Index].getLex() == "ARRAY" ||
        Token_List[Token_Index].getLex() == "RECORD" ||
        Token_List[Token_Index].getLex() == "VAR" ||
        Token_List[Token_Index].getLex() == "ID") {
        t = new TreeNode("ParamList");
        t->Add_Child(paramDecList());
    }
    return t;
}

TreeNode* procDecPart() {
    TreeNode* t = new TreeNode("ProcDecPart");
    t->Add_Child(declarePart());
    return t;
}

TreeNode* procBody() {
    TreeNode* t = new TreeNode("ProcBody");
    t->Add_Child(programBody());
    return t;
}

TreeNode* paramDecList() {
    TreeNode* t = new TreeNode("ParamDecList");
    t->Add_Child(param());
    t->Add_Child(paramMore());
    return t;
}

TreeNode* param() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "INTEGER" ||
        Token_List[Token_Index].getLex() == "CHAR1" ||
        Token_List[Token_Index].getLex() == "ARRAY" ||
        Token_List[Token_Index].getLex() == "ECORD" ||
        Token_List[Token_Index].getLex() == "ID") {
        t = new TreeNode("Param");
        t->Add_Child(typeDef());
        t->Add_Child(formList());
        return t;
    } else if (Token_List[Token_Index].getLex() == "VAR") {
        t = new TreeNode("Param");
        t->Add_Child(matchToken("VAR"));
        t->Add_Child(typeDef());
        t->Add_Child(formList());
        return t;
    }
    Token_Read();
    string temp = "Line ";
    temp += to_string(Token_List[Token_Index].getLineShow());
    temp += " ";
    temp += "\"";
    temp += Token_List[Token_Index].getLex();
    temp += "\" ";
    temp += "Param error\n";
    Print_Error(temp);
    return t;
}

TreeNode* paramMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "SEMI") {
        t = new TreeNode("ParamMore");
        t->Add_Child(matchToken("SEMI"));
        t->Add_Child(paramDecList());
    }
    return t;
}

TreeNode* formList() {
    TreeNode* t = new TreeNode("FormList");
    t->Add_Child(matchToken("ID"));
    t->Add_Child(fidMore());
    return t;
}

TreeNode* fidMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "COMMA") {
        t = new TreeNode("FidMore");
        t->Add_Child(matchToken("COMMA"));
        t->Add_Child(formList());
    }
    return t;
}

TreeNode* stmList() {
    TreeNode* t = new TreeNode("StmList");
    t->Add_Child(stm());
    t->Add_Child(stmMore());
    return t;
}

TreeNode* stm() {
    TreeNode* t = new TreeNode("Stm");
    if (Token_List[Token_Index].getLex() == "IF") {
        t->Add_Child(conditionalStm());
        return t;
    } else if (Token_List[Token_Index].getLex() == "WHILE") {
        t->Add_Child(loopStm());
        return t;
    } else if (Token_List[Token_Index].getLex() == "READ") {
        t->Add_Child(inputStm());
        return t;
    } else if (Token_List[Token_Index].getLex() == "WRITE") {
        t->Add_Child(outputStm());
        return t;
    } else if (Token_List[Token_Index].getLex() == "RETURN1") {
        t->Add_Child(returnStm());
        return t;
    } else if (Token_List[Token_Index].getLex() == "ID") {
        t->Add_Child(matchToken("ID"));
        t->Add_Child(assCall());
        return t;
    }
    return NULL;
}

TreeNode* stmMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "SEMI") {
        t = new TreeNode("StmMore");
        t->Add_Child(matchToken("SEMI"));
        t->Add_Child(stmList());
    }
    return t;
}

TreeNode* conditionalStm() {
    TreeNode* t = new TreeNode("ConditionalStm");
    t->Add_Child(matchToken("IF"));
    t->Add_Child(exp());
    if (Token_List[Token_Index].getLex() == "LT") {
        t->Add_Child(matchToken("LT"));
    } else if (Token_List[Token_Index].getLex() == "EQ") {
        t->Add_Child(matchToken("EQ"));
    } else {
        Token_Read();
        string temp = "Line ";
        temp += to_string(Token_List[Token_Index].getLineShow());
        temp += " ";
        temp += "\"";
        temp += Token_List[Token_Index].getLex();
        temp += "\" ";
        temp += "ConditionalStm Error!\n";
        Print_Error(temp);
    }
    t->Add_Child(exp());
    t->Add_Child(matchToken("THEN"));
    t->Add_Child(stmList());
    t->Add_Child(matchToken("ELSE"));
    t->Add_Child(stmList());
    t->Add_Child(matchToken("FI"));
    return t;
}

TreeNode* loopStm() {
    TreeNode* t = new TreeNode("LoopStm");
    t->Add_Child(matchToken("WHILE"));
    t->Add_Child(exp());
    if (Token_List[Token_Index].getLex() == "LT") {
        t->Add_Child(matchToken("LT"));
    } else if (Token_List[Token_Index].getLex() == "EQ") {
        t->Add_Child(matchToken("EQ"));
    } else {
        Token_Read();
        string temp = "Line ";
        temp += to_string(Token_List[Token_Index].getLineShow());
        temp += " ";
        temp += "\"";
        temp += Token_List[Token_Index].getLex();
        temp += "\" ";
        temp += "LoopStm Error!\n";
        Print_Error(temp);
    }
    t->Add_Child(exp());
    t->Add_Child(matchToken("DO"));
    t->Add_Child(stmList());
    t->Add_Child(matchToken("ENDWH"));
    return t;
}

TreeNode* inputStm() {
    TreeNode* t = new TreeNode("InputStm");
    t->Add_Child(matchToken("READ"));
    t->Add_Child(matchToken("LPAREN"));
    t->Add_Child(matchToken("ID"));
    t->Add_Child(matchToken("RPAREN"));
    return t;
}
TreeNode* outputStm() {
    TreeNode* t = new TreeNode("OutputStm");
    t->Add_Child(matchToken("WRITE"));
    t->Add_Child(matchToken("LPAREN"));
    t->Add_Child(exp());
    t->Add_Child(matchToken("RPAREN"));
    return t;
}

TreeNode* returnStm() {
    TreeNode* t = new TreeNode("ReturnStm");
    t->Add_Child(matchToken("RETURN1"));
    return t;
}

TreeNode* assCall() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "LMIDPAREN" ||
        Token_List[Token_Index].getLex() == "DOT" ||
        Token_List[Token_Index].getLex() == "ASSIGN") {
        t = new TreeNode("AssCall");
        t->Add_Child(assignmentRest());
        return t;
    } else if (Token_List[Token_Index].getLex() == "LPAREN") {
        t = new TreeNode("AssCall");
        t->Add_Child(callStmRest());
        return t;
    }

    string temp = "Line ";
    temp += to_string(Token_List[Token_Index].getLineShow());
    temp += " ";
    temp += "\"";
    temp += Token_List[Token_Index].getLex();
    temp += "\" ";
    temp += "AssCall Error!\n";
    Print_Error(temp);
    return t;
}
TreeNode* assignmentRest() {
    TreeNode* t = new TreeNode("AssignmentRest");
    if (Token_List[Token_Index].getLex() == "LMIDPAREN" ||
        Token_List[Token_Index].getLex() == "DOT") {
        t->Add_Child(variMore());
    }
    t->Add_Child(matchToken("COLON"));
    t->Add_Child(exp());
    return t;
}

TreeNode* callStmRest() {
    TreeNode* t = new TreeNode("CallStmRest");
    t->Add_Child(matchToken("LPAREN"));
    t->Add_Child(actparamList());
    t->Add_Child(matchToken("RPAREN"));
    return t;
}

TreeNode* variMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "LMIDPAREN") {
        t = new TreeNode("VariMore");
        t->Add_Child(matchToken("LMIDPAREN"));
        t->Add_Child(exp());
        t->Add_Child(matchToken("RMIDPAREN"));
    } else if (Token_List[Token_Index].getLex() == "DOT") {
        t = new TreeNode("VariMore");
        t->Add_Child(matchToken("DOT"));
        t->Add_Child(fieldVar());
    }
    return t;
}

TreeNode* exp() {
    TreeNode* t = new TreeNode("Exp");
    t->Add_Child(term());
    t->Add_Child(otherTerm());
    return t;
}

TreeNode* actparamList() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "LPAREN" ||
        Token_List[Token_Index].getLex() == "INTC" ||
        Token_List[Token_Index].getLex() == "ID") {
        t = new TreeNode("ActParamList");
        t->Add_Child(exp());
        t->Add_Child(actparamMore());
    }
    return t;
}

TreeNode* actparamMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "COMMA") {
        t = new TreeNode("ActParamMore");
        t->Add_Child(matchToken("COMMA"));
        t->Add_Child(actparamList());
    }
    return t;
}

TreeNode* term() {
    TreeNode* t = new TreeNode("Term");
    t->Add_Child(factor());
    t->Add_Child(otherFactor());
    return t;
}

TreeNode* otherTerm() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "PLUS" ||
        Token_List[Token_Index].getLex() == "MINUS") {
        TreeNode* t = new TreeNode("OtherTerm");
        t->Add_Child(addOp());
        t->Add_Child(exp());
    }
    return t;
}

TreeNode* addOp() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "PLUS") {
        TreeNode* t = new TreeNode("AddOp");
        t->Add_Child(matchToken("PLUS"));
        return t;
    } else if (Token_List[Token_Index].getLex() == "MINUS") {
        TreeNode* t = new TreeNode("AddOp");
        t->Add_Child(matchToken("MINUS"));
        return t;
    }

    string temp = "Line ";
    temp += to_string(Token_List[Token_Index].getLineShow());
    temp += " ";
    temp += "\"";
    temp += Token_List[Token_Index].getLex();
    temp += "\" ";
    temp += "CompareOp Error!\n";
    Print_Error(temp);
    return t;
}

TreeNode* factor() {
    TreeNode* t = new TreeNode("Factor");

    if (Token_List[Token_Index].getLex() == "INTC") {
        t->Add_Child(matchToken("INTC"));
        return t;
    } else if (Token_List[Token_Index].getLex() == "LPAREN") {
        t->Add_Child(matchToken("LPAREN"));
        t->Add_Child(exp());
        t->Add_Child(matchToken("RPAREN"));
        return t;
    } else if (Token_List[Token_Index].getLex() == "ID") {
        t->Add_Child(variable());
        return t;
    }
    string temp = "Line ";
    temp += to_string(Token_List[Token_Index].getLineShow());
    temp += " ";
    temp += "\"";
    temp += Token_List[Token_Index].getLex();
    temp += "\" ";
    temp += "Factor Error!\n";
    Print_Error(temp);

    return NULL;
}

TreeNode* otherFactor() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "TIMES" ||
        Token_List[Token_Index].getLex() == "OVER") {
        TreeNode* t = new TreeNode("OtherFactor");
        t->Add_Child(multOp());
        t->Add_Child(term());
    }
    return t;
}

TreeNode* multOp() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "TIMES") {
        TreeNode* t = new TreeNode("MultOp");
        t->Add_Child(matchToken("TIMES"));
        return t;
    } else if (Token_List[Token_Index].getLex() == "OVER") {
        TreeNode* t = new TreeNode("MultOp");
        t->Add_Child(matchToken("OVER"));
        return t;
    }

    string temp = "Line ";
    temp += to_string(Token_List[Token_Index].getLineShow());
    temp += " ";
    temp += "\"";
    temp += Token_List[Token_Index].getLex();
    temp += "\" ";
    temp += "CompareOp Error!\n";
    Print_Error(temp);
    return t;
}

TreeNode* variable() {
    TreeNode* t = new TreeNode("Variable");
    t->Add_Child(matchToken("ID"));
    t->Add_Child(variMore());
    return t;
}

TreeNode* fieldVar() {
    TreeNode* t = new TreeNode("FieldVar");
    t->Add_Child(matchToken("ID"));
    t->Add_Child(fieldVarMore());
    return t;
}

TreeNode* fieldVarMore() {
    TreeNode* t = NULL;
    if (Token_List[Token_Index].getLex() == "LMIDPAREN") {
        TreeNode* t = new TreeNode("FieldVarMore");
        t->Add_Child(matchToken("LMIDPAREN"));
        t->Add_Child(exp());
        t->Add_Child(matchToken("RMIDPAREN"));
    }
    return t;
}

void printTree(TreeNode* t, int layer, bool islastson, char out[100]) {
    int strline = 0;
    if (t == NULL)
        return;// 递归出口

    int last = 31; //这段代码的目的是找到当前结点最后一个儿子的index
    while (last >= 0) {
        if (t->child[last] != NULL)
            break;
        // 找到最后一个儿子节点
        last--;
    }
    // 如果没有儿子 last == -1
    // 否则 last 记录最后一个儿子节点的index


    for (int i = 0; i < layer; i++) //根据层数打印空格
    {
        outstr[strline] += "   ";
        //printf("   ");
    }

    if (layer == 0) //特殊情况 根节点的显示
    {
        outstr[strline] += "   ";
        outstr[strline] += t->name;
        //cout << t->name;
    } else //不是根节点的时候
    {
        if (islastson == true) // 如果当前节点是父节点的最后一个儿子节点
        {
            outstr[strline] += "└ "; // 则它前面的树枝会有些区别
            outstr[strline] += t->name;
            //cout << "└ " << t->name;
        } else// 不是父节点的最后一个儿子节点
        {
            outstr[strline] += "├ ";
            outstr[strline] += t->name;
            //cout << "├ " << t->name;
        }
    }

    outstr[strline] += "\n"; //换行
    strline++;
    //outstr += "\n";
    //cout << endl;

    for (int i = 0; i < 10; i++) //递归
    {
        if (i != last) // 如果不是儿子节点 那么第三个参数就标成false
            printTree(t->child[i], layer + 1, false, out);
        else
            printTree(t->child[i], layer + 1, true, out);
    }

    if (layer == 0) // 在整个递归过程结束退出之前进行打印
    {
        // 首先对字符串进行修改
        // 这里修改的目的是让树枝能连起来
        // 底下这段代码的功能比较难以描述了
        // 你可以注释掉一些部分自己看一看会对打印的树有什么影响
        ///*
        for (int i = 1; i < strline; i++) {
            int j = 0;
            while (outstr[i][j] != '\n') {
                if (outstr[i - 1][j] == '\n')
                    break;
                if (outstr[i - 1][j] == ch2[0] && outstr[i - 1][j + 1] == ch2[1]) // 这个很重要 不加的话会出问题
                {
                    ; // 上一行是"└"
                } else if (outstr[i - 1][j] == ch1[0] && outstr[i - 1][j + 1] == ch1[1] && outstr[i][j] == ' ') {
                    // 上一行是"├" 这一行是空格
                    outstr[i][j] = ch3[0];
                    outstr[i][j + 1] = ch3[1];
                } else if (outstr[i - 1][j] == ch3[0] && outstr[i][j] == ' ') {
                    // 上一行是"│" 这一行是空格
                    outstr[i][j] = ch3[0];
                    outstr[i][j + 1] = ch3[1];
                }
                j++;
            }
        }
        //*/

        // 修改完之后进行打印
        ofstream output(out);
        for (int i = 0; i < strline; i++) {
            output << outstr[i];
        }
        output.close();
        for (int i = 0; i < strline; i++) {
            cout << outstr[i];
        }
    }
}

TreeNode* Recursive_down(char* out) {
    TreeNode* t = program();
    ofstream output(out);
    //char header[3] = { (char)0xEF,(char)0xBB,(char)0xBF };
    //output << header[0] << header[1] << header[2];
    // FILE*fp = fopen("C:\\Users\\Yichuan\\Desktop\\tree.txt", "wt+,ccs=UTF-8");
    printTree(t, 0, false, out);
    return t;
}

TreeNode* recursiveDecline(char* in,char* out) {
    int a, sum = 0;
    string t1, t2;
    ifstream infile;
    infile.open(in, ios::in);
    while (!infile.eof()) {
        infile >> a >> t1 >> t2;
        Token_List[sum++] = Token(a, t1, t2);
    }
    return Recursive_down(out);
}
