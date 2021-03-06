%option yylineno
%x comment
%x mlcomment
%x unknown
%{
#include <cctype>
#include <cmath>
#include <string>
#include <map>
#include <iostream>

#include "tokenizer.h"


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
    // printf("\nDEBUG: %s: %d\n", yytext, num_chars);
    return num_chars;
}

static MLCommentToken mltoken{"", -1, -1, -1};
static int mltoken_level = 0;
static Tokenizer tokenizer;
static bool filter_comments = false;

static void print_token(const std::string &name,
                        const std::string &str = "",
                        const std::string &prefix="")
{
    int num_chars = update_num_chars();
    int finish = num_chars - 1;
    int start = finish - yyleng + 1;
    tokenizer.emplace(name, str, prefix, yylineno-1, start, finish);
}

static std::map<std::string, std::string> split_names = {
    {"(",  "LParent"},
    {")",  "RParent"},
    {";",  "Colon"}
};

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
KEYWORDS    if|then|else|while|do|read|write|begin|end
IDENT       [[:alpha:]_][[:alnum:]_]*
SPECIALS    "+"|"-"|"*"|"/"|"%"|"=="|"!="|">"|">="|"<"|"<="|"&&"|"||"|"**"|":="
SPLIT       "("|")"|";"
NOT_SPLIT   [^[:space:]();\n\r]
ENDL        [\n\r]
NOT_ENDL    [^\n\r]
ML_NOT_ENDL [^\n\r\)(]
TOKEN_END   {SPLIT}|{SPACE}|{COMMENTS}
%%

{COMMENTS}{NOT_ENDL}*     {
        char *s = strdup( yytext );
        if (!filter_comments)
        {
            print_token("Comment", s + 2);
        }
        free(s);
        }


<INITIAL,mlcomment>"(*"[^"*"(]* {
            const int num_chars = update_num_chars();
            if (mltoken_level == 0)
            {
                int start_pos = num_chars - yyleng;
                mltoken = MLCommentToken("", yylineno - 1, start_pos, start_pos-1);
                BEGIN(mlcomment);
            }
            mltoken += yytext;
            mltoken_level++;
        }

<mlcomment>"("[^"*"]* {
            update_num_chars();
            mltoken += yytext; }

<mlcomment>{ML_NOT_ENDL}*"*"+")" {
            update_num_chars();
            mltoken += yytext;
            mltoken_level--;
            if (mltoken_level == 0)
            {
                if (!filter_comments)
                {
                    tokenizer.emplace(new MLCommentToken(mltoken));
                }
                BEGIN(INITIAL);
            }
        }

<mlcomment>{ML_NOT_ENDL}*({ENDL}|")") {
            update_num_chars();
            mltoken += yytext;
        }


{RATIONAL}  print_token("Num", yytext);

{KEYWORDS}/[^[:alpha:]]  {
    char *s = strdup(yytext); 
    s[0] = (char)toupper(s[0]);
    print_token(s, "", "KW_");
    free(s);
    }

{BOOLEAN}/[^[:alpha:]]              print_token("Bool", yytext);

{IDENT}                             print_token("Ident", yytext);

{SPECIALS}                          print_token("Op", yytext);

{SPLIT}                             print_token(split_names[yytext]);

<INITIAL>{SPACE}                    update_num_chars();
<INITIAL>.                          BEGIN(unknown); yyless(0);

<unknown>{NOT_SPLIT}+               print_token("Unknown", yytext); BEGIN(INITIAL);

%%
int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        filter_comments |= strcmp(argv[i], "-filter") == 0;
    }

    yylex();
    tokenizer.print();
    return 0;
}
