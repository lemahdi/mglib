/* recognize tokens for the calculator */
%option noyywrap nodefault yylineno
%{
#include "typedef.h"
#include "parser.tab.h"
%}

/* float exponent */
EXP	([Ee][-+]?[0-9]+)
IDENTIFIER ([a-zA-Z][a-zA-Z0-9]*)
INT ([0-9]+)


%%

";" |
"+" |
"-" |
"*" |
"/" |
"," |
"(" |
")" |
"[" |
"]"		{ return yytext[0]; }

[i]([+-]?[0-9]+)?	{ yylval.ix = level(yytext); return INDEX; }

([0-9]+":"[0-9]+"::")?[0-9]+"$"[0-9]+ { yylval.r = regref(yytext); return NAME; }

[0-9]+":"[0-9]+"::"[0-9]+"."[0-9]*{EXP}? |
[0-9]+":"[0-9]+"::""."?[0-9]+{EXP}? { yylval.d = regnum(yytext); return NUMBER; }

"//".*
[ \t] /* ignore whitespace */

\\\n	{ printf("c> "); } /* ignore line continuation */

\n		{ return EOL; }

.		{ yyerror("Mystery character %c\n", *yytext); }


%%
