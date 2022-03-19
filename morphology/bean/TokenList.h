#include "D:\qtp\SNL_Compiler\morphology\bean\Token.h"
#include <vector>
using namespace std;
class TokenList
{
private:
    vector<Token> list;            

public:
    TokenList();
    ~TokenList();
    void add(Token token){
        this->list.push_back(token);
    }
    vector<Token>* getList(){
        return &this->list;
    }
    void print(){
        
    }
};

TokenList::TokenList()
{
}

TokenList::~TokenList()
{
}
