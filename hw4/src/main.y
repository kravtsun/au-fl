%option yylineno
%x comment
%x unknown
%{
#include <cctype>
#include <cmath>
#include <string>
#include <map>
#include <iostream>


static int update_num_chars()
{   
    static int num_chars = 0;
    static int cur_line = -1;
    if (yylineno != cur_line)
    {
        cur_line = yylineno;
        num_chars = 0;
    }
    num_chars += yyleng;
    return num_chars;
}

static void print_token(const std::string &name,
                        const std::string &str = "",
                        bool use_quotes = false,
                        const std::string &prefix="")
{
    std::cout << prefix << name << "(";

    if (!str.empty())
    {
        if (use_quotes)
            std::cout << '"' << str << "\", ";
        else
            std::cout << str << ", ";
    }

    int num_chars = update_num_chars();
    int start = num_chars - yyleng;
    int finish = start + yyleng - 1;
    std::cout << yylineno - 1 << ", " << start << ", " << finish << "); ";
}

static std::map<std::string, std::string> special_names = {
    {"+",  "Plus"},
    {"−",  "Minus"},
    {"∗",  "Mult"},
    {"/",  "Divide"},
    {"%",  "Percent"},
    {"==", "Eq"},
    {"!=", "Neg"},
    {">",  "Gt"},
    {">=", "Ge"},
    {"<",  "Lt"},
    {"<=", "Le"},
    {"&&", "And"},
    {"||", "Or"},
};

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

SIGN 	    ("+"|"-")
NUMBER	    (0|[1-9][0-9]*)
EXPONENT    (e{SIGN}?{NUMBER})
RATIONAL    "-"?{NUMBER}("."[0-9]*)?{EXPONENT}?|"."[0-9]+{EXPONENT}?
BOOLEAN     false|true
KEYWORDS    if|then|else|while|do|read|write|begin|end
IDENT       [[:alpha:]_][[:alnum:]_]*
SPECIALS    "+"|"−"|"∗"|"/"|"%"|"=="|"!="|">"|">="|"<"|"<="|"&&"|"||"
SPLIT       "("|")"|";"
NOT_SPLIT   [^[:space:]();\n\r]
TOKEN_END   {SPLIT}|{SPACE}
%%

<comment>{LINESPLIT} update_num_chars(); BEGIN(INITIAL);
<comment>.           update_num_chars();
"//"                 update_num_chars(); BEGIN(comment);

{KEYWORDS}/{TOKEN_END}      {
    char *s = strdup(yytext); 
    s[0] = (char)toupper(s[0]);
    print_token(s, "", false, "KW_");
    free(s);
    }
{BOOLEAN}/{TOKEN_END}   print_token("Bool", yytext, false);

{IDENT}/{TOKEN_END}     print_token("Ident", yytext, true);

{SPECIALS}/{TOKEN_END}  print_token("Op", special_names[yytext], false);

{RATIONAL}/{TOKEN_END}  print_token("Num", yytext, false);

{SPLIT}                 print_token(split_names[yytext]);

<INITIAL>{SPACE}    update_num_chars();
<INITIAL>.          BEGIN(unknown); yyless(0);

<unknown>{NOT_SPLIT}+   print_token("Unknown", yytext, true); BEGIN(INITIAL);

%%
int main()
   {
       yylex();
       return 0;
   }
