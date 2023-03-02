%{
#include <parser.auto.h>
#include "../include/scanner.h"
#include "../include/object_list.h"

// std
#include <string.h>
#include <stdio.h>

#define yyterminate() return( END )

extern value_types yylval;

/*
int yycolumn = 1;
token_info yylloc;

#define YY_USER_ACTION yylloc.line_start = yylloc.line_end = yylineno; \
	yylloc.column_start = yycolumn; yylloc.column_end = yycolumn+yyleng-1; \
	yycolumn += yyleng;

#define DEFAULT_BUILD(location) build_string_value(yytext, (location), true)
#define BUILD_INVALID(location) build_string_value(yytext, (location), false)
*/
%}

%top {
#include <stdint.h>
}

%option nodefault
%option noyywrap

%e 1000

%%

true				{ yylval.boolean = true; return BOOLEAN; }
false				{ yylval.boolean = false; return BOOLEAN; }

[a-zA-Z]([a-zA-Z]|[0-9])* 	{ yylval.string = strdup(yytext); return KEY; }
[0-9]+\.[0-9]+\.[0-9]+		{ 
		sscanf(yytext, 
			"%d.%d.%d", 
			&yylval.version.major, 
			&yylval.version.minor, 
			&yylval.version.patch); 
		return VERSION; 
	}
\"(\\.|[^\\"\n]*)\"		{ 
		yylval.string = strdup(yytext+1); 
		yylval.string[yyleng-2] = '\0'; 
		return STRING; 
	}

":"				{ return ':'; }
"["				{ return '['; }
"]"				{ return ']'; }
"{"				{ return '{'; }
"}"				{ return '}'; }

[ \t\v\n\f]			{ }
"#"[^\n]*			{ }
.				{ return UNRECOGNIZED; }

%%

