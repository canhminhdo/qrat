/* Section 1: Declarations */
%{
#include <stdio.h>
#include "parser.hpp"
void yyerror(const char *s);
int yylex (void);
%}

/* declare tokens */
%token NUMBER
%token ADD SUB MUL DIV ABS
%token OP CP
%token EOL

/* Section 2: BNF rules and actions */
%%

calclist: /* nothing, matches at beginning of input */
          | calclist exp EOL { printf("= %d\n> ", $2); } /* EOL is end of an expression */
          | calclist EOL { printf("> "); } /* blank line or a comment */
          ;

exp:      factor   /* default action $$ = $1 */
          | exp ADD factor { $$ = $1 + $3; }
          | exp SUB factor { $$ = $1 - $3; }
          ;

factor:   term
          | factor MUL term { $$ = $1 * $3; }
          | factor DIV term { $$ = $1 / $3; }
          ;

term:     NUMBER
          | ABS term  { $$ = $2 > 0 ? $2 : -$2; }
          | OP exp CP { $$ = $2; }
          ;

%%

/* Section 3: C code */
int main(void)
{
    printf("> ");
    yyparse();
}

void yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
}