/* recognize tokens for the calculator and print them out */
%{
  #include "scanner.h"
  yylval = 1;
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
