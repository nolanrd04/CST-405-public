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
    NODE_VAR,       /* Variable reference (e.g., x) */
    NODE_BINOP,     /* Binary operation (e.g., x + y) */
    NODE_DECL,      /* Variable declaration (e.g., int x) */
    NODE_ASSIGN,    /* Assignment statement (e.g., x = 10) */
    NODE_PRINT,     /* Print statement (e.g., print(x)) */
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
} NodeType;

/* AST NODE STRUCTURE
 * Uses a union to efficiently store different node data
 * Only the relevant fields for each node type are used
 */
typedef struct ASTNode {
    NodeType type;  /* Identifies what kind of node this is */
    
    /* Union allows same memory to store different data types */
    union {
        /* Literal number value (NODE_NUM) */
        int num;
        
        /* Variable or declaration name (NODE_VAR, NODE_DECL) */
        char* name;
        
        /* Binary operation structure (NODE_BINOP) */
        struct {
            char op;                    /* Operator character ('+') */
            struct ASTNode* left;       /* Left operand */
            struct ASTNode* right;      /* Right operand */
        } binop;
        
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


    } data;
} ASTNode;

/* AST CONSTRUCTION FUNCTIONS
 * These functions are called by the parser to build the tree
 */
ASTNode* createNum(int value);                                   /* Create number node */
ASTNode* createVar(char* name);                                  /* Create variable node */
ASTNode* createBinOp(char op, ASTNode* left, ASTNode* right);   /* Create binary op node */
ASTNode* createDecl(char* name);                                 /* Create declaration node */
ASTNode* createAssign(char* var, ASTNode* value);               /* Create assignment node */
ASTNode* createPrint(ASTNode* expr);                            /* Create print node */
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


/* AST DISPLAY FUNCTION */
void printAST(ASTNode* node, int level);                        /* Pretty-print the AST */

#endif