typedef enum {
    ENDFILE ,  ERROR ,
            PROGRAM ,  PROCEDURE ,  TYPE ,  VAR ,  IF ,  THEN ,  ELSE ,  FI ,  WHILE ,
            DO ,  ENDWH ,  BEGIN ,  END ,  READ ,  WRITE ,  ARRAY ,  OF ,  RECORD ,
            RETURN , //保留字  
            INTEGER ,  CHAR , //标识符类型
            ID ,  INTC ,  CHARC , //单词类型  
            ASSIGN ,  EQ ,  LT ,  PLUS ,  MINUS ,  TIMES ,  OVER ,  LPAREN ,  RPAREN ,//操作符
            DOT ,  COLON ,  SEMI ,  COMMA ,  LMIDPAREN ,  RMIDPAREN ,  UNDERANGE 
} LexType;
class Token
{
private:
    int lineshow; //记录该单词在原程序中的行数
    LexType lex;
    char *sem;

public:
    Token(int lineshow,LexType lex,char sem[]);
    ~Token();
    int getLineShow()
    {
        return this->lineshow;
    };
    LexType getLex()
    {
        return this->lex;
    };
    char* getSem()
    {
        return this->sem;
    };
};

Token::Token(int lineshow, LexType lex, char sem[])
{
    this->lineshow=lineshow;
    this->lex = lex;
    this->sem = sem;
}

Token::~Token()
{
}
