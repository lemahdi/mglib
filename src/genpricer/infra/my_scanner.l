/* recognize tokens for the C++ calculator and print them out */

%option noyywrap
%{
#include <stdlib.h>

#include "genpricer/infra/nodes.h"

#pragma warning(push)
#pragma warning(disable:4512)
#pragma warning(disable:4267)
#pragma warning(disable:4996)
#pragma warning(disable:4003)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4512)
#pragma warning(disable:4127)
#pragma warning(disable:4100)
#pragma warning(disable:4505)

#include "genpricer/infra/my_parser.tab.hpp"

#ifdef WIN32
#define strdup _strdup
#endif

#define YY_DECL int yylex(yy::my_parser::semantic_type *yylval, \
	yy::my_parser::location_type *yylloc, MG::MG_TableWalker &walker, MG::MG_NodeManager &manager)

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

">"		{ yylval->uval = GT_OP; return token::CMP; }
"<"		{ yylval->uval = LT_OP; return token::CMP; }
"!="	{ yylval->uval = NE_OP; return token::CMP; }
"=="	{ yylval->uval = EQ_OP; return token::CMP; }
">="	{ yylval->uval = GE_OP; return token::CMP; }
"<="	{ yylval->uval = LE_OP; return token::CMP; }

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
