%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}
%option nounput
%option noinput
%%
"int"           { return INT; }
"bool"          { return BOOL; }
"true"          { yylval.num = 1; return TRUE; }
"false"         { yylval.num = 0; return FALSE; }
"float"         { return FLOAT; }
"print"         { return PRINT; }
"println"       { return PRINTLN; }
"return"        { return RETURN;}
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"switch"        { return SWITCH; }
"case"          { return CASE; }
"default"       { return DEFAULT; }
"break"         { return BREAK; }

"when"          { return WHEN; }
"or"            { return WHENOR; }

"while"         { return WHILE; }

["]([^"])*["]    { 
    /* String literal: skip opening quote, copy content, skip closing quote */
    int len = strlen(yytext) - 2;
    char* content = (char*)malloc(len + 1);
    memcpy(content, yytext + 1, len);
    content[len] = '\0';
    yylval.str = content;
    return STRING;
}

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
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }
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
