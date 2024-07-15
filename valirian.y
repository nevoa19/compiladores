%{
#include "nodes.h"
#include "valirian.tab.h"

int yyerror(const char *s);
int yylex(void);
int errorcount = 0;
extern bool force_print_tree;
%}

%define parse.error verbose

%union {
    char *str;
    int itg;
    double flt;
    Node *node;
}

%token TOK_IDENT
%token TOK_PRINT
%token TOK_FLOAT
%token TOK_INT
%token TOK_STRING
%token TOK_SCAN
%token TOK_LOOP
%token TOK_IF
%token TOK_ELSE
%token TOK_TIPOINT
%token TOK_TIPOFLOAT
%token TOK_TIPOSTRING
%token TOK_TIPOBOOL
%token TOK_TRUE
%token TOK_FALSE
%token TOK_INC
%token TOK_DEC
%token TOK_MENORI
%token TOK_MAIORI
%token TOK_IGUAL
%token TOK_DIFE
%token TOK_OR
%token TOK_AND

%type<str> TOK_IDENT TOK_STRING TOK_SCAN TOK_TIPOINT TOK_TIPOFLOAT TOK_TIPOSTRING TOK_TIPOBOOL
%type<itg> TOK_INT tip
%type<flt> TOK_FLOAT
%type<node> globals global expr term factor unary pass opr decl der if print scan

%start program

%%

program : globals {
   Node *program = new Program();
    program->append($globals);

    CheckVarDecl cvd;
    cvd.check(program);
    
    if(errorcount>0)
        cout << errorcount << " error(s) found" <<endl;
    if(force_print_tree || errorcount ==0)
        printf_tree(program);
}

globals : globals[gg] global {
    $gg->append($global);
    $$ = $gg;
}

globals : global {
    Node *n = new Node();
    n->append($global);
    $$ = n;
}

global : TOK_IDENT '=' der ';' {
    $$ = new Variable($TOK_IDENT, $der);
}

global : TOK_IDENT '=' scan ';' {
    $$ = new Variable($TOK_IDENT, $scan);
}

global : if {
    $$ = $if;
}

global : print{
    $$ = $print;
}

global : decl{
    $$ = $decl;
}

expr : expr[ee] '+' term {
    $$ = new BinaryOp($ee, $term, '+');
}

expr : expr[ee] '-' term {
    $$ = new BinaryOp($ee, $term, '-');
}

expr : term {
    $$ = $term;
}

term : term[tt] '*' factor {
    $$ = new BinaryOp($tt, $factor, '*');
}

term : term[tt] '/' factor {
    $$ = new BinaryOp($tt, $factor, '/');
}

term : term[tt] '%' factor{
    $$ = new BinaryOp($tt, $factor, '%');
}

term : factor {
    $$ = $factor;
}

factor : '(' expr ')' {
    $$ = $expr;
}

factor : TOK_IDENT[str] {
    $$ = new Ident($str);
}

factor : TOK_INT[itg] {
    $$ = new Integer($itg);
}

factor : TOK_FLOAT[flt] {
    $$ = new Float($flt);
}

factor : TOK_TRUE{
    $$ = new Boolean(true);
}

factor : TOK_FALSE{
    $$ = new Boolean(false);
}

factor : unary[u] {
    $$ = $u;
}

unary : '-' factor[f] {
    $$ = new Unary($f, '-');
}

global : TOK_LOOP '(' decl opr ';' pass ')' '{' globals '}'{
    $$ = new Loop($decl, $opr, $pass, $globals);
}

pass : TOK_IDENT TOK_DEC {
    $$ = new Pass($TOK_IDENT, "--");
}

pass : TOK_IDENT TOK_INC {
    $$ = new Pass($TOK_IDENT, "++");
}


opr : expr[ee1] TOK_DIFE expr[ee2] {
    $$ = new Condition($ee1, $ee2, "!=");
}

opr : expr[ee1] TOK_MAIORI expr[ee2] {
    $$ = new Condition($ee1, $ee2, ">=");
}

opr : expr[ee1] TOK_MENORI expr[ee2] {
    $$ = new Condition($ee1, $ee2, "<=");
}

opr : expr[ee1] TOK_IGUAL expr[ee2] {
    $$ = new Condition($ee1, $ee2, "==");
}

opr : expr[ee1] '<' expr[ee2] {
   $$ = new BinaryOp($ee1, $ee2, '<');
}

opr : expr[ee1] '>' expr[ee2]{
    $$ = new BinaryOp($ee1, $ee2, '>');
}

opr : '(' opr[ee1] TOK_OR opr[ee2] ')' {
   $$ = new Condition($ee1, $ee2, "||");
}

opr : '(' opr[ee1] TOK_AND opr[ee2] ')' {
  $$ = new Condition($ee1, $ee2, "&&");
}

tip : TOK_TIPOBOOL {
    $$ = 3;
}

tip : TOK_TIPOSTRING {
    $$ = 4;
}

tip : TOK_TIPOFLOAT {
    $$ = 2;
}

tip : TOK_TIPOINT {
    $$ = 1;
}

decl : tip TOK_IDENT '=' der ';'{
    $$ = new Declaration($TOK_IDENT, $der);
}

decl : tip TOK_IDENT '=' scan ';'{
    $$ = new Declaration($TOK_IDENT, $scan);
}

der : expr {
    $$ = $expr;
}
| TOK_STRING {
    $$ = new Ident($TOK_STRING);
}

if : TOK_IF '(' opr ')' '{' globals '}' {
    $$ = new If($opr, $globals);
}

if : TOK_IF '(' opr ')' '{' globals[ee1] '}' TOK_ELSE '{' globals[ee2] '}' { 
    $$ = new IfElse($opr, $ee1, $ee2);
    }


print : TOK_PRINT '(' TOK_STRING ')' ';'{
    String *value = new String($TOK_STRING);
    $$ = new Print(value);
}

print : TOK_PRINT '(' TOK_IDENT ')' ';'{
    Ident *id = new Ident($TOK_IDENT);
    $$ = new Print(id);
}

scan : TOK_SCAN '(' tip ')'{
    $$ = new Scan($TOK_SCAN);
}

%%
