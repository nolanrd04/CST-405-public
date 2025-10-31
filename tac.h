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
    TAC_ARRAY_2D_ACCESS,

    /*Function*/
    TAC_FUNC_DECL,   /* Function declaration */
    TAC_FUNC_BEGIN,    /* Function begin */
    TAC_FUNC_END,      /* Function end */
    TAC_PARAM,         /* Function parameter */
    TAC_CALL,          /* Function call */\
    TAC_ARG,           /* Function argument */
    TAC_RETURN,         /* Return statement */
    TAC_LABEL ,         /* Label for jumps */
    TAC_ENTER_SCOPE , /* Enter new scope */
    TAC_EXIT_SCOPE,    /* Exit scope */

    /*Control Flow*/
    TAC_LABEL,        /* Label for jumps */
    TAC_GOTO ,       /* Unconditional jump */
    TAC_IF_FALSE,    /* Conditional jump */
    TAC_LT,          /* Less than comparison */
    TAC_GT ,         /* Greater than comparison */
    TAC_EQ ,         /* Equality comparison */
    TAC_NE ,        /* Not equal comparison */

    /*Switch Specific*/
    TAC_SWITCH,    /* Switch statement */
    TAC_CASE,      /* Case statement */
    TAC_DEFAULT      /* Default case */

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

/*Function Specific TAC generation*/
void generateFunctionTAC(ASTNode* node);                       /* Generate TAC for functions */
void generateParamTac(ASTNode* node, char* funcName);          /*Generate TAC for parameters*/
void generateFuncCallTAC(ASTNode* node,char** resultTemp);     /* Generate TAC for function calls */
void generateArgListTAC(ASTNode* node);                           /* Generate TAC for arguments */

/* TAC OPTIMIZATION AND OUTPUT */
void printTAC();                                                   /* Display unoptimized TAC */
void optimizeTAC();                                                /* Apply optimizations */
void printOptimizedTAC();                                          /* Display optimized TAC */

#endif