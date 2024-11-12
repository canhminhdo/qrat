/* Section 1: Declarations */
%{
#include <stdio.h>
#include "parser/lexerAux.hpp"
#include "utility/Vector.hpp"
#include "utility/StringTable.hpp"

// global variables
StringTable stringTable;

// for lexer
void yyerror(const char *s);
int yylex (void);
extern FILE *yyin;
extern char *yytext;
%}

%union {
    int codeNr; // the code of the string encoded in the string table
}

/* declare tokens */
%token KW_PROG KW_IS KW_VAR KW_CONST KW_WHERE KW_INIT KW_BEGIN KW_END
%token KW_QUBIT KW_COMPLEX
%token <codeNr> IDENTIFIER
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

/* types for nonterminal sysmbols */
%nterm <codeNr> token

/* start symbol */
%start prog

/* precedence and associativity */
%left '-' '+'
%left '*' '/'
%left UMINUS    /* precedence for unary minus */
%right '^'

/* Section 2: BNF rules and actions */
%%
prog    :   KW_PROG
            token
                {
                    printf("PROG: %s\n", stringTable.name($2));
                }
            KW_IS startDecl startConst startWhere startInit begin KW_END;
token   :   IDENTIFIER;

/* declaration */
startDecl   :   /* empty */
            |   KW_VAR decList
            ;

decList :   decl
        |   decList decl
        ;

decl    :   varNameList ':' typeName expectedSemi;
varNameList :   varName
            |   varNameList ',' varName
            ;
varName :   IDENTIFIER ;
typeName    :   KW_QUBIT
            |   KW_COMPLEX
            ;

/* constants */
startConst  :   /* empty */
            |   KW_CONST decList
            ;

/* constraints for constants */
startWhere  :   /* empty */
            |   KW_WHERE decList
            ;

/* initialization */
startInit   :   /* empty */
            |   KW_INIT initList
            ;
initList    :   init
            |   initList init
            ;
init    :   varName KW_ASSIGN singleQubit expectedSemi;
singleQubit :   singleQubit '+' singleQubit
            |   singleQubit '-' singleQubit
            |   expression '.' singleBasis
            |   singleBasis
            ;
singleBasis :   KW_KET_ZERO
            |   KW_KET_ONE
            ;
expression  :   '(' expression ')'
            |   expression '+' expression
            |   expression '-' expression
            |   expression '*' expression
            |   expression '/' expression
            |   '-' expression  %prec UMINUS NUMBER
            |   IDENTIFIER
            ;

/* statements */
begin   :   /* empty */
        |   KW_BEGIN stmList
        ;
stmList :   stm
        |   stmList stm
        ;
stm :   skip
    |   unitaryStm
    |   condStm
    |   loopStm
    ;
/* skip */
skip    :   KW_SKIP expectedSemi;
/* unitary transformation */
unitaryStm  :   varNameList KW_ASSIGN gate '[' varNameList ']' expectedSemi;
gate    :   KW_GATE_X
        |   KW_GATE_Y
        |   KW_GATE_Z
        |   KW_GATE_H
        |   KW_GATE_CX
        ;
/* conditional statement */
condStm :   KW_IF condExp KW_THEN stmList KW_ELSE stmList KW_FI expectedSemi;
/* loop statement */
loopStm :   KW_WHILE condExp KW_DO stmList KW_OD expectedSemi;

/* measurement */
measure :   KW_MEASURE '[' varName ']';
/* conditional expression */
condExp :   measure KW_EQUAL NUMBER;

/* expected semicolon */
expectedSemi    :   ';';

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
    stringTable.dump();
    // test vector implementation
    Vector<int> v{5};
    v.dump();
}

void yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
}