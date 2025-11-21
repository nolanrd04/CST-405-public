%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

/* Debug flag for scanner */
extern int debug_parser;
%}
%option nounput
%option noinput
%option yylineno
%%
"int"           { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'int'\n", yylineno); return INT; }
"bool"          { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'bool'\n", yylineno); return BOOL; }
"true"          { yylval.num = 1; if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'true'\n", yylineno); return TRUE; }
"false"         { yylval.num = 0; if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'false'\n", yylineno); return FALSE; }
"float"         { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'float'\n", yylineno); return FLOAT; }
"print"         { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'print'\n", yylineno); return PRINT; }
"println"       { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'println'\n", yylineno); return PRINTLN; }
"return"        { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'return'\n", yylineno); return RETURN;}
"void"          { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'void'\n", yylineno); return VOID; }
"if"            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'if'\n", yylineno); return IF; }
"else"          { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'else'\n", yylineno); return ELSE; }
"switch"        { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'switch'\n", yylineno); return SWITCH; }
"case"          { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'case'\n", yylineno); return CASE; }
"default"       { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'default'\n", yylineno); return DEFAULT; }
"break"         { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'break'\n", yylineno); return BREAK; }

"when"          { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'when'\n", yylineno); return WHEN; }
"or"            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'or'\n", yylineno); return WHENOR; }

"while"         { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN 'while'\n", yylineno); return WHILE; }

["]([^"])*["]    { 
    /* String literal: skip opening quote, copy content, skip closing quote */
    int len = strlen(yytext) - 2;
    char* content = (char*)malloc(len + 1);
    memcpy(content, yytext + 1, len);
    content[len] = '\0';
    yylval.str = content;
    if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN STRING '%s'\n", yylineno, content);
    return STRING;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN ID '%s'\n", yylineno, yytext);
    return ID;
}
-?[0-9]+\.[0-9]+  { yylval.fnum = atof(yytext); if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN FNUM %s\n", yylineno, yytext); return FNUM; }
-?[0-9]+          { yylval.num = atoi(yytext); if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN NUM %s\n", yylineno, yytext); return NUM; }
"-"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '-'\n", yylineno); return '-'; }
"+"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '+'\n", yylineno); return '+'; }
"*"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '*'\n", yylineno); return '*'; }
"/"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '/'\n", yylineno); return '/'; }
"="             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '='\n", yylineno); return '='; }
"=="            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '=='\n", yylineno); return EQ; }
"!="            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '!='\n", yylineno); return NEQ; }
">="            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '>='\n", yylineno); return GTE; }
"<="            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '<='\n", yylineno); return LTE; }
">"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '>'\n", yylineno); return GT; }
"<"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '<'\n", yylineno); return LT; }
"&&"            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '&&'\n", yylineno); return AND; }
"||"            { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '||'\n", yylineno); return OR; }
"!"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '!'\n", yylineno); return NOT; }
";"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN ';'\n", yylineno); return ';'; }
"("             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '('\n", yylineno); return '('; }
")"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN ')'\n", yylineno); return ')'; }
"["             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '['\n", yylineno); return '['; }
"]"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN ']'\n", yylineno); return ']'; }
"{"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '{'\n", yylineno); return '{'; }
"}"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN '}'\n", yylineno); return '}'; }
","             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN ','\n", yylineno); return ','; }
":"             { if(debug_parser) fprintf(stderr, "[Lexer] Line %d: TOKEN ':'\n", yylineno); return ':' ; }

"/*"([^*]|\*+[^*/])*\*+"/"    { /* Multi-line comment - ignore */ }
"//".*                        { /* Single-line comment - ignore */ }

[ \t\n\r]       { }
.               { printf("Lexical Error: Unknown character '%c'\n", *yytext); }
%%
int yywrap() {
    return 1;
}
