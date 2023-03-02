%{
#include <parser.auto.h>
%}

%option noyywrap

%e 1000

%%

true				{ return TRUE; }
false				{ return FALSE; }

[a-zA-Z]([a-zA-Z]|[0-9])* 	{ return KEY; }
[0-9]+\.[0-9]+\.[0-9]+		{ return VERSION; }
\"(\\.|[^\\"\n]*)\"		{ return STRING; }

":"				{ return ':'; }
"["				{ return '['; }
"]"				{ return ']'; }
"{"				{ return '{'; }
"}"				{ return '}'; }

[ \t\v\n\f]			{ }
"#"[^\n]*			{ }
.				{ return UNRECOGNIZED; }

%%

