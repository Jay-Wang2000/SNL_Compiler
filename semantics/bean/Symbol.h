#ifndef symbol
#define symbol 1

#include<string>
#include "Info.h"
#include<list>
using namespace std;

struct Symbol {
    string name;
    Info* information;
    union {
        struct {
            AccessKind access;
            int level;
            int off;
        }varAttr;           //变量标识符额外属性
        struct {
            int level;
            int code;
            int size;
            list<int> params; //形参信息表，指示形参在表中的位置
        }procAttr;          //过程名标识符额外属性

    }more;
};
#endif