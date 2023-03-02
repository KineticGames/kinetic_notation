%code requires {
#include "object_list.h"

#define YYSTYPE value_types
}

%code {
#include <stdio.h>

void yyerror(char *s);
struct statement *head;
}

%token END 0
%token UNRECOGNIZED
%token <string> KEY
%token <version> VERSION
%token <string> STRING

%token TRUE
%token FALSE

%type <string> string
%type <boolean> boolean
%type <version> version

%%

std:		    END
   |		    statement_list END
   ;

statement:	    KEY ':' value		{ printf("%s\n", $1); }
	 ;

value:		    array
     |		    string
     |		    version
     |		    boolean
     ;

boolean:	    TRUE		    	{ $$ = 1; }
       |    	    FALSE		    	{ $$ = 0; }
       ;

string:		    STRING		    	{ $$ = $1; }
      ;

version:	    VERSION		    	{ $$ = $1; }
      ;

array:		    '[' object_list ']'
     ;

object_list:	    object
	   |	    object_list object
	   ;

object:		    '{' statement_list '}'
      ;

statement_list:	    statement
	      |	    statement_list statement
	      ;

%%

extern FILE *yyin;

int main() {
    do {
	yyparse();
    } while (!feof(yyin));
}

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}
