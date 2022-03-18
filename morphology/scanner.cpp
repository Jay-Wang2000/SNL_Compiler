#include<fstream>
#include<vector>
#include "morphology/bean/Token.h"
#include "morphology/bean/TokenList.h"
using namespace std;
typedef enum {
    START,INID,INNUM,DONE,INASSIGN,INCOMMENT,INRANGE,INCHAR
}State;
string L;
string N;
string D={'+','-','*','/','(',')',')',';','[',']','=','<',"EOF",''};
void ungentNextChar(){

}
void reservedLookUp(){
}


bool contain(string arr,char c){
    for(int var:arr)
        if(var==c)
            return true;

    return false;
}

vector<Token>* getTokenList(){
   for(int i=0;i<26;i++)
       L[i]='a'+i;
   for(int i=0;i<10;i++)
       N[i]='0'+i;
   TokenList *tl=new TokenList();
   ifstream infile;
   infile.open("D:\\qtp\\SNL_Compiler\\example\\common\\c1.txt");
   while(!infile.eof()){    //get next word
       string tmp;
       infile>>tmp;
       State s=START;
       for(char c : tmp){
           switch (s) {
           case START:
               if(contain(L,c))
                   s=INID;
               else if(contain(N,c))
                   s=INNUM;
               else if(contain(D,c))
                   s=DONE;
               else if(c==':'&& *(&c+8)=='=')
                   s=INASSIGN;
               else if(c=='{')
                   s=INCOMMENT;
               else if(c=='.'&& *(&c+8)=='.')
                   s=INRANGE;
               else if(c=',')
                   s=INCHAR;
               break;

           case INID:
               if(contain(L,C)||contain(N,C))
               s=INID;
               break;

           case INNUM:
               if('0'<=t&&t<='0')
               s=INNUM;
               break;

           case DONE:
               s=INID;
               break;

           case START:
               s=INID;
               break;

           case START:
               s=INID;
               break;
           default:
               break;
           }
       }
   }
   ofstream outfile;
   outfile<<tl.getList();
   infile.close();
   outfile.close();
   return tl.getList();
}
