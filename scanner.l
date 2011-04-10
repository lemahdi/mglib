/* recognize tokens for the calculator and print them out */
%{
  #include "typedef.h"
  #include "parser.tab.h"
%}

%%

"+"	{ return ADD; }
"-"	{ return SUB; }
"*"	{ return MUL; }
"/"	{ return DIV; }
"|"	{ return ABS; }
[0-9]+	{ yylval=atoi(yytext); return NUMBER; }
\n	{ return EOL; }
[ \t]	{ /* ignore white spaces */ }
.	{ printf("Mystery character %c\n", *yytext); }

%%

int yywrap(void) { return 1; }
