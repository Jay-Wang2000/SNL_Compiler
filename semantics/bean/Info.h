#ifndef info 
#define info 1

#include<string>
#include<list>
using namespace std;
typedef enum { intTy, charTy, arrayTy, recordTy, boolTy }TypeKind;
typedef enum { dir, indir }AccessKind;
struct kind
{
    TypeKind type;
    string name;
}type_name[5]{
   {intTy,"intTy"},
   {charTy,"charTy"},
   {arrayTy,"arrayTy"},
   {recordTy,"recordTy"},
   {boolTy,"boolTy"}
};

struct Info      //类型信息
{
    int size;
    TypeKind kind;
    class address {
    private:
        string name;
        TypeKind unitKind;
    };
    union
    {
        struct 
        {
            Info* indexTy;
            Info* elemTy;
        }ArrayAttr;

        list<address> body;
    }more;

};
#endif