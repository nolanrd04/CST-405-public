#ifndef TAC_H
#define TAC_H

#include "ast.h"

/* THREE-ADDRESS CODE (TAC)
 * Intermediate representation between AST and machine code
 * Each instruction has at most 3 operands (result = arg1 op arg2)
 * Makes optimization and code generation easier
 */

/* TAC INSTRUCTION TYPES */
typedef enum {
    TAC_ADD,           /* Addition: result = arg1 + arg2 */
    TAC_SUB,           /* Subtraction: result = arg1 - arg2 */
    TAC_MUL,
    TAC_DIV,           
    TAC_ASSIGN,        /* Assignment: result = arg1 */
    TAC_PRINT,         /* Print: print(arg1) */
    TAC_DECL,          /* Declaration: declare result */

    /* arrays */
    TAC_ARRAY_DECL,     /* Array declaration: declare array[size] */
    TAC_ARRAY_ASSIGN,  /* Array assignment: array[index] = value */
    TAC_ARRAY_ACCESS,   /* Array access: temp = array[index] */
    TAC_ARRAY_EXPR,
    TAC_ARRAY_2D_DECL,
    TAC_ARRAY_2D_ACCESS
} TACOp;

/* TAC INSTRUCTION STRUCTURE */
typedef struct TACInstr {
    TACOp op;               /* Operation type */
    char* arg1;             /* First operand (if needed) */
    char* arg2;             /* Second operand (for binary ops) */
    char* result;           /* Result/destination */
    struct TACInstr* next;  /* Linked list pointer */
} TACInstr;

/* TAC LIST MANAGEMENT */
typedef struct {
    TACInstr* head;    /* First instruction */
    TACInstr* tail;    /* Last instruction (for efficient append) */
    int tempCount;     /* Counter for temporary variables (t0, t1, ...) */
} TACList;

/* TAC GENERATION FUNCTIONS */
void initTAC();                                                    /* Initialize TAC lists */
char* newTemp();                                                   /* Generate new temp variable */
TACInstr* createTAC(TACOp op, char* arg1, char* arg2, char* result); /* Create TAC instruction */                   /* Propagate known values */
void appendTAC(TACInstr* instr);                                  /* Add instruction to list */
void generateTAC(ASTNode* node);                                  /* Convert AST to TAC */
char* generateTACExpr(ASTNode* node);                             /* Generate TAC for expression */

/* TAC OPTIMIZATION AND OUTPUT */
void printTAC();                                                   /* Display unoptimized TAC */
void optimizeTAC();                                                /* Apply optimizations */
void printOptimizedTAC();                                          /* Display optimized TAC */

#endif