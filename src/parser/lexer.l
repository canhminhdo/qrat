/* Section 1: Declarations and options */
/* not call to yywrap() after reaching the end of input. */
%option noyywrap
/* not generate a default rule for matching any unmatched characters (just copy unmatched input to yyout) */
%option nodefault
/*  provide the line number */
%option yylineno
%{
#include "parser/lexerAux.hpp"
#include "utility/StringTable.hpp"
// bison generated token file
#include "parser.hpp"
void lexerIdMode(void);
extern StringTable stringTable;
%}

/* start conditions */
%s ID_MODE
%s CMD_MODE

/* Section 2: A list of patterns and actions */
%%
"prog"      { return KW_PROG; }
"is"        { return KW_IS; }
"var"       { return KW_VAR; }
"const"     { return KW_CONST; }
"where"     { return KW_WHERE; }
"init"      { return KW_INIT; }
"begin"     { return KW_BEGIN; }
"end"       { return KW_END; }
"while"     { return KW_WHILE; }
"do"        { return KW_DO; }
"od"        { return KW_OD; }
"if"        { return KW_IF; }
"then"      { return KW_THEN; }
"else"      { return KW_ELSE; }
"fi"        { return KW_FI; }
"skip"      { return KW_SKIP; }
"qubit"     { return KW_QUBIT; }
"complex"   { return KW_COMPLEX; }
[+\-*/:;,\[\]().]       { return *yytext; }
"="		    { return KW_EQUAL; }
":="		{ return KW_ASSIGN; }
"|0>"		{ return KW_KET_ZERO; }
"|1>"		{ return KW_KET_ONE; }
"X"		    { return KW_GATE_X; }
"Y"		    { return KW_GATE_Y; }
"Z"		    { return KW_GATE_Z; }
"H"		    { return KW_GATE_H; }
"CX"		{ return KW_GATE_CX; }
"M"         { return KW_MEASURE; }
[_a-zA-Z]([_a-zA-Z0-9])*    {
                // printf("identifier: %s at line %d\n", yytext, yylineno);
                yylval.codeNr = stringTable.encode(yytext);
                return IDENTIFIER;
            }
[0-9]*      { return NUMBER; }
[ \t\f\v\t\r]       /* ignore whitespaces */
\n          /* ignore EOL, otherwise return EOL; */
.           { printf("Mystery character %c at line %d\n", *yytext, yylineno); }

<<EOF>>     {
                /* if no rule for <<EOF>>, flex automatically calls yyterminate() to stop scanning */
                /* handling other input files or just return 0 as end of input */
                return 0;
            }
%%

/* Section 3: C codes */
#include "lexerAux.cpp"