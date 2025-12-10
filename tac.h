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
    TAC_MUL,           /* Multiplication */
    TAC_DIV,           /* Division */
    TAC_ASSIGN,        /* Assignment: result = arg1 */
    TAC_PRINT,         /* Print: print(arg1) */
    TAC_DECL,          /* Declaration: declare result */

    /* Logical Operations */
    TAC_AND,           /* Logical AND: result = arg1 && arg2 */
    TAC_OR,            /* Logical OR: result = arg1 || arg2 */
    TAC_NOT,           /* Logical NOT: result = !arg1 */
    
    /* Comparison operations */
    TAC_EQ,            /* == */
    TAC_NEQ,           /* != */
    TAC_LT,            /* < */
    TAC_GT,            /* > */
    TAC_LTE,           /* <= */
    TAC_GTE,           /* >= */

    /* Arrays */
    TAC_ARRAY_DECL,    /* Array declaration: declare array[size] */
    TAC_ARRAY_ASSIGN,  /* Array assignment: array[index] = value */
    TAC_ARRAY_ACCESS,  /* Array access: temp = array[index] */
    TAC_ARRAY_EXPR,    /* Array expression */
    TAC_ARRAY_2D_DECL, /* 2D array declaration */
    TAC_ARRAY_2D_ACCESS, /* 2D array access */

    /* Function-related */
    TAC_FUNC_DECL,     /* Function declaration */
    TAC_FUNC_BEGIN,    /* Mark start of function body */
    TAC_FUNC_END,      /* Mark end of function */
    TAC_PARAM,         /* Function parameter */
    TAC_CALL,          /* Function call: result = call funcName */
    TAC_ARG,           /* Pass argument: arg argValue */
    TAC_RETURN,        /* Return: return arg1 */

    /* Control flow */
    TAC_IFZ,           /* Conditional jump: if arg1 == 0 goto result */
    TAC_GOTO,          /* Unconditional jump: goto result */
    TAC_LABEL,         /* Label definition: result: */
    TAC_IF_FALSE,      /* Conditional jump if false */
    TAC_ENTER_SCOPE,   /* Enter new scope */
    TAC_EXIT_SCOPE,    /* Exit scope */

    /* Switch-specific */
    TAC_SWITCH,        /* Switch statement */
    TAC_CASE,          /* Case statement */
    TAC_DEFAULT        /* Default case */

    /* ===== NEW: WHEN LOOP FEATURE ===== */
    ,TAC_WHEN_START,   /* Mark start of when loop */
    TAC_WHEN_CHECK,   /* Check when condition */
    TAC_WHEN_OR,      /* When OR branch check */
    TAC_WHEN_END,     /* Mark end of when loop */
    TAC_BREAK_WHEN    /* Break-when statement */
    /* ===== END: WHEN LOOP FEATURE ===== */

    ,TAC_WHILE_START,  /* Mark start of while loop */
    TAC_WHILE_CHECK,  /* Check while condition */
    TAC_WHILE_END     /* Mark end of while loop */

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
    int labelCount;    /* Counter for labels (L0, L1, ...) */
} TACList;

/* TAC GENERATION FUNCTIONS */
void initTAC();                                                    /* Initialize TAC lists */
char* newTemp();                                                   /* Generate new temp variable */
char* newLabel();                                                  /* Generate new label */
TACInstr* createTAC(TACOp op, char* arg1, char* arg2, char* result); /* Create TAC instruction */                   /* Propagate known values */
void appendTAC(TACInstr* instr);                                  /* Add instruction to list */
void generateTAC(ASTNode* node);                                  /* Convert AST to TAC */
char* generateTACExpr(ASTNode* node);                             /* Generate TAC for expression */
void generateTAC_If(ASTNode* node);                             /* Generate TAC for if statement */

/* ===== NEW: WHEN LOOP FEATURE ===== */
void generateTAC_When(ASTNode* node);                            /* Generate TAC for when loop */
void generateTAC_WhenOrList(ASTNode* node);                      /* Generate TAC for or branches */
/* ===== END: WHEN LOOP FEATURE ===== */

void generateTAC_While(ASTNode* node);                           /* Generate TAC for while loop */

/*Function Specific TAC generation*/
void generateFunctionTAC(ASTNode* node);                       /* Generate TAC for functions */
void generateParamTac(ASTNode* node, char* funcName);          /*Generate TAC for parameters*/
void generateFuncCallTAC(ASTNode* node,char** resultTemp);     /* Generate TAC for function calls */
void generateArgListTAC(ASTNode* node);                           /* Generate TAC for arguments */

/* OPTIMIZATION STATISTICS TRACKING */
typedef struct {
    int constantFolds;           /* Number of constant folding optimizations */
    int algebraicSimplifications; /* Number of algebraic simplifications (x+0, x*1, etc.) */
    int deadCodeEliminations;    /* Number of dead code instructions removed */
    int strengthReductions;      /* Number of strength reductions (x*2 -> x+x) */
    int commonSubexprEliminated; /* Number of common subexpressions eliminated */
    int copyPropagations;        /* Number of copy propagations */
    int instructionsBefore;      /* Total instructions before optimization */
    int instructionsAfter;       /* Total instructions after optimization */
} OptimizationStats;

/* TAC OPTIMIZATION AND OUTPUT */
void printTAC();                                                   /* Display unoptimized TAC */
void optimizeTAC();                                                /* Apply optimizations */
void printOptimizedTAC();                                          /* Display optimized TAC */
void printOptimizationStats();                                     /* Display optimization statistics */

#endif