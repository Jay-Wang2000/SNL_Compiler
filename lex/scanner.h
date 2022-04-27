#ifndef scan
#define scan 1
#include<string>
#include "bean/Token.h"
#include "bean/TokenList.h"
using namespace std;
inline bool contain(string arr, char c);
inline int findIndex(char var, string op);
vector<Token>* getList();
#endif