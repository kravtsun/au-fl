%locations
%define parse.error verbose
%define parse.lac full

%code requires {
    extern int yylex();
    // #define YY_DECL extern "C" int yylex()
    extern int yyparse();
    extern FILE *yyin;
    extern void yyerror(const char *s);

    #define YYLTYPE YYLTYPE

    #include "ast.h"
    using namespace ast;

    TripleNode *main_node = nullptr;
    static const std::string ident_prefix = "@";
}

%union {
    bool bval;
    double dval;
    char *str;
    char op;
    struct TripleNode *node;
}

%left  <op> OR
%left  <op> AND
%left  <op> EQ NEG
%right <op> ASSIGN
%left  <op> GT GE LT LE
%left  <op> PLUS MINUS
%left  <op> MULT DIVIDE PERCENT
%token LPARENT RPARENT COLON
%left  <str> WRITE READ SKIP WHILE DO IF THEN ELSE TBEGIN END
%left  <str> FALSE TRUE
%token <str> IDENT
%token <dval> NUM
%token <str> MLCOMMENT COMMENT UNKNOWN

%type <node> main multistmt writeexpr readexpr stmt ifstmt expr

%%

main: multistmt { $$ = $1; main_node = $$; }
;

expr
: IDENT { $$ = new LeafNode(ident_prefix + $1, @1); }
| LPARENT expr RPARENT { $$ = $2; }
| NUM   { $$ = new LeafNode(std::to_string($1), @1); }
| FALSE { $$ = new LeafNode("false", @1); }
| TRUE  { $$ = new LeafNode("true", @1); }
| expr PLUS    expr { $$ = new Node($1, $3, "PLUS"); }
| expr MINUS   expr { $$ = new Node($1, $3, "MINUS"); }
| expr MULT    expr { $$ = new Node($1, $3, "MULT"); }
| expr DIVIDE  expr { $$ = new Node($1, $3, "DIVIDE "); }
| expr PERCENT expr { $$ = new Node($1, $3, "PERCENT"); }
| expr EQ      expr { $$ = new Node($1, $3, "EQ"); }
| expr NEG     expr { $$ = new Node($1, $3, "NEG"); }
| expr GT      expr { $$ = new Node($1, $3, "GT"); }
| expr GE      expr { $$ = new Node($1, $3, "GE"); }
| expr LT      expr { $$ = new Node($1, $3, "LT"); }
| expr LE      expr { $$ = new Node($1, $3, "LE"); }
| expr AND     expr { $$ = new Node($1, $3, "AND"); }
| expr OR      expr { $$ = new Node($1, $3, "OR"); }
;

writeexpr : WRITE LPARENT expr RPARENT { 
    TripleNode *left_node = static_cast<TripleNode *>(new LeafNode("write", @1)); 
    $$ = new Node(left_node, $3, "writeexpr"); }
    ;

readexpr : READ LPARENT expr RPARENT {
    TripleNode *left_node = static_cast<TripleNode *>(new LeafNode("read", @1)); 
    $$ = new Node(left_node, $3, "readexpr"); }
    ;

multistmt
: stmt { $$ = $1; }
| stmt COLON multistmt { $$ = new Node($1, $3, "seq"); }
;

stmt
: SKIP { LOG("skip"); $$ = new LeafNode("skip", @1); }
| IDENT ASSIGN expr  { 
    TripleNode *ident_node = new LeafNode(ident_prefix + $1, @1);
    $$ = new Node(ident_node, $3, "assign"); }
| writeexpr { $$ = $1; }
| readexpr  { $$ = $1; }
| WHILE LPARENT expr RPARENT DO ifstmt { $$ = new Node($3, $6, "while-do"); }
| IF LPARENT expr RPARENT THEN ifstmt ELSE ifstmt { $$ = new TripleNode($3, $6, $8, "if-then-else"); } 
;

ifstmt
: stmt { $$ = $1; }
| TBEGIN multistmt END { $$ = $2; }
;

%%

