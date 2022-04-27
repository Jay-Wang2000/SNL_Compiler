#ifndef recursivede
#define recursivede 1
#include<string>
#include "./bean/TreeNode.h"
using namespace std;

TreeNode* program();//
TreeNode* programHead();//
TreeNode* declarePart();
TreeNode* programBody();
TreeNode* typeDecPart();
TreeNode* varDecPart();
TreeNode* procDecpart();
TreeNode* typeDec();
TreeNode* typeDecList();
// TreeNode* typeID();
TreeNode* typeDef();
TreeNode* typeDecMore();
TreeNode* baseType();
TreeNode* structureType();
TreeNode* arrayType();
TreeNode* recType();
TreeNode* fieldDecList();
TreeNode* IDList();
TreeNode* fieldDecMore();
TreeNode* IDMore();
TreeNode* varDec();
TreeNode* varDecList();
TreeNode* varIDList();
TreeNode* varDecMore();
TreeNode* varIDMore();
TreeNode* procDec();
TreeNode* paramList();
TreeNode* procDecPart();
TreeNode* procBody();
TreeNode* paramDecList();
TreeNode* param();
TreeNode* paramMore();
TreeNode* formList();
TreeNode* fidMore();
TreeNode* stmList();
TreeNode* stm();
TreeNode* stmMore();
TreeNode* conditionalStm();
TreeNode* loopStm();
TreeNode* inputStm();
TreeNode* outputStm();
TreeNode* returnStm();
TreeNode* assCall();
TreeNode* assignmentRest();
TreeNode* callStmRest();
TreeNode* variMore();
TreeNode* exp();
TreeNode* actparamList();
TreeNode* actparamMore();
TreeNode* term();
TreeNode* otherTerm();
TreeNode* addOp();
TreeNode* factor();
TreeNode* otherFactor();
TreeNode* multOp();
TreeNode* variable();
TreeNode* fieldVar();
TreeNode* fieldVarMore();
void recursiveDecline();
#endif
