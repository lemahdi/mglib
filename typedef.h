/*
 * Declaration for a calculator
 */

/* interface to the lexer */
extern int yylineno; /* from lexer */
void yyerror(char* s, ...);

/* symbol table */
struct symbol { /* a variable name */
	char *name;
	double value;
};

/* simple symtab of fixed size */
#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char *);

/* node types
 * M unary minus
 * F built in fucntion call
 * R cell
 */

enum bifs { /* built-in functions */
	B_sqrt = 1,
	B_exp,
	B_log,
	B_if,
	B_abs,
	B_df
};

/* nodes in the abstract syntax tree */
/* all have common initial nodetype */

struct ast {
	int nodetype;
	struct ast *l;
	struct ast *r;
};

struct fncall {		/* built-in function */
	int nodetype;	/* type F */
	struct ast *l;
	enum bifs functype;
};

struct numval {
	int nodetype; /* type K for constant */
	double number;
};

struct cellref {
	int nodetype;	/* type R */
	char *i;
	struct symbol *s;
};

/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newnum(double d);
struct ast *newcellref(struct symbol *s, char *i);

/* evaluate an AST */
double eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

/* interface to the lexer */
extern int yylineo; /* from lexer */
void yyerror(char *s, ...);
