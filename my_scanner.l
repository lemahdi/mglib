/* recognize tokens for the C++ calculator and print them out */

%option noyywrap
%{
#include <stdlib.h>

#include "nodes.h"
#include "my_parser.tab.hpp"

#ifdef WIN32
#define strdup _strdup

#pragma warning(push)
  #pragma warning(disable:4267)
  #pragma warning(disable:4996)
  #pragma warning(disable:4003)
  #pragma warning(disable:4018)
  #pragma warning(disable:4244)
#endif

#define YY_DECL int yylex(yy::my_parser::semantic_type *yylval, \
	yy::my_parser::location_type *yylloc, MG_TableWalker &walker, MG_NodeManager &manager)

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
")"	|
"[" |
"]" |
","		{ return yytext[0]; }

[a-zA-Z][a-zA-Z0-9]*							{ yylval->cval = strdup(yytext); return token::NAME; }

[0-3][0-9]"/"[0-1][0-9]"/"[1-2][0-9][0-9][0-9]	{ yylval->lval = manager.ToJulianDay(yytext); return token::DATE; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?								{ yylval->dval = atof(yytext); return token::NUMBER; }

\n		{ yylloc->lines(1); return token::EOL; }

	/* skip over comments and whitespace */
"//".* |
[ \t]	{ yylloc->step(); }

.		{ printf("Mystery character %c\n", *yytext); }

%%

#ifdef WIN32
#pragma warning(pop)
#endif
