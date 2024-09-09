/* Section 1: Declarations and options */
%{
#include "parser.hpp"
%}

/* Section 2: A list of patterns and actions */
%%

"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"|"     { return ABS; }
"("     { return OP; }
")"     { return CP; }
[0-9]+  { yylval = atoi(yytext); return NUMBER; }
\n      { return EOL; }
"//".*
[ \t]   { /* ignore whitespaces and comments starting with // */ }
.       { printf("Mystery character %c\n", *yytext); }
[_a-zA-Z]([_a-zA-Z0-9])*    { printf("identifier: %s\n", yytext); }

%%

/* Section 3: C codes */