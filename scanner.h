#ifndef SCANNER_H_
#define SCANNER_H_

enum yytokentype {
    NUMBER = 258,
    ADD,
    SUB,
    MUL,
    DIV,
    ABS,
    EOL
};
extern int yylval;
/*extern "C" { */int yylex(void); //}

#endif

