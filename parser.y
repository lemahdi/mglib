/* calculator with AST */

%{
#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
%}

%union {
	struct ast *a;
	struct coord *d;	/* row, col, val */
	struct coord *r;	/* row, col */
	int ix;				/* index */
}

/* declare tokens */
%token <d> NUMBER
%token <r> NAME
%token <ix> INDEX
%token EOL

%left '+' '-'
%left '*' '/'
%right ';'

%type <a> exp

%start calclist

%%
	
exp: exp '+' exp			{ $$ = newast('+', $1, $3); }
	| exp '-' exp			{ $$ = newast('-', $1, $3); }
	| exp '*' exp			{ $$ = newast('*', $1, $3); }
	| exp '/' exp			{ $$ = newast('/', $1, $3); }
	| '(' exp ')'			{ $$ = $2; }
	| '-' exp				{ $$ = newast('M', NULL, $2); }
	| NUMBER				{ $$ = newnum($1); }
	| NAME '[' INDEX ']'	{ $$ = newref($1, $3); }
;

calclist: /* nothing */
	| calclist exp ';' {
		printf("= %4.4g\n", eval($2));
	}
	| calclist exp EOL {
		printf("= %4.4g\n", eval($2));
		printf("> ");
	}
	| calclist error EOL { yyerrok; printf("> "); }
;
	
%%
