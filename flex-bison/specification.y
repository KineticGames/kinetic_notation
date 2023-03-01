%{
#include <stdio.h>
#include "../include/token_info.h"
%}

%define api.value.type {token_info}

%token END 0

%%

sdl:	END
   ;

%%

extern FILE *yyin;

int main() {
    do {
	yyparse();
    } while (!feof(yyin));
}

