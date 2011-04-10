/*
 * helper fucntions for calculator
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "typedef.h"


#ifdef WIN32
static char*
strndup(const char *str, size_t n)
{
  char *ret = malloc((n+1) * sizeof (char));

  if (!ret)
    exit(0);

  strncpy(ret, str, n);
  ret[n] = 0;

  return ret;
}
#endif

int
level(char *text)
{
	if (strlen(text) == 1)
		return 0;
	else
	{
		char *ix = strndup(text+2, strlen(text)-2);
		int i = atof(ix);
		free(ix);
		return i;
	}
}

/* symbol table */
/* hash a symbol */
static unsigned
astrefhash(char *sym)
{
	unsigned int hash = 0;
	unsigned c;

	while (c = *sym++) hash = hash*9 ^ c;

	return hash;
}


struct coord*
getcoord(char *text, enum coordtype ct)
{
	size_t len1 = 0;
	size_t len2 = 0;

	while (text[len2] != '\0')
	{
		if (text[len1] != ':')
			len2 = ++len1;
		len2++;
		if (text[len2] == ':' && text[len1] == ':')
		{
			len2--;
			break;
		}
	}
	{
	char* num1 = strndup(text, len1);
	char* num2 = strndup(text+2, len2-len1);

	struct coord *c = NULL;
	switch (ct) {
	case T_num:
		{
			struct coordnum *cc = malloc(sizeof(struct coordnum));
			char* num3 = strndup(text+len1+len2+2, strlen(text)-len1-len2-2);
			cc->val = atof(num3);
			free(num3);
			c = (struct coord *)cc;
		}
		break;
	case T_ref:
		{
			struct coordref *cr = malloc(sizeof(struct coordref));
			char *r_c = strndup(text+len2+3, strlen(text)-len2-3);
			int idx = 0;
			while (r_c[idx] != '$')
				idx++;
			r_c[idx] = ':';
			{
			int index = astrefhash(r_c);
			free(r_c);
			cr->ref = astreftab[index%NHASH].c;
			c = (struct coord *)cr;
			}
		}
		break;
	}
	c->row = atof(num1);
	c->col = atof(num2);

	free(num1);
	free(num2);
	
	return c;
	}
}

struct coord*
reg(char *text, enum coordtype ct)
{
	int index = 0;
	struct coord *c = getcoord(text, ct);
	int len = 0;
	{
	while (text[len+1] != '\0')
	{
		if (text[len] == ':' && text[len+1] == ':')
			break;
		len++;
	}
	}
	{
	char* hash_text = strndup(text, len);

	/* getting the reference to ast */
	index = astrefhash(hash_text);
	free(hash_text);
	}
	{
	struct astref *sp = &astreftab[index%NHASH];
	int scount = NHASH; /* how many have we looked at */

	while (--scount >= 0) {
		if (sp->c && sp->c->col*sp->c->row!=0)
		{
			if (sp->a) treefree(sp->a);
			free(sp->c);
			
			sp->c = c;
			sp->a = NULL;
			sp->c->index = index%NHASH;

			break;
		}

		if (!sp->c) { /* new entry */
			sp->c = c;
			sp->a = NULL;
			sp->c->index = index%NHASH;
			break;
		}

		if (++sp >= astreftab+NHASH)
			sp = astreftab; /* try the next entry */
	}
	if (scount  < 0) {
		yyerror("symbol table overflow\n");
		abort(); /* tried them all, table is full */
	}
	return sp->c;
	}
}

struct coord*
regnum(char *text)
{
	/* getting number */
	struct coord* c = reg(text, T_num);
	return c;

}

struct coord*
regref(char* text)
{
	/* getting reference */
	int len = 0;
	while (text[len] != '\0')
	{
		if (text[len] == ':' && text[len+1] == ':')
			break;
		len++;
	}

	{
	struct coord* c = reg(text, T_ref);
	return c;
	}
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
newnum(struct coord *d)
{
	struct coordnum * dn = (struct coordnum *)d; 
	struct numval *a = malloc(sizeof(struct numval));

	if (!a) {
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'K';
	a->number = dn->val;
	{
	struct astref *ar = &astreftab[dn->index];
	ar->a = (struct ast*)a;
	return (struct ast *)a;
	}
}

struct ast *
newref(struct coord *cr, int i)
{i;
{
	struct coordref *crr = (struct coordref *)cr;
	struct cellref *c = malloc(sizeof(struct cellref));

	if (!c) {
		yyerror("out of space");
		exit(0);
	}
	c->nodetype = 'R';
	{
	struct astref *crrr = &astreftab[crr->ref->index];
	c->l = crrr->a;
	astreftab[crr->index].a = crrr->a;
	return (struct ast *)c;
	}
}
}

struct ast *
newidx(struct ast *l)
{
	struct index *i = malloc(sizeof(struct index));

	if (!i) {
		yyerror("out of space");
		exit(0);
	}
	i->nodetype = 'I';
	i->l = l;

	return (struct ast *)i;
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
		case 'M': case 'F': case 'I':
			treefree(a->l);
			break;

		case 'K': case 'R':
			break;

		default: printf("internal error: free bad node %c\n", a->nodetype);
	}

	free(a); /* always free the node itself */
}

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

		case 'R': v = eval(((struct astref *)a)->a); break;

		default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
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
	/*ostringstream os1, os2;
	os1 << "Start;End;IT;Nominal;Strike;Spot;Payoff";
	os2 << "3;183;(Start[i]-End[i])/365;";
	os2 << "1000;25;39;";
	os2 << "MAX(Spot[i]-Strike[i],0)*IT[i]*Nominal[i]*DF(Start[i])";*/
	printf("> ");
	return yyparse();
}
