%{   
#include <stdio.h>
#include <stdlib.h>

char *build_file_name;
bool force_print_tree = false;
class Node;
#include "valirian.tab.h"

int yyerror(const char *s);
%}

%option yylineno

%%


[ \t\r\n] { /* ignora */ }

"//".*\n { /* ignora */ }

"bardugon" { return TOK_PRINT; }
"pikibagon" { return TOK_SCAN; }

"syt" { return TOK_LOOP; }

"lo" { return TOK_IF; }
"daor" { return TOK_ELSE; }

"giez" { return TOK_TIPOINT; }
"bevumbagon" { return TOK_TIPOFLOAT; } 
"engos" { return TOK_TIPOSTRING; }
"iderennon" { return TOK_TIPOBOOL; }

"dreje" { return TOK_TRUE; }
"pirta" { return TOK_FALSE; }

"++" { return TOK_INC; }
"--" { return TOK_DEC; }
"<=" { return TOK_MENORI; }
">=" { return TOK_MAIORI; }
"==" { return TOK_IGUAL; }
"!=" { return TOK_DIFE; }
"||" { return TOK_OR; }
"&&" { return TOK_AND; }

"<" { return '<'; }
">" { return '>'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"(" { return '('; }
")" { return ')'; }
";" { return ';'; }
"=" { return '='; }
"{" { return '{'; }
"}" { return '}'; }
"%" { return '%'; }

\"[a-zA-Z0-9_' ']+\" {
    yylval.str = strndup(yytext,yyleng);
    return TOK_STRING;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strndup(yytext,yyleng);
    return TOK_IDENT;
}

[0-9]+\.[0-9]* {
	yylval.flt = atof(yytext);
    return TOK_FLOAT;
}

[0-9]+ {
    yylval.itg = atoi(yytext);
    return TOK_INT;
}

. {
    printf("Simbolo nao reconhecido%c\n", yytext[0]);
    }


%%

int yywrap() {
    return 1;
}

int yyerror(const char *s){
    printf("Erro de sintaxe na linha %d: %s\n", yylineno, s);
    return 1;
}

int main(int argc, char *argv[]){

    if(argc <=1){
        printf("Sintaxe: %s nome_do_programa\n", argv[0]);
        return 1;
    }

    int build_file_id=1;

	if(strcmp(argv[1], "-f")==0){
		force_print_tree = true;
		build_file_id++;
	}

    build_file_name = argv[build_file_id];
    yyin = fopen(argv[1], "r");
    if (yyin == NULL){
        printf("Não foi possível abrir o arquivo %s.\n", argv[1]);
        return 1;
    }

    yyparse();

    if (yyin)
        fclose(yyin);

    return 0;

}