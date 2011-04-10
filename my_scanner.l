/* recognize tokens for the C++ calculator and print them out */

%option noyywrap
%{
#include <stdlib.h>

#include "nodes.h"
#include "my_parser.tab.hpp"

#define YY_DECL int yylex(yy::my_parser::semantic_type *yylval, \
	yy::my_parser::location_type *yylloc, TableWalker &walker, NodeManager &manager)

// make location include the current token
#define YY_USER_ACTION yylloc->columns(yyleng);

typedef yy::my_parser::token token;
%}

/* float exponent */
EXP	([Ee][-+]?[0-9]+)

%%

	// rules for C++-compatible scanner
%{
	// start where previous token ended
	yylloc->step();
%}

"+" |
"-" |
"*" |
"/" |
"(" |
")"		{ return yytext[0]; }

[a-zA-Z][a-zA-Z0-9]* { yylval->nval = manager.GetChildNode(walker, yytext); return token::NAME; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval->dval = atof(yytext); return token::NUMBER; }

\n		{ yylloc->lines(1); return token::EOL; }

	/* skip over comments and whitespace */
"//".* |
[ \t]	{ yylloc->step(); }

.		{ printf("Mystery character %c\n", *yytext); }

%%
