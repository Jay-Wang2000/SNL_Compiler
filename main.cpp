#include "./lex/scanner.cpp"
#include "./syntax/LL1.cpp"
#include "./syntax/recursiveDe.cpp"
#include "./semantics/sem.cpp"
#include <string.h>
using namespace std;

int main(int argc, char* argv[]) {
    char out[100] = "D:\\qtp\\SNL_Compiler\\out\\recursiveDe.txt";
    char in[100] = "D:\\qtp\\SNL_Compiler\\example\\common\\c1.txt";
    TreeNode* t;
    while (1) {
        if (strcmp(argv[1], "-lex") == 0) {
            getList(argv[2], argv[3]);
            return 0;
        } else if (strcmp(argv[1], "-LL1") == 0) {
            t = LL1(argv[2], argv[3]);
            printTreeLL1(t, 0, false, argv[3]);
            semanticAnalysis(t, argv[3]);
            return 0;
        } else if (strcmp(argv[1], "-RD") == 0) {
            t = LL1(argv[2], argv[3]);
            recursiveDecline(argv[2], argv[3]);
            semanticAnalysis(t, argv[3]);
            return 0;
        } else if (strcmp(argv[1], "-h") == 0) {
            printf("---------------------------------------------\n"
                "+ -lex infile    outfile 语 法 分 析         +\n"
                "+ -LL1 Tokenfile outfile 使用LL1 文法分析    +\n"
                "+ -RD  Tokenfile outfile 使用递归下降文法分析+\n"
                "+                语义分析自动执行            +\n"
                "---------------------------------------------\n");
            return 0;
        }  else {
            printf("Method error!");
            return 0;
        }
    }
}
