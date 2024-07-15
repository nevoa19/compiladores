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

%printer { fprintf(yyo, "%s", $$);} <str>
%printer { fprintf(yyo, "%d", $$);} <itg>
%printer { fprintf(yyo, "%f", $$);} <flt>
%printer { fprintf(yyo, "%s", $$-> toDebug().c_str());} <node>

%start program

%%


program : globals {
   Node *program = new Program();
    program->append($globals);

    cout << "Checking variable declarations..." << endl;
    CheckVarDecl cvd;
    cvd.check(program);

    cout << "deu..." << endl;

    if (errorcount > 0)
        cout << errorcount << " error(s) found" << endl;
    if (force_print_tree || errorcount == 0)
        printf_tree(program);
    else
        cout << "Errors found, not printing the tree." << endl;
}


globals : globals[gg] global {
    cout << "deu 1 globals globals global..." << endl;
    $gg->append($global);
    cout << "deu2..." << endl;
    $$ = $gg;
    cout << "deu3..." << endl;
}

globals : global {
    cout << "deu?" << endl;
    Node *n = new Node();
    cout << "deu!?" << endl;
    n->append($global);
    cout << "deu3..." << endl;
    $$ = n;
    cout << "deu3!!!" << endl;
}

global : TOK_IDENT '=' expr ';' {
    $$ = new Attribution($TOK_IDENT, $expr);
}

global : TOK_IDENT '=' scan ';' {
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

term : factor {
    $$ = $factor;
}

factor : '(' expr ')' {
    $$ = $expr;
}

factor : TOK_IDENT[str] {
    cout << "deu????" << endl;
    $$ = new Ident($str);
}

factor : TOK_INT[itg] {
    cout << "deu????" << endl;
    $$ = new Integer($itg);
}

factor : TOK_FLOAT[flt] {
    cout << "deu????" << endl;
    $$ = new Float($flt);
}

factor : TOK_TRUE[str]{
    cout << "deu????" << endl;
    $$ = new True();
}

factor : TOK_FALSE[str]{
    cout << "deu????" << endl;
    $$ = new False();
}

factor : unary[u] {
    $$ = $u;
}

unary : '-' factor[f] {
    $$ = new Unary($f, '-');
}

global : TOK_LOOP '(' decl cond ';' pass ')' '{' globals '}'{
    $$ = new Loop($decl, $opr, $pass, $globals);
}

pass : TOK_IDENT TOK_DEC {
    $$ = new Pass($TOK_IDENT, "--");
}

pass : TOK_IDENT TOK_INC {
    $$ = new Pass($TOK_IDENT, "++");
}

opr : TOK_MENORI {
    $$ = $TOK_MENORI;
}

opr : TOK_MAIORI {
    $$ = $TOK_MAIORI;
}

opr : TOK_IGUAL {
    $$ = $TOK_IGUAL;
}

opr : TOK_DIFE{
    $$ = $TOK_DIFE;
}

opr : '<' {
    $$ = "<";
}
opr : '>' {
    $$ = ">";
}

cond : factor[ee1] opr factor[ee2] {
    $$ = new Condition($ee1, $ee2, $opr);
}

cond : '('cond TOK_OR cond ')'{
}

cond : '('cond TOK_AND cond ')'{
}

decl : tip TOK_IDENT '=' der ';'{
    cout << "deu???????" << endl;
    cout << "nao deu :c" << endl;
    $$ = new Variable(new TypeDec($tip), $TOK_IDENT, $der);
    cout << "deu simmm!" << endl;
}

decl : tip TOK_IDENT '=' scan ';'{
}

der : expr | TOK_STRING {
}

if : TOK_IF '('  cond  ')' '{' globals '}'{
}

if : TOK_IF '('  cond  ')' '{' globals '}' TOK_ELSE '{' globals '}'{
}

if : TOK_IF '('  cond  ')' '{' globals '}' TOK_ELSE if{
}

print : TOK_PRINT '(' TOK_STRING ')' ';'{
}

print : TOK_PRINT '(' TOK_IDENT ')' ';'{
}

tip : TOK_TIPOBOOL{
    $$ = 3;
}

tip : TOK_TIPOSTRING{
    $$ = 4;
}

tip : TOK_TIPOFLOAT{
    $$ = 2;
}

tip : TOK_TIPOINT{
    $$ = 1;
}

scan : TOK_SCAN '(' tip ')'{
}

%%