%option yylineno
%x comment
%x mlcomment
%x unknown
%option noyywrap
%{
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cmath>
#include <cassert>

#include "LGrammar.tab.g"
//#define DEBUG_LEXER 1

typedef std::map<std::string, yytokentype> token_map;

token_map operator_tokens = {
    {"+",  PLUS   },
    {"-",  MINUS  },
    {"*",  MULT   },
    {"/",  DIVIDE },
    {"%",  PERCENT},
    {"==", EQ     },
    {"!=", NEG    },
    {">",  GT     },
    {">=", GE     },
    {"<",  LT     },
    {"<=", LE     },
    {"&&", AND    },
    {"||", OR     },
    {":=", ASSIGN }
};

token_map keyword_tokens = {
    {"if",    IF},
    {"then",  THEN},
    {"else",  ELSE},
    {"while", WHILE},
    {"do",    DO},
    {"read",  READ},
    {"write", WRITE},
    {"begin", TBEGIN},
    {"end",   END},
    {"skip",  SKIP}
};

static token_map split_tokens = {
    {"(",  LPARENT},
    {")",  RPARENT},
    {";",  COLON}
};

static token_map bool_tokens = {
    {"false", FALSE},
    {"true", TRUE}
};

struct MLCommentToken
{
    MLCommentToken(const std::string &str
                   , int line_start
                   , int start
                   , int end)
        : line_start_(line_start)
        , start_(start)
        , end_(end)
        , str_(str)
    {}

    MLCommentToken & operator+=(const std::string &s)
    {
        str_ += s;
        end_ += static_cast<int>(s.size());
        return *this;
    }

    int line_start_, start_, end_;
    int level_ = 0;
    std::string str_;
};

static MLCommentToken mltoken{"", -1, -1, -1};

static int update_num_chars()
{   
    static int num_chars = 0;
    static int cur_line = -1;
    const bool end_of_line = yytext[yyleng - 1] == '\r' ||
                             yytext[yyleng - 1] == '\n';

    if (!end_of_line && yylineno != cur_line)
    {
        cur_line = yylineno;
        num_chars = 0;
    }
    num_chars += yyleng;
    return num_chars;
}

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.last_column = update_num_chars(); \
    yylloc.first_column = yylloc.last_column - yyleng + 1; \
//     yycolumn += yyleng; \
//     yylval.str = strdup(yytext);

static bool filter_comments = true;

static void print_token(const std::string &name,
                        const YYLTYPE &yylloc,
                        const std::string &str = "")
{
    (void)yylloc, (void)name, void(str);
#if DEBUG_LEXER
    static std::ofstream fout("lexer.out");
    int num_chars = update_num_chars();
    int finish = num_chars - 1;
    int start = finish - yyleng + 1;

    fout << name << "(" + str << ", " << start << ", " << finish << ")" << std::endl;
#endif
}

%}

CR      \x0D
LF      \x0A
CRLF    {CR}{LF}
LINESPLIT   {CR}|{LF}|{CRLF}
SP      \x20 
HT      \x09
FF      \x0C
SPACE   {LINESPLIT}|{SP}|{HT}|{FF}

COMMENTS    "//"
SIGN 	    ("+"|"-")
NUMBER	    (0|[1-9][0-9]*)
EXPONENT    (e{SIGN}?{NUMBER})
RATIONAL    "-"?{NUMBER}("."[0-9]*)?{EXPONENT}?|"."[0-9]+{EXPONENT}?
BOOLEAN     false|true
KEYWORDS    if|then|else|while|do|read|write|begin|end|skip
IDENT_TOKEN [[:alpha:]_][[:alnum:]_]*
SPECIALS    "+"|"-"|"*"|"/"|"%"|"=="|"!="|">"|">="|"<"|"<="|"&&"|"||"|"**"|":="
SPLIT       "("|")"|";"
NOT_SPLIT   [^[:space:]();\n\r]
ENDL        [\n\r]
NOT_ENDL    [^\n\r]
ML_NOT_ENDL [^\n\r\)(]
TOKEN_END   {SPLIT}|{SPACE}|{COMMENTS}
%%

{COMMENTS}{NOT_ENDL}*   {
             if (!filter_comments)
             {
                 yylval.str = strdup(yytext);
                 return COMMENT;
             }
        }

<INITIAL,mlcomment>"(*" {
            const int num_chars = update_num_chars();
            if (mltoken.level_ == 0)
            {
                int start_pos = num_chars - yyleng;
                mltoken = MLCommentToken("", yylineno, start_pos, start_pos - 1);
                BEGIN(mlcomment);
            }
            mltoken += yytext;
            mltoken.level_++;
        }
<mlcomment>"("/[^"*"] { mltoken += yytext; }
<mlcomment>({ML_NOT_ENDL}*|"(")"*"+")" {
            mltoken += yytext;
            mltoken.level_--;
            if (mltoken.level_ == 0)
            {
                BEGIN(INITIAL);
                yylval.str = strdup(mltoken.str_.c_str());
                yylloc.first_column = mltoken.start_;
                yylloc.first_line = mltoken.line_start_;
                yylloc.last_column = mltoken.end_;
                if (!filter_comments)
                {
                    return MLCOMMENT;
                }
            }
        }
<mlcomment>{ML_NOT_ENDL}*({ENDL}|(")"*)) { mltoken += yytext; }

{RATIONAL}  { print_token("Num", yylloc, yytext); yylval.dval = atof(yytext); return NUM; }

{KEYWORDS}/[^[:alpha:]]  { print_token(std::string("KW_") + yytext, yylloc, yytext); yylval.str = strdup(yytext); return keyword_tokens.at(yytext); }

{BOOLEAN}/[^[:alpha:]] { print_token("Bool", yylloc, yytext); yylval.bval = strcmp(yytext, "true") == 0? true : false; return bool_tokens.at(yytext); }

{IDENT_TOKEN}  { print_token("Ident", yylloc, yytext); yylval.str = strdup(yytext); return IDENT; }

{SPECIALS}     { print_token("Op", yylloc, yytext); yylval.op = yytext[0]; return operator_tokens.at(yytext); }

{SPLIT}        { print_token("SPLIT", yylloc, yytext); return split_tokens.at(yytext); }

<INITIAL>{SPACE}  { }
<INITIAL>.        { BEGIN(unknown); yyless(0); }

<unknown>{NOT_SPLIT}+ { print_token("Unknown", yylloc, yytext); BEGIN(INITIAL); yylval.str = strdup(yytext); return UNKNOWN; }

%%

#if DEBUG_LEXER
 int test(void)
 {
     int token;
     while ((token = yylex()) != 0)
     {
         printf("Token: %d (%s)\n", token, yytext);
     }
     return 0;
 }
#endif

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "USAGE: " << argv[0] << " L-language source file." << std::endl;
        return -1;
    }

    yyin = fopen(argv[1], "r");


//    for (int i = 1; i < argc; ++i)
//    {
//        filter_comments |= strcmp(argv[i], "-filter") == 0;
//    }

#if DEBUG_LEXER
    return test();
#else
    std::string dot_filename = std::string(argv[1]) + ".dot";
    std::ofstream fout(dot_filename);
    if (yyparse() == 0) {
        fout <<
        "digraph \"parser\"\n"
        "{                                                              \n"
        "  node [fontname = courier, shape = box, colorscheme = paired6]\n"
        "  edge [fontname = courier]                                    \n"
        ;
        assert(main_node);
        main_node->print(fout);
        fout << "}" << std::endl;
    }
    delete main_node;
    return 0;
#endif
}

void yyerror(const char *s) {
    std::cout << "Parser error: " << s << std::endl;
    exit(-1);
}

