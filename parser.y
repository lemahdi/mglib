/* calculator with AST */

%{
#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
%}

%union {
	struct ast *a;
	double d;
	struct symbol *s;	/* which symbol */
	int fn;				/* which function */
	char *i;			/* index */
}

/* declare tokens */
%token <d> NUMBER
%token <s> NAME
%token <fn> FUNC
%token <i> INDEX
%token EOL

%left '+' '-'
%left '*' '/'

%type <a> exp explist

%start calclist

%%
	
exp: exp '+' exp			{ $$ = newast('+', $1, $3); }
	| exp '-' exp			{ $$ = newast('-', $1, $3); }
	| exp '*' exp			{ $$ = newast('*', $1, $3); }
	| exp '/' exp			{ $$ = newast('/', $1, $3); }
	| '(' exp ')'			{ $$ = $2; }
	| '-' exp				{ $$ = newast('M', NULL, $2); }
	| NUMBER				{ $$ = newnum($1); }
	| FUNC '(' explist ')'	{ $$ = newfunc($1, $3); }
	| NAME '[' INDEX ']'	{ $$ = newcellref($1, $3); }
;

explist: exp
	| exp ',' explist		{ $$ = newast('L', $1, $3); }
;

calclist: /* nothing */
	| calclist exp EOL {
		printf("= %4.4g\n", eval($2));
		treefree($2);
		printf("> ");
	}
	| calclist error EOL { yyerrok; printf("> "); }
	;
	
%%
