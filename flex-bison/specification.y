%{
#include <stdio.h>
%}

%token A

%%

b:	A
 ;

%%

extern FILE *yyin;

int main() {
    do {
	yyparse();
    } while (!feof(yyin));
}

