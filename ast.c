/* AST IMPLEMENTATION
 * Functions to create and manipulate Abstract Syntax Tree nodes
 * The AST is built during parsing and used for all subsequent phases
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Create a number literal node */
ASTNode* createNum(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_NUM;
    node->data.num = value;  /* Store the integer value */
    return node;
}

/* Create a variable reference node */
ASTNode* createVar(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_VAR;
    node->data.name = strdup(name);  /* Copy the variable name */
    return node;
}

/* Create a binary operation node (for addition) */
ASTNode* createBinOp(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINOP;
    node->data.binop.op = op;        /* Store operator (+) */
    node->data.binop.left = left;    /* Left subtree */
    node->data.binop.right = right;  /* Right subtree */
    return node;
}

/* Create a variable declaration node */
ASTNode* createDecl(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_DECL;
    node->data.name = strdup(name);  /* Store variable name */
    return node;
}

/* Create an assignment statement node */
ASTNode* createAssign(char* var, ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->data.assign.var = strdup(var);  /* Variable name */
    node->data.assign.value = value;      /* Expression tree */
    return node;
}

/* Create a print statement node */
ASTNode* createPrint(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PRINT;
    node->data.expr = expr;  /* Expression to print */
    return node;
}

/* Create a statement list node (links statements together) */
ASTNode* createStmtList(ASTNode* stmt1, ASTNode* stmt2) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_STMT_LIST;
    node->data.stmtlist.stmt = stmt1;  /* First statement */
    node->data.stmtlist.next = stmt2;  /* Rest of list */
    return node;
}

ASTNode* createDeclAssign(char* type, char* id, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_DECL_ASSIGN;
    node->data.declAssign.type = strdup(type);
    node->data.declAssign.id = strdup(id);
    node->data.declAssign.expr = expr;
    return node;
}

ASTNode* createArrayDeclOfLength(char* type, char* name, int size) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_DECL;
    node->data.array_decl.name = strdup(name);
    node->data.array_decl.size = size;
    return node;
}

ASTNode* createArrayAssign(char* name, char* id, int size, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_ASSIGN;
    node->data.array_assign.name = strdup(id);
    node->data.array_assign.index = NULL; // No index for full array assignment
    node->data.array_assign.value = expr;
    return node;
}

ASTNode* createArrayAccess(char* name, ASTNode* index) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_ACCESS;
    node->data.array_access.name = strdup(name);
    node->data.array_access.index = index;
    return node;
}

ASTNode* createExprList(ASTNode* expr, ASTNode* next) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_STMT_LIST; // Reusing statement list for expression list
    node->data.stmtlist.stmt = expr;
    node->data.stmtlist.next = next;
    return node;
}

ASTNode* createArrayElemAssign(char* name, ASTNode* index, ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_ASSIGN;
    node->data.array_assign.name = strdup(name);
    node->data.array_assign.index = index;
    node->data.array_assign.value = value;
    return node;
}

ASTNode* createArrayDeclAssign(char* type, char* name, int size, ASTNode* initList) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_DECL_ASSIGN;
    node->data.array_decl_assign.type = strdup(type);
    node->data.array_decl_assign.name = strdup(name);
    node->data.array_decl_assign.size = size;
    node->data.array_decl_assign.initList = initList;
    return node;
}

ASTNode* create2DArrayDeclOfLength(char* type, char* name, int sizeX, int sizeY)
{
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_2D_DECL;
    node->data.array_2d_decl.type = strdup(type);
    node->data.array_2d_decl.name = strdup(name);
    node->data.array_2d_decl.sizeX = sizeX;
    node->data.array_2d_decl.sizeY = sizeY;
    return node;
}

ASTNode* createArray2DAccess(char* name, ASTNode* indexX, ASTNode* indexY)
{
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_2D_ACCESS;
    node->data.array_2d_access.name = strdup(name);
    node->data.array_2d_access.indexX = indexX;
    node->data.array_2d_access.indexY = indexY;
    return node;
}

ASTNode* createArray2DElemAssign(char* name, ASTNode* indexX, ASTNode* indexY, ASTNode* value)
{
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_2D_ELEM_ASSIGN;
    node->data.array_2d_elem_assign.name = strdup(name);
    node->data.array_2d_elem_assign.indexX = indexX;
    node->data.array_2d_elem_assign.indexY = indexY;
    node->data.array_2d_elem_assign.value = value;
    return node;
}

ASTNode* createFuncDecl(char* returnType, char* name, ASTNode* params, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_FUNC_DECL;
    node->data.func_decl.returnType = strdup(returnType); /*store return type */
    node->data.func_decl.name = strdup(name);         /* store function name */
    node->data.func_decl.params = params;               /* Parameter list (NODE_PARAM_LIST) */
    node->data.func_decl.body = body;                   /* Function body (NODE_BLOCK) */
    return node;
}

ASTNode* createParam(char* type, char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PARAM;
    node->data.param.type = strdup(type); /* store parameter type */
    node->data.param.name = strdup(name); /* store parameter name */
    return node;
}

ASTNode* createParamList(ASTNode* param, ASTNode* next){
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PARAM_LIST;
    node->data.param_list.param = param; /* Current parameter (NODE_PARAM) */
    node->data.param_list.next = next;   /* Next parameter (NODE_PARAM_LIST) */
    return node;
}

ASTNode* createBlock(ASTNode* stmts) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BLOCK;
    node->data.block.stmts = stmts; /* Store the list of statements */
    return node;
}

ASTNode* createFuncCall(char* name, ASTNode* args){
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_FUNC_CALL;
    node->data.func_call.name = strdup(name); /* Store function name */
    node->data.func_call.args = args;         /* Store argument list (NODE_ARG_LIST) */
    return node;
}

ASTNode* createArgList(ASTNode* expr, ASTNode* next) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ARG_LIST;
    node->data.arg_list.expr = expr;   /* Current argument expression */
    node->data.arg_list.next = next;   /* Next argument in list */
    return node;
}

ASTNode* createReturn(ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    node->data.ret.value = value;
    return node;
}

/* Display the AST structure (for debugging and education) */
void printAST(ASTNode* node, int level) {
    if (!node) return;
    
    /* Indent based on tree depth */
    for (int i = 0; i < level; i++) printf("  ");
    
    /* Print node based on its type */
    switch(node->type) {
        case NODE_NUM:
            printf("NUM: %d\n", node->data.num);
            break;
        case NODE_VAR:
            printf("VAR: %s\n", node->data.name);
            break;
        case NODE_BINOP:
            printf("BINOP: %c\n", node->data.binop.op);
            printAST(node->data.binop.left, level + 1);
            printAST(node->data.binop.right, level + 1);
            break;
        case NODE_DECL:
            printf("DECL: %s\n", node->data.name);
            break;
        case NODE_ASSIGN:
            printf("ASSIGN: %s\n", node->data.assign.var);
            printAST(node->data.assign.value, level + 1);
            break;
        case NODE_PRINT:
            printf("PRINT\n");
            printAST(node->data.expr, level + 1);
            break;
        case NODE_STMT_LIST:
            /* Print statements in sequence at same level */
            printAST(node->data.stmtlist.stmt, level);
            printAST(node->data.stmtlist.next, level);
            break;
        case NODE_DECL_ASSIGN:
            printf("DECL_ASSIGN: %s %s\n", node->data.declAssign.type, node->data.declAssign.id);
            printAST(node->data.declAssign.expr, level + 1);
            break;
        case NODE_ARRAY_DECL:
            printf("ARRAY_DECL: %s[%d]\n", node->data.array_decl.name, node->data.array_decl.size);
            break;
        case NODE_ARRAY_ASSIGN:
            printf("ARRAY_ASSIGN: %s\n", node->data.array_assign.name);
            printAST(node->data.array_assign.value, level + 1);
            break;
        case NODE_ARRAY_ACCESS:
            printf("ARRAY_ACCESS: %s\n", node->data.array_access.name);
            printAST(node->data.array_access.index, level + 1);
            break;
        case NODE_EXPR_LIST:
            printf("EXPR_LIST:\n");
            printAST(node->data.list.expr, level + 1);
            printAST(node->data.list.next, level + 1);
            break;
        case NODE_ARRAY_DECL_ASSIGN:
            printf("ARRAY_DECL_ASSIGN: %s %s[%d]\n", node->data.array_decl_assign.type, node->data.array_decl_assign.name, node->data.array_decl_assign.size);
            printAST(node->data.array_decl_assign.initList, level + 1);
            break;
        case NODE_ARRAY_2D_DECL:
            printf("ARRAY_2D_DECL: %s[%d][%d]\n", 
            node->data.array_2d_decl.name,
            node->data.array_2d_decl.sizeX, 
            node->data.array_2d_decl.sizeY);
            break;
        case NODE_ARRAY_2D_ACCESS:
            printf("ARRAY_2D_ACCESS: %s\n", node->data.array_2d_access.name);
            printf("  IndexX:\n");
            printAST(node->data.array_2d_access.indexX, level + 1);
            printf("  IndexY:\n");
            printAST(node->data.array_2d_access.indexY, level + 1);
            break;
        case NODE_ARRAY_2D_ELEM_ASSIGN:
            printf("ARRAY_2D_ASSIGN: %s\n", node->data.array_2d_elem_assign.name);
            printf("  IndexX:\n");
            printAST(node->data.array_2d_elem_assign.indexX, level + 1);
            printf("  IndexY:\n");
            printAST(node->data.array_2d_elem_assign.indexY, level + 1);
            printf("\nValue:\n");
            printAST(node->data.array_2d_elem_assign.value, level + 1);
            break;
        case NODE_FUNC_DECL:
            printf("FUNC_DECL: %s %s\n", node->data.func_decl.returnType, node->data.func_decl.name);
            if (node->data.func_decl.params) {
                for (int i = 0; i < level +1; i++) printf("  ");
                printf("Parameters:\n");
                printAST(node->data.func_decl.params, level + 2);
            }
            for (int i = 0; i < level +1; i++) printf("  ");
            printf("Body:\n");
            printAST(node->data.func_decl.body, level + 2);
            break;
        case NODE_PARAM:
            printf("PARAM: %s %s\n", node->data.param.type, node->data.param.name);
            break;
        case NODE_PARAM_LIST:
            printAST(node->data.param_list.param, level);
            printAST(node->data.param_list.next, level);
            break;
        case NODE_BLOCK:
            printf("BLOCK:\n");
            printAST(node->data.block.stmts, level + 1);
            break;
        case NODE_RETURN:
            printf("RETURN:\n");
            if (node ->data.ret.value){
                printAST(node->data.ret.value, level + 1);
            }
            break;
        case NODE_FUNC_CALL:
            printf("FUNC_CALL: %s\n", node->data.func_call.name);
            if (node->data.func_call.args) {
                for (int i = 0; i < level +1; i++) printf("  ");
                printf("Arguments:\n");
                printAST(node->data.func_call.args, level + 2);
            }
            break;
        case NODE_ARG_LIST:
            printAST(node->data.arg_list.expr, level);
            printAST(node->data.arg_list.next, level);
            break;
    }
}