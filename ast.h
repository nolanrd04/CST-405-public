#ifndef AST_H
#define AST_H

/* ABSTRACT SYNTAX TREE (AST)
 * The AST is an intermediate representation of the program structure
 * It represents the hierarchical syntax of the source code
 * Each node represents a construct in the language
 */

/* NODE TYPES - Different kinds of AST nodes in our language */
typedef enum {
    NODE_NUM,       /* Numeric literal (e.g., 42) */
    NODE_BOOL,      /* Boolean literal (e.g., true/false) */
    NODE_UNARYOP,    /* Unary operation (e.g., !x) */
    NODE_VAR,       /* Variable reference (e.g., x) */
    NODE_BINOP,     /* Binary operation (e.g., x + y) */
    NODE_DECL,      /* Variable declaration (e.g., int x) */
    NODE_ASSIGN,    /* Assignment statement (e.g., x = 10) */
    NODE_PRINT,     /* Print statement (e.g., print(x)) */
    NODE_PRINTLN,   /* Println statement (e.g., println(x)) */
    NODE_IF,       /* If statement (e.g., if (cond) { ... } else { ... }) */
    NODE_STMT_LIST,  /* List of statements (program structure) */
    NODE_DECL_ASSIGN, /* Variable declaration with a type */

    /*Array Node Types*/
    NODE_ARRAY_DECL, /*Array declaration (e.g., int x[1]) */
    NODE_ARRAY_ASSIGN, /*Array assingment (e.g., x[5] = 10) */
    NODE_ARRAY_ACCESS, /*Array Acces (e.g., x[5]) */
    NODE_ARRAY_DECL_ASSIGN, /*Array declaration with assignment (e.g., int x[3] = {1,2,3}) */
    NODE_EXPR_LIST,   /*List of expressions (for array initializers)*/
    NODE_ARRAY_2D_DECL,
    NODE_ARRAY_2D_ACCESS,
    NODE_ARRAY_2D_ELEM_ASSIGN,

    /* ## NEW: Function related node types ## */
    NODE_FUNC_DECL, /* Function declaration (e.g., int foo(int a)) */
    NODE_PARAM,      /* Function parameter (e.g., int a) */
    NODE_PARAM_LIST, /* List of function parameters */
    NODE_BLOCK,      /* Block of statements (e.g., { ... }) */
    NODE_RETURN,     /* Return statement (e.g., return expr;) */
    NODE_FUNC_CALL,  /* Function call (e.g., foo(5)) */
    NODE_ARG_LIST    /* List of function call arguments */

    /* SWITCJ Control flow nodes*/
    ,NODE_SWITCH     /* Switch statement */
    ,NODE_CASE_LIST  /* List of case statements */
    ,NODE_CASE      /* Single case statement */
    ,NODE_BREAK     /* Break statement */
    ,NODE_DEFAULT_CASE /* Default case in switch */

    /* ===== NEW: WHEN LOOP FEATURE ===== */
    ,NODE_WHEN_STMT /* When loop statement */
    ,NODE_WHEN_OR_LIST /* List of when-or branches */
    ,NODE_WHEN_OR_BRANCH /* Single when-or branch */
    ,NODE_BREAK_WHEN /* Break-when statement */
    /* ===== END: WHEN LOOP FEATURE ===== */
    ,NODE_WHILE_STMT /* While loop statement */
    
    /* ===== NEW: STRING LITERAL SUPPORT ===== */
    ,NODE_STRING /* String literal */
    /* ===== END: STRING LITERAL SUPPORT ===== */
} NodeType;

typedef enum {
    OP_ADD,    // +
    OP_SUB,    // -
    OP_MUL,    // *
    OP_DIV,    // /
    OP_EQ,     // ==
    OP_NEQ,    // !=
    OP_LT,     // <
    OP_GT,     // >
    OP_LTE,    // <=
    OP_GTE,    // >=
    OP_AND,    // &&
    OP_OR,     // ||
    OP_NOT     // !
} BinOpType;

/* AST NODE STRUCTURE
 * Uses a union to efficiently store different node data
 * Only the relevant fields for each node type are used
 */
typedef struct ASTNode {
    NodeType type;  /* Identifies what kind of node this is */
    
    /* Union allows same memory to store different data types */
    union {
        /* Literal number value (NODE_NUM) */
        struct {
            int is_float; /* 1 if float, 0 if int */
            union {
                int ival;      /* Integer value */
                double fval;   /* Floating-point value */
                int bval;      /* Boolean value */
            } value;
        } num;

        struct {
            int bool_value; /* 1 for true, 0 for false */
        } boolVal;
        
        /* Variable reference (NODE_VAR) */
        char* name;

        /* Variable declaration (NODE_DECL) */
        struct {
            char* varName;
            char* varType;
        } decl;
        
        /* Binary operation structure (NODE_BINOP) */
        struct {
            BinOpType op;               /* Operator type */
            struct ASTNode* left;       /* Left operand */
            struct ASTNode* right;      /* Right operand */
        } binop;

        struct{
            BinOpType op;               /* Operator type */
            struct ASTNode* operand;    /* Operand */
        } unaryop;
        
        /* Assignment structure (NODE_ASSIGN) */
        struct {
            char* var;                  /* Variable being assigned to */
            struct ASTNode* value;      /* Expression being assigned */
        } assign;

        /* Assignment with declaration */
        struct {
            char* type; /* type of the identifier (variable)*/
            char* id; /* name of the identifier (a, b, etc)*/
            struct ASTNode* expr;
        } declAssign;
        
        /* Print expression (NODE_PRINT) */
        struct ASTNode* expr;
        
        /* Statement list structure (NODE_STMT_LIST) */
        struct {
            struct ASTNode* stmt;       /* Current statement */
            struct ASTNode* next;       /* Rest of the list */
        } stmtlist;
        
        /*Array declaration structure (NODE_ARRAY_DECL)*/
        struct {
            char* name;     /*Array name */
            int size;       /* Array Size*/
            char* type;
        }array_decl;

        /* Array assignment structure (NODE_ARRAY_ASSIGN) */
        struct {
            char* name;     /* Array name */
            struct ASTNode* index;  /* Index expression */
            struct ASTNode* value;  /* Value to assign */
        } array_assign;

        /* Array access structure (NODE_ARRAY_ACCESS) */
        struct {
            char* name ;    /* Array name */
            struct ASTNode* index; /* Index expression */

        }array_access;

        /* Expression list structure (NODE_EXPR_LIST) */
        struct {
            struct ASTNode* expr;      /* Current expression */
            struct ASTNode* next;      /* Next expression in list */
        } list; /* array */

        struct {
            char* type;              /* Type (e.g., "int") */
            char* name;              /* Array name */
            int size;                /* Size (0 if inferred from initializer) */
            struct ASTNode* initList; /* Initializer expression list */
        } array_decl_assign;

        struct
        {
            char* type;
            char* name;
            int sizeX;
            int sizeY;
        }array_2d_decl;

        struct{
            char* name;
            struct ASTNode* indexX;
            struct ASTNode* indexY;
        }array_2d_access;

        struct{
            char* name;
            struct ASTNode* indexX;
            struct ASTNode* indexY;
            struct ASTNode* value;
        }array_2d_elem_assign;

        /* ## NEW: function declaration structure (NODE_FUNC_DECL)## */
        struct {
            char* returnType;          /* Return type (e.g., "int") */
            char* name;                /* Function name */
            struct ASTNode* params;    /* Parameter list (NODE_PARAM_LIST) */
            struct ASTNode* body;      /* Function body (NODE_BLOCK) */
        } func_decl;

        /* ## NEW: Parameter structure (NODE_PARAM) ## */
        struct {
            char* type;                /* Parameter type (e.g., "int") */
            char* name;                /* Parameter name */
        } param;

        /* ## NEW: Parameter list structure (NODE_PARAM_LIST) ## */
        struct {
            struct ASTNode* param;     /* Current parameter (NODE_PARAM) */
            struct ASTNode* next;      /* Next parameter in list */
        } param_list;

        /* ## NEW: Block structure (NODE_BLOCK) ## */
        struct {
            struct ASTNode* stmts;   /* Statements in the block (NODE_STMT_LIST) */
        } block;

        /* ## NEW: Return statement structure (NODE_RETURN) ## */
        struct {
            struct ASTNode* value; /* return value (null for void) */
        } ret;

        /* ## NEW: Function call structure (NODE_FUNC_CALL) ## */
        struct {
            char* name;                /* Function name */
            struct ASTNode* args;      /* Argument list (NODE_ARG_LIST) */
        } func_call;

        /* ## NEW: Argument list structure (NODE_ARG_LIST) ## */
        struct {
            struct ASTNode* expr;       /* Current argument expression */
            struct ASTNode* next;      /* Next argument in list */
        } arg_list;

        /* ## NEW: Switch statement structure (NODE_SWITCH) ## */
        struct {
            struct ASTNode* expr;        /* Expression to switch on */
            struct ASTNode* cases;    /* List of cases (NODE_CASE_LIST) */
        } switch_stmt;

        struct{
            int value;                   /* Case value */
            struct ASTNode* stmts;       /* Statements for this case */
        } case_stmt;

        struct{
            struct ASTNode* stmts ;      /* Statements for default case */
        } default_case;

        struct{
            struct ASTNode* case_item;   /* current case*/
            struct ASTNode* next;        /* next case */
        } case_list;


        /* ===== NEW: WHEN LOOP FEATURE ===== */
        /* When statement structure (NODE_WHEN_STMT) */
        struct {
            struct ASTNode* primaryCond;  /* Primary when condition */
            struct ASTNode* primaryBlock; /* Primary when block */
            struct ASTNode* orBranches;   /* List of OR branches (NODE_WHEN_OR_LIST) */
            struct ASTNode* elseBlock;    /* Optional else block (may be NULL) */
        } when_stmt;

        /* When-or branch structure (NODE_WHEN_OR_BRANCH) */
        struct {
            struct ASTNode* condition;    /* Branch condition */
            struct ASTNode* block;        /* Branch block */
        } when_or_branch;

        /* When-or list structure (NODE_WHEN_OR_LIST) */
        struct {
            struct ASTNode* branch;       /* Current branch */
            struct ASTNode* next;         /* Next branch in list */
        } when_or_list;

        /* Break-when structure (NODE_BREAK_WHEN) */
        struct {
            struct ASTNode* expr;         /* Expression for break when */
        } break_when;
        
        /* While loop structure (NODE_WHILE_STMT) */
        struct {
            struct ASTNode* condition;    /* Loop condition */
            struct ASTNode* block;        /* Loop body */
        } while_stmt;
        
        /* ===== NEW: STRING LITERAL SUPPORT ===== */
        /* String literal structure (NODE_STRING) */
        struct {
            char* value;                  /* String value */
        } string_literal;
        /* ===== END: STRING LITERAL SUPPORT ===== */
    } data;

    // General purpose pointers for constructs needing multiple sub-nodes (if statements, etc.)
    struct ASTNode* condition;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

/* AST CONSTRUCTION FUNCTIONS
 * These functions are called by the parser to build the tree
 */
ASTNode* createNum(double value, int isFloat);                                   /* Create number node */
ASTNode* createVar(char* name);     /* Create variable node */
ASTNode* createBool(int value);                            /* Create boolean node */

/* ===== NEW: STRING LITERAL SUPPORT ===== */
ASTNode* createString(char* value);                        /* Create string literal node */
/* ===== END: STRING LITERAL SUPPORT ===== */

ASTNode* createUnaryOp(BinOpType op, ASTNode* operand); /* Create unary op node */
ASTNode* createBinOp(BinOpType op, ASTNode* left, ASTNode* right);   /* Create binary op node */

ASTNode* createDecl(char* varType, char* name);                                 /* Create declaration node */
ASTNode* createAssign(char* var, ASTNode* value);               /* Create assignment node */
ASTNode* createPrint(ASTNode* expr);                            /* Create print node */
ASTNode* createPrintln(ASTNode* expr);                          /* Create println node */
ASTNode* createIfNode(ASTNode* condition, ASTNode* thenBranch, ASTNode* elseBranch); /* Create if statement node */
ASTNode* createStmtList(ASTNode* stmt1, ASTNode* stmt2);  
      /* Create statement list */

/* Nolan added: */
ASTNode* createDeclAssign(char* type, char* id, ASTNode* expr);

/* arrays */
/* 1d */
ASTNode* createArrayDeclOfLength(char* type, char* id, int length);
ASTNode* createArrayAssign(char* name, char* id, int size, ASTNode* expr);
ASTNode* createArrayAccess(char* name, ASTNode* index);
ASTNode* createExprList(ASTNode* expr, ASTNode* next);
ASTNode* createArrayElemAssign(char* name, ASTNode* index, ASTNode* value);
ASTNode* createArrayDeclAssign(char* type, char* name, int size, ASTNode* initList);
/* 2d */
ASTNode* create2DArrayDeclOfLength(char* type, char* name, int sizeX, int sizeY);
ASTNode* createArray2DAccess(char* name, ASTNode* indexX, ASTNode* indexY);
ASTNode* createArray2DElemAssign(char* name, ASTNode* indexX, ASTNode* indexY, ASTNode* value);

/* NEW: function related AST construction functions */
ASTNode* createFuncDecl(char* returnType, char* name, ASTNode* params, ASTNode* body); /* Function declaration */
ASTNode* createParam(char* type, char* name);                     /* Function parameter */
ASTNode* createParamList(ASTNode* param, ASTNode* next);          /* Parameter list */
ASTNode* createBlock(ASTNode* stmts);                             /* Block of statements */
ASTNode* createReturn(ASTNode* value);                            /* Return statement */
ASTNode* createFuncCall(char* name, ASTNode* args);               /* Function call */
ASTNode* createArgList(ASTNode* expr, ASTNode* next);             /* Argument list */

/* Switch statement AST */
ASTNode* createSwitch(ASTNode* expr, ASTNode* cases);               /* Switch statement */
ASTNode* createCase(int value, ASTNode* stmts);                     /* Single case*/
ASTNode* createDefaultCase(ASTNode* stmts);
ASTNode* createCaseList(ASTNode* case1, ASTNode* case2);         /* List of cases */
ASTNode* createBreak();     
                                       /* Break statement */
/* ===== NEW: WHEN LOOP FEATURE ===== */
ASTNode* createWhenStmt(ASTNode* cond, ASTNode* block, ASTNode* orBranches); /* When statement */
ASTNode* createWhenStmtWithElse(ASTNode* cond, ASTNode* block, ASTNode* orBranches, ASTNode* elseBlock); /* When with else */
ASTNode* createWhenOrBranch(ASTNode* cond, ASTNode* block);       /* When-or branch */
ASTNode* createWhenOrList(ASTNode* branch1, ASTNode* branch2);    /* List of or branches */
ASTNode* createBreakWhen(ASTNode* expr);                           /* Break-when statement */
/* ===== END: WHEN LOOP FEATURE ===== */
/* ===== NEW: WHILE LOOP FEATURE ===== */
ASTNode* createWhileStmt(ASTNode* cond, ASTNode* block);          /* While loop statement */
/* ===== END: WHILE LOOP FEATURE ===== */
/* AST DISPLAY FUNCTION */
void printAST(ASTNode* node, int level);                        /* Pretty-print the AST */

#endif