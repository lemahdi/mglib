/* recognize tokens for the C++ calculator and print them out */

%option noyywrap
%{
#include <stdlib.h>

#include "cppcalc-ctx.hh"
#include "cppcalc.tab.hh"

#define YY_DECL int yylex(yy::cppcalc::semantic_type *yylval, \
	yy::cppcalc::location_type *yylloc, cppcalc_ctx &ctx)

// make location include the current token
#define YY_USER_ACTION yylloc->columns(yyleng);

typedef yy::cppcalc::token token;
extern int myatoi(int radix, char *s); // defined in the parser
%}

%%

	// rules for C++-compatible scanner
%{
	// start where previous token ended
	yylloc->step();
%}

"+"		{ return token::ADD; }
"-"		{ return token::SUB; }
"*"		{ return token::MUL; }
"/"		{ return token::DIV; }
"|"		{ return token::ABS; }
"("		{ return token::OP; }
")"		{ return token::CP; }
[0-9]+	{ yylval->ival = myatoi(ctx.getradix(), yytext); return token::NUMBER; }

\n		{ yylloc->lines(1); return token::EOL; }

	/* skip over comments and whitespace */
"//".* |
[ \t]	{ yylloc->step(); }

.		{ printf("Mystery character %c\n", *yytext); }

%%
