/*
 * Declaration for GP
 */
#pragma once

#pragma warning (disable : 4267)
#pragma warning (disable : 4996)
#pragma warning (disable : 4003)
#pragma warning (disable : 4018)
#pragma warning (disable : 4244)
#pragma warning (disable : 4003)
#pragma warning (disable : 4013)
#pragma warning (disable : 4273)

/* interface to the lexer */
extern int yylineno; /* from lexer */
void yyerror(char* s, ...);


int level(char *text);


/* coordinates */
enum coordtype {
	T_num = 1,
	T_ref
};

struct coord {
	int row;
	int col;
	int index;
};

struct coordnum {
	int row;
	int col;
	int index;
	double val;
};

struct coordref {
	int row;
	int col;
	int index;
	struct coord* ref;
};

struct coord *getcoord(char *text, enum coordtype ct);
struct coord *regnum(char *text);
struct coord *regref(char *text);
struct coord *reg(char *text, enum coordtype);


/* node types
 * M unary minus
 * K constants
 * R cell reference
 * I cell index
 */

/* nodes in the abstract syntax tree */
/* all have common initial nodetype */

struct ast {
	int nodetype;
	struct ast *l;
	struct ast *r;
};

struct numval {		/* constant */
	int nodetype;	/* type K */
	double number;
};

struct index {		/* index */
	int nodetype;	/* type I */
	struct ast *l;
};

struct cellref {	/* reference to another cell */
	int nodetype;	/* R */
	struct ast *l;
};

struct astref {		/* col::ast */
	struct coord *c;		/* coordinates */
	struct ast *a;
};

/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(struct coord* c);
struct ast *newref(struct coord* c, int i);
struct ast *newidx(struct ast *l);

/* evaluate an AST */
double eval(struct ast *a);

/* delete and free an AST */
void treefree(struct ast *a);


/* simple astreftab of fixed size */
#define NHASH 9997
struct astref astreftab[NHASH];


/* interface to the lexer */
extern int yylineo; /* from lexer */
void yyerror(char *s, ...);
