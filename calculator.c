#include <stdio.h>
#include "scanner.h"

main(int argc, char** argv)
{
    int tok;
    while(tok = yylex()) {
        printf("%d", tok);
	if (tok==NUMBER) printf(" = %d\n", yylval);
	else printf("\n");
    }
}
