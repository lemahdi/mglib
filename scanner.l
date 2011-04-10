/* recognize tokens for the calculator */
%option noyywrap nodefault yylineno
%{
#include "typedef.h"
#include "parser.tab.h"
%}

/* float exponent */
EXP	([Ee][-+]?[0-9]+)


%%


"+" |
"-" |
"*" |
"/" |
"," |
"(" |
")" |
"[" |
"]"		{ return yytext[0]; }

"SQRT"	{ yylval.fn = B_sqrt; return FUNC; }
"EXP"	{ yylval.fn = B_exp; return FUNC; }
"LOG"	{ yylval.fn = B_log; return FUNC; }
"IF"	{ yylval.fn = B_if; return FUNC; }
"ABS"	{ yylval.fn = B_abs; return FUNC; }
"DF"	{ yylval.fn = B_df; return FUNC; }

i([+-]?[0-9]+)?	{ yylval.i = strdup(yytext); return INDEX; }

[a-zA-Z][a-zA-Z0-9]* { yylval.s = lookup(yytext); return NAME; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.d = atof(yytext); return NUMBER; }

"//".*
[ \t] /* ignore whitespace */

\\\n	{ printf("c> "); } /* ignore line continuation */

\n		{ return EOL; }

.		{ yyerror("Mystery character %c\n", *yytext); }


%%
