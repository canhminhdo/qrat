/* Section 1: Declarations */
%define parse.error detailed
%{
#include <stdio.h>
#include <vector>
#include "parser/lexerAux.hpp"
#include "utility/Vector.hpp"
#include "core/Token.hpp"
#include "core/global.hpp"
#include "core/type.hpp"
#include "ast/KetExpNode.hpp"
#include "ast/QubitExpNode.hpp"
#include "ast/ExpNode.hpp"
#include "ast/OpExpNode.hpp"
#include "ast/ConstExpNode.hpp"
#include "ast/NumExpNode.hpp"
#include "ast/StmNode.hpp"
#include "ast/StmSeqNode.hpp"
#include "ast/SkipStmNode.hpp"
#include "ast/GateExpNode.hpp"
#include "ast/UnitaryStmNode.hpp"

#include "core/Gate.hpp"
#include "utility/macros.hpp"

// for interpreter and programs
SyntaxProg *currentSyntaxProg;

extern DeclMode declFlag;

// for lexer
void yyerror(const char *s);
int yylex (void);
extern FILE *yyin;
extern char *yytext;
extern int yylineno;
%}

%union {
    Token yyToken;
    int codeNr; // the code of the string encoded in the string table
    Type type; // type of variables and constants
    TokenList *yyTokenList;
    Node *node;
    KetExpNode *ketBasis;
    QubitExpNode *qubit;
    ExpNode *expr;
    Gate* gate;
}

/* declare tokens */
%token KW_PROG KW_IS KW_VAR KW_CONST KW_WHERE KW_INIT KW_BEGIN KW_END
%token KW_QUBIT KW_COMPLEX
%token <yyToken> IDENTIFIER
%token KW_SKIP
%token KW_IF KW_THEN KW_ELSE KW_FI
%token KW_WHILE KW_DO KW_OD
%token KW_ASSIGN
%token KW_KET_ZERO
%token KW_KET_ONE
%token INTEGER RATIONAL REAL
%token KW_GATE_X KW_GATE_Y KW_GATE_Z KW_GATE_H KW_GATE_I KW_GATE_CX
%token KW_MEASURE
%token KW_EQUAL
%token EOL

/* types for nonterminal sysmbols */
%nterm <yyToken> token varName
%nterm <type> typeName
%nterm <yyTokenList> varNameList
%nterm <ketBasis> basis
%nterm <qubit> oneQubit
%nterm <expr> expression NUMBER
%nterm <node> stm stmList unitaryStm
%nterm <gate> gate

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
                    interpreter.setCurrentProg($2);
                    currentSyntaxProg = interpreter.getCurrentProg();
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

decl    :   varNameList ':' typeName expectedSemi
                {
                    if (declFlag == VAR_DECL) {
                        currentSyntaxProg->addVarDecl($1, $3);
                    } else if (declFlag == CONST_DECL) {
                        currentSyntaxProg->addConstDecl($1, $3);
                    }
                    delete $1;
                }
            ;
varNameList :   varName
                    {
                        $$ = new TokenList();
                        $$->push_back($1);
                    }
            |   varNameList ',' varName
                    {
                        $1->push_back($3);
                        $$ = $1;
                    }
            ;
varName :   IDENTIFIER ;
typeName    :   KW_QUBIT { $$ = Type::Qubit; }
            |   KW_COMPLEX { $$ = Type::Complex; }
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
init    :   varName KW_ASSIGN expression expectedSemi
                {
                    currentSyntaxProg->addInit($1, $3);
                }
            ;
oneQubit    :   expression '.' basis
                    {
                        $$ = new QubitExpNode($1, $3);
                    }
            |   basis
                    {
                        $$ = new QubitExpNode(nullptr, $1);
                    }
            ;
basis   :   KW_KET_ZERO
                {
                    $$ = new KetExpNode(KetType::KET_ZERO);
                }
        |   KW_KET_ONE
                {
                    $$ = new KetExpNode(KetType::KET_ONE);
                }
        ;
expression  :   '(' expression ')'
                    {
                        $$ = $2;
                    }
            |   expression '+' expression
                    {
                        $$ = new OpExpNode(OpExpType::ADD, $1, $3);
                    }
            |   expression '-' expression
                    {
                        $$ = new OpExpNode(OpExpType::SUB, $1, $3);
                    }
            |   expression '*' expression
                    {
                        $$ = new OpExpNode(OpExpType::MUL, $1, $3);
                    }
            |   expression '/' expression
                    {
                        $$ = new OpExpNode(OpExpType::DIV, $1, $3);
                    }
            |   '-' expression  %prec UMINUS NUMBER
                    {
                        $$ = new OpExpNode(OpExpType::MINUS, nullptr, $2);
                    }
            |   IDENTIFIER
                    {
                        if (!currentSyntaxProg->hasConstSymbol($1)) {
                            printf("Error: %s is not declared as a constant", $1.name());
                            exit(1);
                        }
                        $$ = new ConstExpNode(currentSyntaxProg->lookup($1));
                    }
            |   NUMBER
                    {
                        $$ = $1;
                    }
            |   oneQubit
                    {
                        $$ = $1;
                    }
            ;

NUMBER      :   INTEGER
                    {
                        $$ = new NumExpNode(NumType::INT, yytext);
                    }
            |   RATIONAL
                    {
                        $$ = new NumExpNode(NumType::INT, yytext);
                    }
            |   REAL
                    {
                        $$ = new NumExpNode(NumType::REAL, yytext);
                    }
            ;

/* statements */
begin   :   /* empty */
        |   KW_BEGIN stmList
                {
                    currentSyntaxProg->addStmSeq(dynamic_cast<StmSeqNode *>($2));
                }
        ;
stmList :   stm
                {
                    $$ = new StmSeqNode();
                    dynamic_cast<StmSeqNode *>($$)->addStm(dynamic_cast<StmNode *>$1);
                }
        |   stmList stm
                {
                    dynamic_cast<StmSeqNode *>($1)->addStm(dynamic_cast<StmNode *>$2);
                    $$ = $1;
                }
        ;
stm :   skip
            {
                $$ = new SkipStmNode();
            }
    |   unitaryStm
            {
                $$ = $1;
                printf("unitaryStm\n");
            }
    |   condStm { printf("condStm\n"); }
    |   loopStm { printf("loopStm\n"); }
    ;
/* skip */
skip    :   KW_SKIP expectedSemi;
/* unitary transformation */
unitaryStm  :   varNameList KW_ASSIGN gate '[' varNameList ']' expectedSemi
                    {
                        $$ = new UnitaryStmNode(currentSyntaxProg, $1, $3, $5);
                    }
            ;
gate    :   KW_GATE_X
                {
                    $$ = new Gate(GateType::X);
                }
        |   KW_GATE_Y
                {
                    $$ = new Gate(GateType::Y);
                }
        |   KW_GATE_Z
                {
                    $$ = new Gate(GateType::Z);
                }
        |   KW_GATE_H
                {
                    $$ = new Gate(GateType::H);
                }
        |   KW_GATE_CX
                {
                    $$ = new Gate(GateType::CX);
                }
        ;
/* conditional statement */
condStm :   KW_IF condExp KW_THEN stmList KW_ELSE stmList KW_FI expectedSemi;
/* loop statement */
loopStm :   KW_WHILE condExp KW_DO stmList KW_OD expectedSemi;

/* measurement */
measure :   KW_MEASURE '[' varName ']';
/* conditional expression */
condExp :   measure KW_EQUAL INTEGER;

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
    #if false
    // test vector implementation
    Vector<int> v{5};
    v.dump();
    printf("Code: %d\n", Token::code("TELEPORT"));
    printf("Code: %d\n", Token::code("q1"));
    printf("Code: %d\n", Token::code("q2"));
    printf("Code: %d\n", Token::code("q3"));
    printf("Code: %d\n", Token::code("a"));
    printf("Code: %d\n", Token::code("b"));
    printf("Code: %d\n", Token::code("c"));
    Token::dump();
    #endif
    currentSyntaxProg->dump();
}

void yyerror(const char *s)
{
    fprintf(stderr, "error: %s at line %d\n", s, yylineno);
}