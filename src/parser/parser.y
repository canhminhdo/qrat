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
#include "core/Search.hpp"
#include "ast/Node.hpp"
#include "ast/KetExpNode.hpp"
#include "ast/QubitExpNode.hpp"
#include "ast/ExpNode.hpp"
#include "ast/OpExpNode.hpp"
#include "ast/ConstExpNode.hpp"
#include "ast/NumExpNode.hpp"
#include "ast/StmNode.hpp"
#include "ast/StmSeq.hpp"
#include "ast/SkipStmNode.hpp"
#include "ast/UnitaryStmNode.hpp"
#include "ast/MeasExpNode.hpp"
#include "ast/CondExpNode.hpp"
#include "ast/CondStmNode.hpp"
#include "ast/WhileStmNode.hpp"
#include "ast/BoolExpNode.hpp"
#include "ast/InitExpNode.hpp"
#include "ast/PropExpNode.hpp"
#include "dd/DDOperation.hpp"

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

#define EXP_NODE(node) dynamic_cast<ExpNode *>(node)
#define STM_NODE(node) dynamic_cast<StmNode *>(node)
#define NUM_EXP_NODE(node) dynamic_cast<NumExpNode *>(node)
%}

%union {
    Token yyToken;
    int codeNr; // the code of the string encoded in the string table
    Type type; // type of variables and constants
    GateInfo gateInfo; // type of quantum operations
    TokenList *yyTokenList;
    Node *node;
    ExpNode *expr;
    StmNode *stm;
    StmSeq *stmSeq;
    Search::Type searchType;
}

/* declare tokens */
/* for programs */
%token KW_PROG KW_IS KW_VAR KW_CONST KW_WHERE KW_INIT KW_BEGIN KW_END
%token KW_QUBIT KW_COMPLEX
%token <yyToken> IDENTIFIER
%token KW_SKIP
%token KW_IF KW_THEN KW_ELSE KW_FI
%token KW_WHILE KW_DO KW_OD
%token KW_ASSIGN
%token KW_KET_ZERO
%token KW_KET_ONE
%token KW_KET_RANDOM
%token INTEGER RATIONAL REAL
%token KW_SINGLE_TARGET_OP KW_SINGLE_TARGET_COP KW_SINGLE_TARGET_MCOP
%token KW_MEASURE
%token KW_EQUAL
/* for commands */
%token KW_SEARCH KW_IN KW_WITH KW_SUCH KW_THAT
%token KW_ARROW_ONE KW_ARROW_STAR KW_ARROW_PLUS KW_ARROW_EXCLAMATION
%token KW_TRUE KW_FALSE KW_AND KW_OR KW_NOT KW_PROP
%token EOL

/* types for nonterminal sysmbols */
%nterm <yyToken> token varName
%nterm <type> typeName
%nterm <gateInfo> operation
%nterm <yyTokenList> varNameList
/*
%nterm <expr> expression number oneQubit basis measure condExp
%nterm <stm> stm stmList unitaryStm condStm loopStm
*/
%nterm <expr> expression number oneQubit basis measure condExp property basisProp
%nterm <stm> stm unitaryStm condStm loopStm
%nterm <stmSeq> stmList
%nterm <searchType> arrow
/* start symbol */
%start top

/* precedence and associativity */
%left '-' '+'
%left '*' '/'
%left UMINUS    /* precedence for unary minus */
%right '^'
%nonassoc '.'  /* %precedence '.' */
%left KW_OR
%left KW_AND
%left KW_NOT

/* Section 2: BNF rules and actions */
%%
top :   /* empty */
    |   top item
    ;

item    :   prog
        |   command
        ;

prog    :   KW_PROG
            token
                {
                    interpreter.setCurrentProg($2);
                    currentSyntaxProg = interpreter.getCurrentProg();
                }
            KW_IS startDecl startConst startWhere startInit begin KW_END
        ;
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
                        $$ = currentSyntaxProg->makeNode(new QubitExpNode($1, $3));
                    }
            |   basis
                    {
                        $$ = $1; // new QubitExpNode(nullptr, $1);
                    }
            ;
basis   :   KW_KET_ZERO
                {
                    $$ = currentSyntaxProg->makeNode(new KetExpNode(KetType::KET_ZERO));

                }
        |   KW_KET_ONE
                {
                    $$ = currentSyntaxProg->makeNode(new KetExpNode(KetType::KET_ONE));
                }
        |   KW_KET_RANDOM
                {
                    $$ = currentSyntaxProg->makeNode(new KetExpNode(KetType::KET_RANDOM));
                }
        ;
expression  :   '(' expression ')'
                    {
                        $$ = $2;
                    }
            |   expression '+' expression
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::ADD, $1, $3));
                    }
            |   expression '-' expression
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::SUB, $1, $3));
                    }
            |   expression '*' expression
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::MUL, $1, $3));
                    }
            |   expression '/' expression
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::DIV, $1, $3));
                    }
            |   '-' expression  %prec UMINUS
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::MINUS, nullptr, $2));
                    }
            |   IDENTIFIER
                    {
                        if (!currentSyntaxProg->hasConstSymbol($1)) {
                            printf("Error: %s is not declared as a constant", $1.name());
                            exit(1);
                        }
                        $$ = currentSyntaxProg->makeNode(new ConstExpNode(currentSyntaxProg->lookup($1)));
                    }
            |   number
                    {
                        $$ = $1;
                    }
            |   oneQubit
                    {
                        $$ = $1;
                    }
            ;

number      :   INTEGER
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
                    currentSyntaxProg->addStmSeq($2);
                }
        ;
stmList :   stm
                {
                    $$ = new StmSeq();
                    $$->addStm($1);
                }
        |   stmList stm
                {
                    $1->addStm($2);
                    $$ = $1;
                }
        ;
stm :   skip
            {
                $$ = new SkipStmNode();
            }
    |   unitaryStm
    |   condStm
    |   loopStm
    ;
/* skip */
skip    :   KW_SKIP expectedSemi;
/* unitary transformation */

unitaryStm  :   varNameList KW_ASSIGN operation '[' varNameList ']' expectedSemi
                    {
                        $$ = DDOperation::makeOperation(currentSyntaxProg, $1, $3, $5);
                    }
            ;
operation   :   KW_SINGLE_TARGET_OP
            |   KW_SINGLE_TARGET_COP
            |   KW_SINGLE_TARGET_MCOP
            ;
/* conditional statement */
condStm :   KW_IF condExp KW_THEN stmList KW_ELSE stmList KW_FI expectedSemi
                {
                    $$ = new CondStmNode($2, $4, $6);
                }
        ;
/* loop statement */
loopStm :   KW_WHILE condExp KW_DO stmList KW_OD expectedSemi
                {
                    $$ = new WhileStmNode($2, $4);
                }
        ;

/* measurement */
measure :   KW_MEASURE '[' varName ']'
                {
                    if (!currentSyntaxProg->hasVarSymbol($3)) {
                        printf("Error: variable %s is undefined", $3.name());
                        exit(1);
                    }
                    $$ = new MeasExpNode(currentSyntaxProg->lookup($3));
                }
        ;
/* conditional expression */
condExp :   measure KW_EQUAL number
                {
                    $$ = new CondExpNode($1, RelOpType::EQ, $3);
                    if (!NUM_EXP_NODE($3)->isZeroOrOne()) {
                        yyerror("the measurement result must be 0 or 1");
                        exit(SEMANTIC_ERROR);
                    }
                }
        |   number KW_EQUAL measure
                {
                    $$ = new CondExpNode($3, RelOpType::EQ, $1);
                    if (!NUM_EXP_NODE($1)->isZeroOrOne()) {
                        yyerror("the measurement result must be 0 or 1");
                        exit(SEMANTIC_ERROR);
                    }
                }
        ;

/* expected semicolon */
expectedSemi    :   ';';

command :   KW_SEARCH KW_IN
            token
                {
                    if (currentSyntaxProg == nullptr || currentSyntaxProg->getName() != $3.code()) {
                        yyerror(("Search in an undefined program: " + std::string($3.name())).c_str());
                        exit(SEMANTIC_ERROR);
                    }
                }
            KW_WITH
            arrow
            KW_SUCH KW_THAT
            property
            expectedSemi
                {
                    interpreter.initializeSearch($3.code(), $9, $6, UNBOUNDED, UNBOUNDED);
                    interpreter.execute();
                }
        |   KW_SEARCH '[' number ']' KW_IN
            token
                {
                    if (currentSyntaxProg == nullptr || currentSyntaxProg->getName() != $6.code()) {
                        yyerror(("Search in an undefined program: " + std::string($6.name())).c_str());
                        exit(SEMANTIC_ERROR);
                    }
                }
            KW_WITH
            arrow
            KW_SUCH KW_THAT
            property
            expectedSemi
                {
                    if (!NUM_EXP_NODE($3)->isInt()) {
                        yyerror("The number of solutions must be an integer");
                        exit(SEMANTIC_ERROR);
                    }
                    interpreter.initializeSearch($6.code(), $12, $9, NUM_EXP_NODE($3)->getIntVal(), UNBOUNDED);
                    delete $3;
                    interpreter.execute();
                }
        |   KW_SEARCH '[' ',' number ']' KW_IN
            token
                {
                    if (currentSyntaxProg == nullptr || currentSyntaxProg->getName() != $7.code()) {
                        yyerror(("Search in an undefined program: " + std::string($7.name())).c_str());
                        exit(SEMANTIC_ERROR);
                    }
                }
            KW_WITH
            arrow
            KW_SUCH KW_THAT
            property
            expectedSemi
                {
                    if (!NUM_EXP_NODE($4)->isInt()) {
                        yyerror("The bounded depth must be an integer");
                        exit(SEMANTIC_ERROR);
                    }
                    interpreter.initializeSearch($7.code(), $13, $10, UNBOUNDED, NUM_EXP_NODE($4)->getIntVal());
                    delete $4;
                    interpreter.execute();
                }
        |   KW_SEARCH '[' number ',' number ']' KW_IN
            token
                {
                    if (currentSyntaxProg == nullptr || currentSyntaxProg->getName() != $8.code()) {
                        yyerror(("Search in an undefined program: " + std::string($8.name())).c_str());
                        exit(SEMANTIC_ERROR);
                    }
                }
            KW_WITH
            arrow
            KW_SUCH KW_THAT
            property
            expectedSemi
                {
                    if (!NUM_EXP_NODE($3)->isInt()) {
                        yyerror("The number of solutions must be an integer");
                        exit(SEMANTIC_ERROR);
                    }
                    if (!NUM_EXP_NODE($5)->isInt()) {
                        yyerror("The bounded depth must be an integer");
                        exit(SEMANTIC_ERROR);
                    }
                    interpreter.initializeSearch($8.code(), $14, $11, NUM_EXP_NODE($3)->getIntVal(), NUM_EXP_NODE($5)->getIntVal());
                    delete $3;
                    delete $5;
                    interpreter.execute();
                }
        ;
arrow   :   KW_ARROW_ONE
                {
                    $$ = Search::Type::ARROW_ONE;
                }
        |   KW_ARROW_STAR
                {
                    $$ = Search::Type::ARROW_STAR;
                }
        |   KW_ARROW_PLUS
                {
                    $$ = Search::Type::ARROW_PLUS;
                }
        |   KW_ARROW_EXCLAMATION
                {
                    $$ = Search::Type::ARROW_EXCLAMATION;
                }
        ;

property    :   '(' property ')'
                    {
                        $$ = $2;
                    }
            |   KW_TRUE
                    {
                        $$ = currentSyntaxProg->makeNode(new BoolExpNode(BoolType::TRUE));
                    }
            |   KW_FALSE
                    {
                        $$ = currentSyntaxProg->makeNode(new BoolExpNode(BoolType::FALSE));
                    }
            |   KW_PROP '(' varName ',' basisProp ')'
                    {
                        if (!currentSyntaxProg->hasVarSymbol($3)) {
                            printf("Error: variable %s is undefined", $3.name());
                            exit(1);
                        }
                        $$ = currentSyntaxProg->makeNode(new PropExpNode(currentSyntaxProg->lookup($3), $5));
                    }
            |   property KW_AND property
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::AND, $1, $3));
                    }
            |   property KW_OR property
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::OR, $1, $3));
                    }
            |   KW_NOT property %prec KW_NOT
                    {
                        $$ = currentSyntaxProg->makeNode(new OpExpNode(OpExpType::NOT, nullptr, $2));
                    }
            ;

basisProp   :   basis
            |   KW_INIT '[' varName ']'
                    {
                        if (!currentSyntaxProg->hasVarSymbol($3)) {
                            printf("Error: variable %s is undefined", $3.name());
                            exit(1);
                        }
                        $$ = currentSyntaxProg->makeNode(new InitExpNode(currentSyntaxProg->lookup($3)));
                    }
            ;
%%

/* Section 3: C code */
int main(int argc, char **argv)
{
    // reading from a file for testing
    const char* fileName = "prog.qw";
    if (!(yyin = fopen(fileName, "r"))) {
        fprintf(stderr, "Error opening file '%s': %s\n", fileName, strerror(errno));
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
    currentSyntaxProg->dump();
    interpreter.execute();
    #endif
}

void yyerror(const char *s)
{
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}