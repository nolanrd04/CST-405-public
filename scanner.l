%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
%}
%option nounput
%option noinput
%%
"int"           { return INT; }
"float"         { return FLOAT; }
"print"         { return PRINT; }
"return"        { return RETURN;}
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"switch"        { return SWITCH; }
"case"          { return CASE; }
"default"       { return DEFAULT; }
"break"         { return BREAK; }
[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return ID;
}
[0-9]+\.[0-9]+  { yylval.fnum = atof(yytext); return FNUM; }
[0-9]+          { yylval.num = atoi(yytext); return NUM; }
"-"             { return '-'; }
"+"             { return '+'; }
"*"             { return '*'; }
"/"             { return '/'; }
"="             { return '='; }
"=="            { return EQ; }
"!="            { return NEQ; }
">="            { return GTE; }
"<="            { return LTE; }
">"             { return GT; }
"<"             { return LT; }
";"             { return ';'; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"{"             { return '{'; }
"}"             { return '}'; }
","             { return ','; }
":"             { return ':' ; }



[ \t\n]         { }
.               { printf("Lexical Error: Unknown character '%c'\n", *yytext); }
%%
int yywrap() {
    return 1;
}
