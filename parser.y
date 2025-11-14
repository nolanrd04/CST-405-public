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

/* ============================================================================
   SEMANTIC VALUES UNION
   Defines possible types for tokens and grammar symbols
   ============================================================================ */
%union {
    int num;                /* For integer literals */
    float fnum;             /* For float literals */
    char* str;              /* For identifiers */
    struct ASTNode* node;   /* For AST nodes */
}

/* ============================================================================
   TOKEN DECLARATIONS with their semantic value types
   ============================================================================ */

/* Literals */
%token <num> NUM            /* Integer literal */
%token <fnum> FNUM          /* Float literal */
%token <num> TRUE FALSE     /* Boolean literals */

/* Identifiers */
%token <str> ID             /* Identifier/variable name */

/* ===== NEW: STRING LITERAL SUPPORT ===== */
%token <str> STRING         /* String literal */
/* ===== END: STRING LITERAL SUPPORT ===== */

/* Data Types */
%token INT FLOAT VOID BOOL

/* Keywords & Control Flow */
%token IF ELSE              /* Conditional statements */
%token SWITCH CASE DEFAULT BREAK  /* Switch statements */
%token RETURN               /* Function return */

/* ===== NEW: WHEN LOOP FEATURE ===== */
%token WHEN WHENOR          /* When loop and or branches */
/* ===== END: WHEN LOOP FEATURE ===== */

%token WHILE                /* While loop */

/* I/O */
%token PRINT PRINTLN            /* Print and println statements */

/* Operators */
%token EQ NEQ LT GT LTE GTE   /* Comparison operators */
%token AND OR NOT            /* Logical operators */

/* ============================================================================
   NON-TERMINAL TYPES - Define return types for grammar rules
   ============================================================================ */
%type <node> program stmt_list
%type <node> stmt decl declAssign assign
%type <node> if_stmt switch_stmt case_list case_Stmt
%type <node> func_decl param_list param block
%type <node> print_stmt return_stmt func_call arg_list
%type <node> expr arrayExpr

/* ===== NEW: WHEN LOOP FEATURE ===== */
%type <node> when_stmt when_or_list when_or_branch break_when_stmt
/* ===== END: WHEN LOOP FEATURE ===== */
%type <node> while_stmt

/* ============================================================================
   OPERATOR PRECEDENCE AND ASSOCIATIVITY (lowest to highest)
   ============================================================================ */
%left OR                    /* Logical OR - lowest precedence */
%left AND                   /* Logical AND */
%left EQ NEQ                /* Equality operators */
%left LT GT LTE GTE         /* Comparison operators */
%left '+' '-'               /* Addition, Subtraction */
%left '*' '/'               /* Multiplication, Division */
%right NOT                  /* Logical NOT - highest precedence */

%%

/* ============================================================================
   PROGRAM STRUCTURE
   ============================================================================ */

program:
    stmt_list { 
        root = $1;
    }
    ;

/* ============================================================================
   STATEMENT LISTS (top-level and nested)
   ============================================================================ */

stmt_list:
    stmt { 
        $$ = $1;
    }
    | func_decl { 
        $$ = $1;
    }
    | stmt_list stmt { 
        $$ = createStmtList($1, $2);
    }
    | stmt_list func_decl {
        $$ = createStmtList($1, $2);
    }
    ;

/* ============================================================================
   STATEMENTS (all types grouped by purpose)
   ============================================================================ */

stmt:
    /* Variable management */
    decl
    | declAssign
    | assign
    
    /* I/O */
    | print_stmt
    
    /* Control flow */
    | if_stmt
    | switch_stmt
    | return_stmt
    | func_call ';'
    | BREAK ';' { $$ = createBreak(); }
    /* ===== NEW: WHEN LOOP FEATURE ===== */
    | when_stmt
    | break_when_stmt
    /* ===== END: WHEN LOOP FEATURE ===== */
    | while_stmt
    
    /* Grouping */
    | block
    ;

/* ============================================================================
   VARIABLE DECLARATIONS
   ============================================================================ */

decl:
    INT ID ';' { 
        $$ = createDecl("int", $2);
        free($2);
    }
    | FLOAT ID ';' {
        $$ = createDecl("float", $2);
        free($2);
    }
    | BOOL ID ';' {
        $$ = createDecl("bool", $2);
        free($2);
    }
    /* 1D integer arrays */
    | INT ID '[' NUM ']' ';' {
        $$ = createArrayDeclOfLength("int", $2, $4);
        addArrayVar($2, $4, "int");
        free($2);
    }
    /* 2D integer arrays */
    | INT ID '[' NUM ']' '[' NUM ']' ';' {
        addArray2DVar($2, $4, $7, "int");
        $$ = create2DArrayDeclOfLength("int", $2, $4, $7);
        free($2);
    }
    /* 1D float arrays */
    | FLOAT ID '[' NUM ']' ';' {
        $$ = createArrayDeclOfLength("float", $2, $4);
        addArrayVar($2, $4, "float");
        free($2);
    }
    /* 2D float arrays */
    | FLOAT ID '[' NUM ']' '[' NUM ']' ';' {
        addArray2DVar($2, $4, $7, "float");
        $$ = create2DArrayDeclOfLength("float", $2, $4, $7);
        free($2);
    }
    ;

/* Declaration with initialization */
declAssign:
    INT ID '=' expr ';' {
        $$ = createDeclAssign("int", $2, $4);
        free($2);
    }
    | FLOAT ID '=' expr ';' { 
        $$ = createDeclAssign("float", $2, $4);
        free($2);
    }
    | BOOL ID '=' expr ';' {
        $$ = createDeclAssign("bool", $2, $4);
        free($2);
    }
    /* 1D array initialization with explicit size */
    | INT ID '[' NUM ']' '=' '{' arrayExpr '}' ';' {
        $$ = createArrayAssign("int", $2, $4, $8);
        free($2);
    }
    /* 1D array initialization with inferred size */
    | INT ID '[' ']' '=' '{' arrayExpr '}' ';' {
        $$ = createArrayDeclAssign("int", $2, 0, $7);
        free($2);
    }
    /* 1D float array initialization with explicit size */
    | FLOAT ID '[' NUM ']' '=' '{' arrayExpr '}' ';' {
        $$ = createArrayAssign("float", $2, $4, $8);
        free($2);
    }
    /* 1D float array initialization with inferred size */
    | FLOAT ID '[' ']' '=' '{' arrayExpr '}' ';' {
        $$ = createArrayDeclAssign("float", $2, 0, $7);
        free($2);
    }
    ;

/* Assignment statements */
assign:
    /* Simple variable assignment */
    ID '=' expr ';' { 
        $$ = createAssign($1, $3);
        free($1);
    }
    /* 1D array element assignment */
    | ID '[' expr ']' '=' expr ';' {
        $$ = createArrayElemAssign($1, $3, $6);
        free($1);
    }
    /* 2D array element assignment */
    | ID '[' expr ']' '[' expr ']' '=' expr ';' {
        $$ = createArray2DElemAssign($1, $3, $6, $9);
        free($1);
    }
    ;

/* ============================================================================
   FUNCTION DECLARATIONS AND CALLS
   ============================================================================ */

func_decl:
    INT ID '(' param_list ')' block {
        $$ = createFuncDecl("int", $2, $4, $6);
        free($2);
    }
    | INT ID '(' ')' block {
        $$ = createFuncDecl("int", $2, NULL, $5);
        free($2);
    }
    | FLOAT ID '(' param_list ')' block {
        $$ = createFuncDecl("float", $2, $4, $6);
        free($2);
    }
    | FLOAT ID '(' ')' block {
        $$ = createFuncDecl("float", $2, NULL, $5);
        free($2);
    }
    | VOID ID '(' ')' block {
        $$ = createFuncDecl("void", $2, NULL, $5);
        free($2);
    }
    ;

param_list:
    param {
        $$ = $1;
    }
    | param_list ',' param {
        $$ = createParamList($1, $3);
    }
    ;

param:
    INT ID {
        $$ = createParam("int", $2);
        free($2);
    }
    | FLOAT ID {
        $$ = createParam("float", $2);
        free($2);
    }
    ;

func_call:
    ID '(' arg_list ')' {
        $$ = createFuncCall($1, $3);
        free($1);
    }
    | ID '(' ')' {
        $$ = createFuncCall($1, NULL);
        free($1);
    }
    ;

arg_list:
    expr {
        $$ = createArgList($1, NULL);
    }
    | arg_list ',' expr {
        $$ = createArgList($3, $1);
    }
    ;

/* ============================================================================
   CONTROL FLOW STATEMENTS
   ============================================================================ */

if_stmt:
    IF '(' expr ')' stmt {
        $$ = createIfNode($3, $5, NULL);
    }
    | IF '(' expr ')' stmt ELSE stmt {
        $$ = createIfNode($3, $5, $7);
    }
    ;

switch_stmt:
    SWITCH '(' expr ')' '{' case_list '}' {
        $$ = createSwitch($3, $6);
    }
    ;

case_list:
    case_Stmt {
        $$ = $1;
    }
    | case_list case_Stmt {
        $$ = createCaseList($1, $2);
    }
    ;

case_Stmt:
    CASE NUM ':' stmt_list {
        $$ = createCase($2, $4);
    }
    | DEFAULT ':' stmt_list {
        $$ = createDefaultCase($3);
    }
    ;

/* ============================================================================
   WHEN LOOP STATEMENTS (NEW FEATURE)
   ============================================================================ */

/* While loop: loop while condition is true */
while_stmt:
    WHILE '(' expr ')' block {
        $$ = createWhileStmt($3, $5);
    }
    ;

/* ===== NEW: WHEN LOOP FEATURE ===== */
/* Main when statement with primary condition and optional branches */
when_stmt:
    WHEN '(' expr ')' block when_or_list {
        $$ = createWhenStmt($3, $5, $6);
    }
    | WHEN '(' expr ')' block ELSE block {
        $$ = createWhenStmtWithElse($3, $5, NULL, $7);
    }
    | WHEN '(' expr ')' block when_or_list ELSE block {
        $$ = createWhenStmtWithElse($3, $5, $6, $8);
    }
    ;

/* List of OR branches (can be empty) */
when_or_list:
    when_or_branch {
        $$ = $1;
    }
    | when_or_list when_or_branch {
        $$ = createWhenOrList($1, $2);
    }
    ;

/* Individual OR branch with condition and block */
when_or_branch:
    WHENOR '(' expr ')' block {
        $$ = createWhenOrBranch($3, $5);
    }
    ;

/* Break-when statement: break when(expr); */
break_when_stmt:
    BREAK WHEN '(' expr ')' ';' {
        $$ = createBreakWhen($4);
    }
    ;
/* ===== END: WHEN LOOP FEATURE ===== */

/* ============================================================================
   FUNCTION RETURNS AND I/O
   ============================================================================ */

return_stmt:
    RETURN expr ';' {
        $$ = createReturn($2);
    }
    | RETURN ';' {
        $$ = createReturn(NULL);
    }
    ;

print_stmt:
    PRINT '(' expr ')' ';' { 
        $$ = createPrint($3);
    }
    | PRINTLN '(' expr ')' ';' {
        $$ = createPrintln($3);
    }
    ;

/* ============================================================================
   BLOCK STATEMENTS
   ============================================================================ */

block:
    '{' stmt_list '}' {
        $$ = createBlock($2);
    }
    | '{' '}' {
        $$ = createBlock(NULL);
    }
    ;

/* ============================================================================
   EXPRESSIONS (organized by type)
   ============================================================================ */

expr:
    /* Literals */
    NUM { 
        $$ = createNum($1, 0);
    }
    | FNUM { 
        $$ = createNum($1, 1);
    }
    | TRUE {
        $$ = createNum(1, 0);
    }
    | FALSE {
        $$ = createNum(0, 0);
    }
    /* ===== NEW: STRING LITERAL SUPPORT ===== */
    | STRING {
        $$ = createString($1);
        free($1);
    }
    /* ===== END: STRING LITERAL SUPPORT ===== */
    
    /* Variables and array access */
    | ID { 
        $$ = createVar($1);
        free($1);
    }
    | ID '[' expr ']' {
        $$ = createArrayAccess($1, $3);
        free($1);
    }
    | ID '[' expr ']' '[' expr ']' {
        $$ = createArray2DAccess($1, $3, $6);
        free($1);
    }
    
    /* Parenthesized expressions */
    | '(' expr ')' { 
        $$ = $2;
    }
    
    /* Arithmetic operations */
    | expr '+' expr { 
        $$ = createBinOp(OP_ADD, $1, $3);
    }
    | expr '-' expr { 
        $$ = createBinOp(OP_SUB, $1, $3);
    }
    | expr '*' expr { 
        $$ = createBinOp(OP_MUL, $1, $3);
    }
    | expr '/' expr { 
        $$ = createBinOp(OP_DIV, $1, $3);
    }
    
    /* Comparison operations */
    | expr EQ expr {
        $$ = createBinOp(OP_EQ, $1, $3);
    }
    | expr NEQ expr {
        $$ = createBinOp(OP_NEQ, $1, $3);
    }
    | expr LT expr {
        $$ = createBinOp(OP_LT, $1, $3);
    }
    | expr GT expr {
        $$ = createBinOp(OP_GT, $1, $3);
    }
    | expr LTE expr {
        $$ = createBinOp(OP_LTE, $1, $3);
    }
    | expr GTE expr {
        $$ = createBinOp(OP_GTE, $1, $3);
    }
    
    /* Logical operations */
    | expr AND expr {
        $$ = createBinOp(OP_AND, $1, $3);
    }
    | expr OR expr {
        $$ = createBinOp(OP_OR, $1, $3);
    }
    | NOT expr {
        $$ = createUnaryOp(OP_NOT, $2);  /* Unary NOT: left=$2, right=NULL */
    }
    
    /* Function calls */
    | func_call { 
        $$ = $1;
    }
    ;

/* Array element expressions */
arrayExpr:
    expr {
        $$ = createExprList($1, NULL);
    }
    | arrayExpr ',' expr {
        $$ = createExprList($3, $1);
    }
    ;

%%

/* ============================================================================
   ERROR HANDLING
   ============================================================================ */

void yyerror(const char* s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}

/* ============================================================================
   LANGUAGE FEATURES SUPPORTED
   ============================================================================
   1. Binary arithmetic: +, -, *, /
   2. Integer and float variables
   3. Variable declarations and assignments
   4. 1D and 2D arrays (int and float)
   5. Array element access and assignment
   6. Function declarations with parameters and return values
   7. Function calls with arguments
   8. Conditional statements: if/else
   9. Switch statements with cases and default
   10. Logical operators: &&, ||, !
   11. Comparison operators: ==, !=, <, >, <=, >=
   12. Print statements for output
   ============================================================================ */
