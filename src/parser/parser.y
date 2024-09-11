/* Section 1: Declarations */
%{
#include <stdio.h>
#include "parser/lexerAux.hpp"
#include "parser.hpp"
void yyerror(const char *s);
int yylex (void);
extern FILE *yyin;
extern char *yytext;
%}

/* declare tokens */
%token KW_PROG KW_IS KW_VAR KW_CONST KW_WHERE KW_INIT KW_BEGIN KW_END
%token KW_QUBIT KW_COMPLEX
%token IDENTIFIER
%token KW_SKIP
%token KW_IF KW_THEN KW_ELSE KW_FI
%token KW_WHILE KW_DO KW_OD
%token KW_ASSIGN
%token KW_KET_ZERO
%token KW_KET_ONE
%token NUMBER
%token KW_GATE_X KW_GATE_Y KW_GATE_Z KW_GATE_H KW_GATE_I KW_GATE_CX
%token KW_MEASURE
%token KW_EQUAL
%token EOL

/* start symbol */
%start prog

/* Section 2: BNF rules and actions */
%%
prog : KW_PROG
       token {

       } KW_IS startDecl startConst startWhere startInit begin KW_END;
token : IDENTIFIER;

/* declaration */
startDecl : KW_VAR decList | ;
decList : decList decl | decl ;
decl : varNameList ':' typeName expectedSemi;
varNameList : varNameList ',' varName | varName;
varName : IDENTIFIER ;
typeName : KW_QUBIT | KW_COMPLEX ;

/* constants */
startConst : KW_CONST decList | ;

/* constraints for constants */
startWhere : KW_WHERE decList | ;

/* initialization */
startInit : KW_INIT initList | ;
initList : initList init | init;
init : varName KW_ASSIGN singleQubit expectedSemi;
singleQubit :   singleQubit '+' singleQubit |
                singleQubit '-' singleQubit |
                expression '.' singleBasis |
                singleBasis;
singleBasis : KW_KET_ZERO | KW_KET_ONE;
expression : NUMBER | IDENTIFIER;

/* statements */
begin : KW_BEGIN stmList | ;
stmList : stmList stm | stm ;
stm : skip | unitaryStm | condStm | loopStm;
/* skip */
skip : KW_SKIP expectedSemi;
/* unitary transformation */
unitaryStm : varNameList KW_ASSIGN gate '[' varNameList ']' expectedSemi;
gate : KW_GATE_X | KW_GATE_Y | KW_GATE_Z | KW_GATE_H | KW_GATE_CX;
/* conditional statement */
condStm : KW_IF condExp KW_THEN stmList KW_ELSE stmList KW_FI expectedSemi;
/* loop statement */
loopStm : KW_WHILE condExp KW_DO stmList KW_OD expectedSemi;

/* measurement */
measure : KW_MEASURE '[' varName ']';
/* conditional expression */
condExp : measure KW_EQUAL NUMBER ;

/* expected semicolon */
expectedSemi : ';'
%%

/* Section 3: C code */
int main(int argc, char **argv)
{
    // reading from a file for testing
    if (!(yyin = fopen("prog.qw", "r"))) {
        printf("Could not open the file prog.qw");
        return 1;
    }
    yyparse();
}

void yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
}