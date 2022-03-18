#include "Token.h"
#include <vector>
class TokenList
{
private:
    vector<Token> list;

public:
    TokenList();
    ~TokenList();
    void add(Token token){
        list.push_back(token);
    }
    vector<Token> getList(){
        return list;
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
