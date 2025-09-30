%{
/* SYNTAX ANALYZER (PARSER)
 * This is the second phase of compilation - checking grammar rules
 * Bison generates a parser that builds an Abstract Syntax Tree (AST)
 * The parser uses tokens from the scanner to verify syntax is correct
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtab.h"

/* External declarations for lexer interface */
extern int yylex();      /* Get next token from scanner */
extern int yyparse();    /* Parse the entire input */
extern FILE* yyin;       /* Input file handle */

void yyerror(const char* s);  /* Error handling function */
ASTNode* root = NULL;          /* Root of the Abstract Syntax Tree */
%}

/* SEMANTIC VALUES UNION
 * Defines possible types for tokens and grammar symbols
 * This allows different grammar rules to return different data types
 */
%union {
    int num;                /* For integer literals */
    char* str;              /* For identifiers */
    struct ASTNode* node;   /* For AST nodes */
}

/* TOKEN DECLARATIONS with their semantic value types */
%token <num> NUM        /* Number token carries an integer value */
%token <str> ID         /* Identifier token carries a string */
%token INT PRINT        /* Keywords have no semantic value */

/* NON-TERMINAL TYPES - Define what type each grammar rule returns */
%type <node> program stmt_list stmt decl assign declAssign expr print_stmt arrayExpr arrayExpr2D

/* OPERATOR PRECEDENCE AND ASSOCIATIVITY */
%left '+'  /* Addition is left-associative: a+b+c = (a+b)+c */
%left '-' /* Subtraction is left-associative: a-b-c = (a-b)-c */

%%

/* GRAMMAR RULES - Define the structure of our language */

/* PROGRAM RULE - Entry point of our grammar */
program:
    stmt_list { 
        /* Action: Save the statement list as our AST root */
        root = $1;  /* $1 refers to the first symbol (stmt_list) */
    }
    ;

/* STATEMENT LIST - Handles multiple statements */
stmt_list:
    stmt { 
        /* Base case: single statement */
        $$ = $1;  /* Pass the statement up as-is */
    }
    | stmt_list stmt { 
        /* Recursive case: list followed by another statement */
        $$ = createStmtList($1, $2);  /* Build linked list of statements */
    }
    ;

/* STATEMENT TYPES - The three kinds of statements we support */
stmt:
    decl        /* Variable declaration */
    | assign    /* Assignment statement */
    | declAssign /* Declaration with assignment */
    | print_stmt /* Print statement */
    ;

/* DECLARATION RULE - "int x;" or "int x = expr;*/
decl:
    INT ID ';' { 

        /* Create declaration node and free the identifier string */
        $$ = createDecl($2);  /* $2 is the ID token's string value; stored in the symbol table; returns into $$, which is a pointer to a sub tree. */
        free($2);             /* Free the string copy from scanner */
    }

    /* ##### ONE DIMENSIONAL ARRAYS ##### */
    /* array element of length NUM */
    | INT ID '[' NUM ']' ';'
    {
        $$ = createArrayDeclOfLength("int", $2, $4);/* done */
        free($2);
    }

    /* ##### TWO DIMENSIONAL ARRAYS ##### */
    /* 2D array element of length NUM x NUM */
    | INT ID '[' NUM ']' '[' NUM ']' ';'
    {
        $$ = create2DArrayDeclOfLength("int", $2, $4, $7); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free($2);
    }
    ;

/* ASSIGNMENT RULE - "x = expr;" */
assign:
    /* array element assignment: ID '[' expr ']' '=' expr ';' */
    ID '[' expr ']' '=' expr ';' {
        $$ = createArrayElemAssign($1, $3, $6); 
        free($1);
    }
    /* plain variable assignment */
    | ID '=' expr ';' { 
        $$ = createAssign($1, $3);  
        free($1);
    }

    /* 2D arrays */
    /*assign all rows and values to a 2d array */
    | ID '='  '{' '}' ';'
    {
        /* not implemented, might not.*/
        free($1);
    }
    /* assign a whole array (row)(y) to an X index. For example: list2D [0][] = {1, 2, 3}; list2D [1][] = {4, 5, 6}; would be [1, 2, 3],[4, 5, 6] */
    | ID '[' expr ']' '[' ']' '=' arrayExpr ';'
    {
        /* not implemented, might not. */
        /* $$ createArray2DRowAssign($1, $3, $8); */
        free ($1);
    }
    /* assign a single element at a time */
    | ID '[' expr ']' '[' expr ']' '=' expr ';'
    {
        $$ = createArray2DElemAssign($1, $3, $6, $9); /* scanner.l -> parser.y -> ast.h (done) -> ast.c (done)-> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free($1);
    }
    ;

declAssign:

    /* ##### DONE ##### */
    INT ID { 
    } '=' expr ';'
    {
        $$ = createDeclAssign("int", $2, $5); /* done */
        free($2);
    }

    /*array of length num with assignment */
    | INT ID '[' NUM ']' '=' '{' arrayExpr '}' ';'
    {
        $$ = createArrayAssign("int", $2, $4, $8);/* done */
        free($2);
    }
    /* array element of unknown length with assignment */
    | INT ID '[' ']' '=' '{' arrayExpr '}' ';'
    {
        $$ = createArrayDeclAssign("int", $2, 0, $7);/* done */
        free($2);
    }

    /* 2D array of unknown length with assignment */
    | INT ID '[' ']' '[' ']' '=' '{' arrayExpr2D '}' ';' 
    {
        /* $$ = create2DArrayDeclAssign("int", $2, 0, 0, $9); */ /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free($2);
    }
    ;

/* EXPRESSION RULES - Build expression trees */
expr:
    NUM { 
        /* Literal number */
        $$ = createNum($1);  /* Create leaf node with number value */
    }
    | ID { 
        /* Variable reference */
        $$ = createVar($1);  /* Create leaf node with variable name */
        free($1);            /* Free the identifier string */
    }
    | ID '[' expr ']' { /* allows to access whatever value is at index 'expr' */
        $$ = createArrayAccess($1, $3);
        free($1);
    }
    | ID '[' expr ']' '[' expr ']' /* allows to access whatever value is at index 'expr', 'expr' */
    {
        $$ = createArray2DAccess($1, $3, $6); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free($1);
    }
    | expr '+' expr { 
        /* Addition operation - builds binary tree */
        $$ = createBinOp('+', $1, $3);  /* Left child, op, right child */
    } | expr '-' expr { 
        /* Subtraction operation - builds binary tree */
        $$ = createBinOp('-', $1, $3);  /* Left child, op, right child */
    }
    ;

/* PRINT STATEMENT - "print(expr);" */
print_stmt:
    PRINT '(' expr ')' ';' { 
        /* Create print node with expression to print */
        $$ = createPrint($3);  /* $3 is the expression inside parens */
    }
    ;

/* array expression list */
arrayExpr:
    expr {
        $$ = createExprList($1, NULL); /* done */
    }
    | arrayExpr ',' expr {
        $$ = createExprList($3, $1); /* done */
    }
    ;

/* 2D array expression list */
arrayExpr2D:
    '{' arrayExpr '}' {
        /* $$ = create2DExprList($2, NULL); */ /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
    }
    | arrayExpr2D ',' '{' arrayExpr '}' {
        /* $$ = create2DExprList($4, $1); */ /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
    }
    ;

%%

/* ERROR HANDLING - Called by Bison when syntax error detected */
void yyerror(const char* s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}