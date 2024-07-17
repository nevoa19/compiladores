%{
#include "nodes.h"

int yyerror(const char *s);
int yylex(void);
int errorcount = 0;
extern bool force_print_tree;
%}

%define parse.error verbose

%union {
    const char *str;
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

%type<str> tip TOK_IDENT TOK_STRING TOK_SCAN TOK_TIPOINT TOK_TIPOFLOAT TOK_TIPOSTRING TOK_TIPOBOOL
%type<itg> TOK_INT 
%type<flt> TOK_FLOAT
%type<node> globals global expr term factor unary pass cond decl der if print scan

%printer { fprintf(yyo, "%s", $$);} <str>
%printer { fprintf(yyo, "%d", $$);} <itg>
%printer { fprintf(yyo, "%f", $$);} <flt>
%printer { fprintf(yyo, "%s", $$-> toDebug().c_str());} <node>

%start program

%%


program : globals {
    Node *program = new Program();
    program->append($globals);

    CheckVarDecl cvd;
    cout << "Checking variable declarations..." << endl;
    cvd.check(program);
    cout << "erros: " << errorcount << endl;

    CheckVarMix cvm;
    cout << "Checking type mix declarations..." << endl;
    cvm.check(program);
    cout << "erros: " << errorcount << endl;

    CheckDuplicateVariable cdv;
    cout << "Checking duplicate variable declarations..." << endl;
    cdv.check(program, "global"); 
    cout << "erros: " << errorcount << endl;

    if (errorcount > 0)
        cout << errorcount << " error(s) found" << endl;
    if (force_print_tree || errorcount == 0)
        printf_tree(program);
    else
        cout << "Errors found, not printing the tree." << endl;
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

global : TOK_IDENT '=' expr ';' {
    $$ = new Attribution($TOK_IDENT, $expr);
}

global : TOK_IDENT '=' scan ';' {
    $$ = new Scan();
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

term : term[tt] '%' factor {
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
    $$ = new True();
}

factor : TOK_FALSE{
    $$ = new False();
}

factor : unary[u] {
    $$ = $u;
}

unary : '-' factor[f] {
    $$ = new Unary($f, '-');
}

global : TOK_LOOP '(' decl cond ';' pass ')' '{' globals '}'{
    $$ = new Loop($decl, $cond, $pass, $globals);
}

pass : TOK_IDENT TOK_DEC {
    $$ = new Pass($TOK_IDENT, "--");
}

pass : TOK_IDENT TOK_INC {
    $$ = new Pass($TOK_IDENT, "++");
}

cond : factor[f1] TOK_MENORI factor[f2] {
    $$ = new Condition($f1, $f2, "<=");
}

cond : factor[f1] TOK_MAIORI factor[f2] {
    $$ = new Condition($f1, $f2, ">=");
}

cond : factor[f1] TOK_IGUAL factor[f2] {
    $$ = new Condition($f1, $f2, "==");
}

cond : factor[f1] TOK_DIFE factor[f2] {
    $$ = new Condition($f1, $f2, "!=");
}

cond : factor[f1] '<' factor[f2] {
    $$ = new Condition($f1, $f2, "<");
}

cond : factor[f1] '>' factor[f2] {
    $$ = new Condition($f1, $f2, ">");
}

cond : '('cond[c1] TOK_OR cond[c2] ')'{
    $$ = new Condition($c1, $c2, "||");
}

cond : '('cond[c1] TOK_AND cond[c2] ')'{
    $$ = new Condition($c1, $c2, "&&");
}

decl : tip TOK_IDENT '=' der ';'{
    $$ = new Variable(new TypeDec($tip), $TOK_IDENT, $der);
}

decl : tip TOK_IDENT[str] '=' scan ';'{
    $$ = new Variable(new TypeDec($tip), $str, new Scan());
}

der : expr {
    $$ = $expr; 
}
| TOK_STRING[str] {
    $$ = new String($str);
}

if : TOK_IF '('  cond  ')' '{' globals '}'{
    $$ = new If($cond, $globals);
}

if : TOK_IF '(' cond ')' '{' globals[g1] '}' TOK_ELSE '{' globals[g2] '}' { 
    $$ = new IfElse($cond, $g1, $g2);
    }

print : TOK_PRINT '(' TOK_STRING[str] ')' ';'{
    String *value = new String($str);
    $$ = new Print(value);
}

print : TOK_PRINT '(' TOK_IDENT[str] ')' ';'{
    Ident *id = new Ident($str);
    $$ = new Print(id);
}

tip : TOK_TIPOBOOL{
    $$ = "bool";
}

tip : TOK_TIPOSTRING{
    $$ = "string";
}

tip : TOK_TIPOFLOAT{
    $$ = "float";
}

tip : TOK_TIPOINT{
    $$ = "int";
}

scan : TOK_SCAN '(' tip ')'{
    $$ = new Scan(new TypeDec($tip));
}

%%