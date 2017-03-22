%option yylineno
%x comment
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

static Tokenizer tokenizer;

static void print_token(const std::string &name,
                        const std::string &str = "",
                        bool use_quotes = false,
                        const std::string &prefix="")
{
    // std::cout << prefix << name << "(";
    int num_chars = update_num_chars();
    int finish = num_chars - 1;
    int start = finish - yyleng + 1;
    // std::cout << yylineno - 1 << ", " << start << ", " << finish << "); ";
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
SPECIALS    "+"|"-"|"*"|"/"|"%"|"=="|"!="|">"|">="|"<"|"<="|"&&"|"||"
SPLIT       "("|")"|";"
NOT_SPLIT   [^[:space:]();\n\r]
TOKEN_END   {SPLIT}|{SPACE}|{COMMENTS}
%%

{COMMENTS}[^\n\r]*     {
    char *s = strdup( yytext ); print_token("Comment", s + 2, true); free(s);
    }

{KEYWORDS}/{TOKEN_END}      {
    char *s = strdup(yytext); 
    s[0] = (char)toupper(s[0]);
    print_token(s, "", false, "KW_");
    free(s);
    }

{BOOLEAN}/{TOKEN_END}    print_token("Bool", yytext, false);

{IDENT}/{TOKEN_END}                 print_token("Ident", yytext, true);

({SPECIALS})+              print_token("Op", yytext, false);

{RATIONAL}  print_token("Num", yytext, false);

{SPLIT}                             print_token(split_names[yytext]);

<INITIAL>{SPACE}                    update_num_chars();
<INITIAL>.                          BEGIN(unknown); yyless(0);

<unknown>{NOT_SPLIT}+               print_token("Unknown", yytext, true); BEGIN(INITIAL);

%%
int main()
   {
       yylex();
       tokenizer.print();
       return 0;
   }
