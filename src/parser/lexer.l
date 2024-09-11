/* Section 1: Declarations and options */
%option noyywrap nodefault yylineno
%{
#include "parser/lexerAux.hpp"
// bison generated token file
#include "parser.hpp"
void lexerIdMode(void);
%}

/* start conditions */
%s ID_MODE
%s CMD_MODE

/* Section 2: A list of patterns and actions */
%%
"prog"      { printf("%s at line %d\n", yytext, yylineno); return KW_PROG; }
"is"        { printf("%s at line %d\n", yytext, yylineno); return KW_IS; }
"var"       { printf("%s at line %d\n", yytext, yylineno); return KW_VAR; }
"const"     { printf("%s at line %d\n", yytext, yylineno); return KW_CONST; }
"where"     { printf("%s at line %d\n", yytext, yylineno); return KW_WHERE; }
"init"      { printf("%s at line %d\n", yytext, yylineno); return KW_INIT; }
"begin"     { printf("%s at line %d\n", yytext, yylineno); return KW_BEGIN; }
"end"       { printf("%s at line %d\n", yytext, yylineno); return KW_END; }
"while"     { printf("%s at line %d\n", yytext, yylineno); return KW_WHILE; }
"do"        { printf("%s at line %d\n", yytext, yylineno); return KW_DO; }
"od"        { printf("%s at line %d\n", yytext, yylineno); return KW_OD; }
"if"        { printf("%s at line %d\n", yytext, yylineno); return KW_IF; }
"then"      { printf("%s at line %d\n", yytext, yylineno); return KW_THEN; }
"else"      { printf("%s at line %d\n", yytext, yylineno); return KW_ELSE; }
"fi"        { printf("%s at line %d\n", yytext, yylineno); return KW_FI; }
"skip"      { printf("%s at line %d\n", yytext, yylineno); return KW_SKIP; }
"qubit"     { printf("%s at line %d\n", yytext, yylineno); return KW_QUBIT; }
"complex"   { printf("%s at line %d\n", yytext, yylineno); return KW_COMPLEX; }
"+" |
"-" |
"*" |
"/" |
":" |
";" |
"," |
"[" |
"]" |
"(" |
")" |
"."         { return yytext[0]; }
"="		    { printf("%s at line %d\n", yytext, yylineno); return KW_EQUAL; }
":="		{ printf("%s at line %d\n", yytext, yylineno); return KW_ASSIGN; }
"|0>"		{ printf("%s at line %d\n", yytext, yylineno); return KW_KET_ZERO; }
"|1>"		{ printf("%s at line %d\n", yytext, yylineno); return KW_KET_ONE; }
"X"		    { printf("%s at line %d\n", yytext, yylineno); return KW_GATE_X; }
"Y"		    { printf("%s at line %d\n", yytext, yylineno); return KW_GATE_Y; }
"Z"		    { printf("%s at line %d\n", yytext, yylineno); return KW_GATE_Z; }
"H"		    { printf("%s at line %d\n", yytext, yylineno); return KW_GATE_H; }
"CX"		{ printf("%s at line %d\n", yytext, yylineno); return KW_GATE_CX; }
"M"         { printf("%s at line %d\n", yytext, yylineno); return KW_MEASURE; }
[_a-zA-Z]([_a-zA-Z0-9])*    { printf("identifier: %s at line %d\n", yytext, yylineno); return IDENTIFIER; }
[0-9]*      { printf("number: %s at line %d\n", yytext, yylineno); return NUMBER; }
[ \t]       /* ignore whitespaces */
\n          /* ignore EOF, otherwise return EOL; */
.           { printf("Mystery character %c at line %d\n", *yytext, yylineno); }
%%

/* Section 3: C codes */
#include "lexerAux.cpp"