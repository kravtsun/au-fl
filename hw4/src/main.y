%x lineno
%x comment
%{
#include <cctype>
#include <string>
#include <map>

static int num_pages = 0, num_lines = 0, num_chars = 0;
static void debug_counters()
{
    printf("%d, %d, %d", num_pages, num_lines, num_chars-1);
    printf("); ");
}

static void update_num_chars()
{
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

%}

CR      \x0D
LF      \x0A
CRLF    {CR}{LF}
LINESPLIT   {CR}|{LF}|{CRLF}
SP      \x20 
HT      \x09
FF      \x0C
SPACE   {LINESPLIT}|{SP}|{HT}|{FF}

KEYWORDS if|then|else|while|do|read|write|begin|end
IDENT   ([:alpha:]|_)([:alnum:]|_)*
SPECIALS    "+"|"−"|"∗"|"/"|"%"|"=="|"!="|">"|">="|"<"|"<="|"&&"|"||"
%%

<comment>{LINESPLIT} num_chars += yyleng; ++num_lines; BEGIN(INITIAL);
<comment>.           ++num_chars;

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

"//"            num_chars += 2; BEGIN(comment);
.               update_num_chars();

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
