#ifndef treenode
#define treenode 1
#include<string>
using namespace std;

typedef enum {
    ENDFILE, ERROR,
    PROGRAM, PROCEDURE, TYPE, VAR, IF,
    THEN, ELSE, FI, WHILE, DO, ENDWH,
    BEGIN, END, READ, WRITE, ARRAY, OF,
    RECORD, RETURN,
    INTEGER, CHAR, //INTEGER 是integer
    ID, INTC, CHARC,
    ASSIGN, EQ, LT, PLUS, MINUS,
    TIMES, OVER, LPAREN, RPAREN, DOT,
    COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
    UNDERANGE
}LexType;// 终极符



typedef enum {
    Program, ProgramHead, ProgramName, DeclarePart,
    TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
    TypeId, TypeName, BaseType, StructureType,
    ArrayType, Low, Top, RecType,
    FieldDecList, FieldDecMore, IdList, IdMore,
    VarDec, VarDeclaration, VarDecList, VarDecMore,
    VarIdList, VarIdMore, ProcDec, ProcDeclaration,
    ProcDecMore, ProcName, ParamList, ParamDecList,
    ParamMore, Param, FormList, FidMore,
    ProcDecPart, ProcBody, ProgramBody, StmList,
    StmMore, Stm, AssCall, AssignmentRest,
    ConditionalStm, StmL, LoopStm, InputStm,
    InVar, OutputStm, ReturnStm, CallStmRest,
    ActParamList, ActParamMore, RelExp, OtherRelE,
    Exp, OtherTerm, Term, OtherFactor,
    Factor, Variable, VariMore, FieldVar,
    FieldVarMore, CmpOp, AddOp, MultOp
}NonTerminal; // 非终极符

// 保留字
struct Word {
    string str;
    LexType tok;
}reservedWords[42] =
{
    {"ENDFILE",ENDFILE},
    {"ERROR",ERROR},
    {"PROGRAM",PROGRAM},
    {"PROCEDURE",PROCEDURE},
    {"TYPE",TYPE},
    {"VAR",VAR},
    {"IF",IF},
    {"THEN",THEN},
    {"ELSE",ELSE},
    {"FI",FI},
    {"WHILE",WHILE},
    {"DO",DO},
    {"ENDWH",ENDWH},
    {"BEGIN",BEGIN},
    {"END",END},
    {"READ",READ},
    {"WRITE",WRITE},
    {"ARRAY",ARRAY},
    {"OF",OF},
    {"RECORD",RECORD},
    {"RETURN",RETURN},
    {"INTEGER",INTEGER},
    {"CHAR",CHAR}, //INTEGER 是integer
    {"ID",ID},
    {"INTC",INTC},
    {"CHARC",CHARC},
    {"ASSIGN",ASSIGN},
    {"EQ",EQ},
    {"LT",LT},
    {"PLUS",PLUS},
    {"MINUS",MINUS},
    {"TIMES",TIMES},
    {"OVER",OVER},
    {"LPAREN",LPAREN},
    {"RPAREN",RPAREN},
    {"DOT",DOT},
    {"COLON",COLON},
    {"SEMI",SEMI},
    {"COMMA",COMMA},
    {"LMIDPAREN",LMIDPAREN},
    {"RMIDPAREN",RMIDPAREN},
    {"UNDERANGE",UNDERANGE}
};
struct token {
    int line;
    struct Word wd;
    int index = -1; // tokenlist中的index
};// token
class TreeNode {
public:
    string name;
    token* tk;
    TreeNode* child[32];
    int childIndex;
    TreeNode(string nm) {
        name = nm;
        tk = NULL;
        for (int i = 0; i < 31; i++) {
            child[i] = NULL;
        }
        childIndex = 0;
    }
    void Add_Child(TreeNode* t) {
        child[childIndex] = t;
        childIndex++;
    }
};

#endif