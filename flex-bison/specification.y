%code requires {
#include <stdbool.h>

#define YYLLOC_DEFAULT(Cur, Rhs, N)                                            \
  do {                                                                         \
    if (N) {                                                                   \
      token_info_combine(&(Cur), &YYRHSLOC(Rhs, 1));                           \
      token_info_combine(&(Cur), &YYRHSLOC(Rhs, N));                           \
    } else {                                                                   \
      token_info_combine(&(Cur), &YYRHSLOC(Rhs, 0));                           \
    }                                                                          \
  } while (0)

#define YYLOCATION_PRINT token_info_print
}

%code {
#include <stdio.h>

void yyerror(const char *s);
struct statement *head;
}

%union {
  char *string;
  struct {
    int major;
    int minor;
    int patch;
  } version;
  bool boolean;
}

%token END 0
%token UNRECOGNIZED
%token <string> KEY
%token <version> VERSION
%token <string> STRING
%token <boolean> BOOLEAN

%type <string> string
%type <boolean> boolean
%type <version> version

%locations

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

boolean:	    BOOLEAN			{ $$ = $1; }
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

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
