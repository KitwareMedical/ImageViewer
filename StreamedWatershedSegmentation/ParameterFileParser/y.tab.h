typedef union
{
  long int integer;
  float decimal;
  std::string *string;
  Parameter*  Parameter_ptr;
  File*       File_ptr;
  GenericValue * GenericValue_ptr;
  std::vector<GenericValue *> * ValueArray_ptr;
} YYSTYPE;
#define NUM     257
#define DEC_NUM 258
#define IDENT   259
#define STRING  260
#define EQUALS  261
#define LPAREN  262
#define RPAREN  263
#define NEWLINE 264


extern YYSTYPE yylval;
