%x lineno
%x comment
%{
#include <cctype>
#include <cmath>
#include <string>
#include <map>

static int num_pages = 0, num_lines = 0, num_chars = 0;
static void debug_counters()
{
    int start = num_chars - yyleng;
    int finish = start + yyleng - 1;
    printf("%d, %d, %d", num_lines, start, finish);
    printf("); ");
}

static void update_num_chars()
{
    // printf("(%s, %d) ###\n", yytext, yyleng);
    num_chars += yyleng;
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
KEYWORDS if|then|else|while|do|read|write|begin|end
IDENT   [[:alpha:]_][[:alnum:]_]*
SPECIALS    "+"|"−"|"∗"|"/"|"%"|"=="|"!="|">"|">="|"<"|"<="|"&&"|"||"
SPLIT   "("|")"|";"
%%

<comment>{LINESPLIT} num_chars += yyleng; ++num_lines; BEGIN(INITIAL);
<comment>.           ++num_chars;
"//"            num_chars += 2; BEGIN(comment);

<*>{FF}         update_num_chars(); ++num_pages;
<*>{LINESPLIT}  update_num_chars(); ++num_lines;   // check if CR and LF does not fire CR and LF separately.
<*>{SPACE}      update_num_chars();                // check if it doesn't spoil FF.

{KEYWORDS}      {
    update_num_chars(); 
    char *s = strdup(yytext); 
    s[0] = (char)toupper(s[0]);
    printf("KW_%s(", s);
    free(s);
    debug_counters();
    }

{IDENT}         {
    update_num_chars(); 
    printf("Ident(\"%s\", ", yytext);
    debug_counters();
    }

{SPECIALS}      {
    update_num_chars();
    printf("Op(%s, ", special_names[yytext].c_str());
    debug_counters();
    }

{SPLIT}         {
    update_num_chars();
    printf("%s(", split_names[yytext].c_str());
    debug_counters();
    }

{RATIONAL}      {
    // printf("%s ", yytext);
    update_num_chars();
    printf("Num(%s, ", yytext);
    debug_counters();
    }

.               update_num_chars(); ECHO;

%%
int main()
   {
       yylex();
       printf("\n### END LEXER ###\n");
       printf("PAGES: %d\n", num_pages);
       printf("LINES: %d\n", num_lines);
       printf("CHARS: %d\n", num_chars);
       return 0;
   }
