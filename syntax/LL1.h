#ifndef ll1
#define ll1 1
#include "./bean/TreeNode.h"
using namespace std;


char OUTerrorLL1[100] = "D:\\qtp\\SNL_Compiler\\out\\errorLL1.txt"; // 报错

typedef LexType Terminal; //终极符的别名

token tokenList[1024];// token表
/****************************************************************/
// 语法分析

// LL1分析栈的一个节点
struct StackNode {
    int ntflag;//判断是终极符还是非终极符

    NonTerminal n;//0
    Terminal t;//1

    TreeNode* p;

    struct StackNode* next;

};
// LL1分析栈
class AnalysisStack {
public:
    AnalysisStack() {
        isEmpty = 1;
        top = NULL;
    }
    StackNode* top; // 注意 pop() 无返回值 查看栈顶元素请用top
    int isEmpty;
    void push(int ntflag, int num) {
        StackNode* p;
        p = new StackNode();
        p->next = NULL;
        p->ntflag = -1; //正常应为 0 1
        p->n = (NonTerminal) -1;//按错误的值进行初始化
        p->t = (Terminal) -1;//按错误的值进行初始化


        if (ntflag == 0)
            p->n = (NonTerminal) num;
        else if (ntflag == 1)
            p->t = (Terminal) num;

        p->ntflag = ntflag;
        p->next = top;
        top = p;
        isEmpty = 0;
    }
    void pop() {
        top = top->next;
        if (top == NULL)
            isEmpty = 1;
    }
};

AnalysisStack* anlsstack = new AnalysisStack();

// 全局变量 LL1树根结点 设为全局变量是为了方便在语义分析中继续使用
TreeNode* treeroot = NULL;

/****************************************************************/
/****************************************************************/

int sum = 0;
int INDEX = 0; // 搭配tokenList使用
token* currenttoken; // 建立语法树时使用

string outstrLL1[512]; //
int strlineLL1 = 0;

int LL1Table[104][104]; // LL1表 这里采用了书上的做法 用一个单独的函数进行初始化
TreeNode* currentTree; // LL1用

void processAddChild(int istmnl, int lex, string name, int childindex);

void process(int num);

void InitLL1Table();

TreeNode* programLL1();

void readtoken();

void printErrorMsg(string reason);

void printTreeLL1(TreeNode* t, int layer, bool islastson, char out[100]);

void processAddChild(int istmnl, int lex, string name, int childindex);\

void process(int num);

void InitLL1Table();

TreeNode* programLL1();

void LL1();
#endif
