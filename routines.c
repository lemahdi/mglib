/*
 * helper fucntions for calculator
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "typedef.h"

#pragma warning (disable : 4996)
#pragma warning (disable : 4244)
#pragma warning (disable : 4013)
/* symbol table */
/* hash a symbol */
static unsigned
symhash(char *sym)
{
	unsigned int hash = 0;
	unsigned c;

	while (c = *sym++) hash = hash*9 ^ c;

	return hash;
}

static const char* column1 = "Start";
static const char* column2 = "End";
static const char* column3 = "Spot";
static const char* column4 = "Strike";
static const char* column5 = "Payoff";

struct symbol *
lookup(char* sym)
{
	struct symbol *sp = &symtab[symhash(sym)%NHASH];
	int scount = NHASH; /* how many have we looked at */

	int iscolumn = 1;
	iscolumn *= strcmp(sym, column1);
	iscolumn *= strcmp(sym, column2);
	iscolumn *= strcmp(sym, column3);
	iscolumn *= strcmp(sym, column4);
	iscolumn *= strcmp(sym, column5);

	/*if (iscolumn) {
		yyerror("token not element of the table\n");
	}*/

	while (--scount >= 0) {
		if (sp->name && !strcmp(sp->name, sym))
			return sp;

		if (!sp->name) { /* new entry */
			sp->name = strdup(sym);
			sp->value = 0;
			return sp;
		}

		if (++sp >= symtab+NHASH)
			sp = symtab; /* try the next entry */
	}

	yyerror("symbol table overflow\n");
	abort(); /* tried them all, table is full */
}

struct ast *
newast(int nodetype, struct ast *l, struct ast *r)
{
	struct ast *a = malloc(sizeof(struct ast));

	if (!a) {
		yyerror("out of space");
		exit(0);
	}

	a->nodetype = nodetype;
	a->l = l;
	a->r = r;

	return a;
}

struct ast *
newnum(double d)
{
	struct numval *a = malloc(sizeof(struct numval));

	if (!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'K';
	a->number = d;
	return (struct ast *)a;
}

struct ast *
newfunc(int functype, struct ast *l)
{
	struct fncall *a = malloc(sizeof(struct fncall));

	if (!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'F';
	a->l = l;
	a->functype = functype;
	return (struct ast *)a;
}

struct ast *
newcellref(struct symbol *s, char *i)
{
	struct cellref *c = malloc(sizeof(struct cellref));

	if (!c) {
		yyerror("out of space");
		exit(0);
	}
	c->nodetype = 'R';
	c->s = s;
	c->i = i;
	return (struct ast *)c;
}

/* free a tree of ASTs */
void
treefree(struct ast *a)
{
	switch(a->nodetype) {

			/* two subtrees */
		case '+':
		case '-':
		case '*':
		case '/':
		case 'L':
			treefree(a->r);

			/* one subtree */
		case 'M': case 'F':
			treefree(a->l);
			break;

			/* no subtree */
		case 'R':
			free(((struct cellref *)a)->i);
			break;

		case 'K': case 'N':
			break;

		default: printf("internal error: free bad node %c\n", a->nodetype);
	}

	free(a); /* always free the node itself */
}

static double callbuiltin(struct fncall *);

double
eval(struct ast *a)
{
	double v;

	if (!a) {
		yyerror("internal error, null eval");
		return 0.0;
	}

	switch(a->nodetype) {
		/* constant */
		case 'K': v = ((struct numval *)a)->number; break;

		/* expressions */
		case '+': v = eval(a->l) + eval(a->r); break;
		case '-': v = eval(a->l) - eval(a->r); break;
		case '*': v = eval(a->l) * eval(a->r); break;
		case '/': v = eval(a->l) / eval(a->r); break;
		case 'M': v = -eval(a->l); break;

		/* list of statements */
		case 'L': eval(a->l); v = eval(a->r); break;

		case 'F': v = callbuiltin((struct fncall *)a); break;

		case 'R': v = ((struct symbol *)a)->value; break;

		default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
}

static double
callbuiltin(struct fncall *f)
{
	enum bifs functype = f->functype;
	double v = eval(f->l);
	
	switch(functype) {
		case B_sqrt:
			return sqrt(v);
		case B_exp:
			return exp(v);
		case B_log:
			return log(v);
		case B_abs:
			return abs(v);
		case B_if:
			if (v)
				return v;
			else
				return -v;
		case B_df:
			return exp(-0.025*v);
		default:
			yyerror("Unknown built-in  function %d", functype);
			return 0.0;
	}
}

void
yyerror(char *s, ...)
{
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

int main()
{
	printf("> ");
	return yyparse();
}
