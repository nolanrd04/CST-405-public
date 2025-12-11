#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symtab.h"
#include "tac.h"

/* DEBUG OUTPUT CONTROL - Set to 0 to disable all debug messages */
#define ENABLE_DEBUG_OUTPUT 1

/* External reference to log file from main.c */
extern FILE* log_file;

/* Debug print macro - only prints if ENABLE_DEBUG_OUTPUT is 1 */
#if ENABLE_DEBUG_OUTPUT
    #define DEBUG_PRINT(...) do { \
        if (log_file) { \
            fprintf(log_file, __VA_ARGS__); \
            fflush(log_file); \
        } \
    } while(0)
#else
    #define DEBUG_PRINT(...) do {} while(0)
#endif

extern TACList optimizedList;

FILE* output;
int tempReg = 0;
char* currentFunctionType = NULL;
int currentFunctionPrologueSize = 0;  // Track how much space was allocated in prologue
int stackAdjustment = 0;  // Track dynamic stack pushes during expression evaluation

/* ===== NEW: WHEN LOOP FEATURE ===== */
/* Stack for tracking nested when loop end labels */
#define MAX_WHEN_DEPTH 10
char* whenLoopStack[MAX_WHEN_DEPTH];
int whenLoopDepth = 0;

void pushWhenLabel(char* label) {
    if (whenLoopDepth < MAX_WHEN_DEPTH) {
        whenLoopStack[whenLoopDepth++] = label;
    }
}

char* popWhenLabel() {
    if (whenLoopDepth > 0) {
        return whenLoopStack[--whenLoopDepth];
    }
    return NULL;
}

char* currentWhenLabel() {
    if (whenLoopDepth > 0) {
        return whenLoopStack[whenLoopDepth - 1];
    }
    return NULL;
}
/* ===== END: WHEN LOOP FEATURE ===== */

/* ===== NEW: STRING LITERAL SUPPORT ===== */
/* String table for tracking string literals */
#define MAX_STRINGS 100
struct {
    char* value;
    int id;
} stringTable[MAX_STRINGS];
int stringCount = 0;

int addString(char* value) {
    for (int i = 0; i < stringCount; i++) {
        if (strcmp(stringTable[i].value, value) == 0) {
            return stringTable[i].id;  /* Return existing ID */
        }
    }
    if (stringCount < MAX_STRINGS) {
        stringTable[stringCount].value = strdup(value);
        stringTable[stringCount].id = stringCount;
        return stringCount++;
    }
    return -1;
}
/* ===== END: STRING LITERAL SUPPORT ===== */
/* ===== END: WHEN LOOP FEATURE ===== */

int getNextTemp() {
    int reg = tempReg;
    tempReg = (tempReg + 1) % 8;
    return reg;
}

int isExprFloat(ASTNode* node) {
    if (!node) return 0;

    switch(node->type) {
        case NODE_NUM:
            return node->data.num.is_float;
        case NODE_VAR: {
            char* type = getVarType(node->data.name);
            return (type && strcmp(type, "float") == 0);
        }
        case NODE_BINOP:
            return isExprFloat(node->data.binop.left) || isExprFloat(node->data.binop.right);
        case NODE_ARRAY_ACCESS:
        case NODE_ARRAY_2D_ACCESS: {
            char* type = getVarType(node->data.array_access.name);
            return (type && strcmp(type, "float") == 0);
        }
        default:
            return 0;
    }
}

// Helper function to check if expression is boolean
int isExprBool(ASTNode* node) {
    if (!node) return 0;

    switch(node->type) {
        case NODE_BOOL:
            return 1;
        case NODE_VAR: {
            char* type = getVarType(node->data.name);
            return isBoolType(type);
        }
        case NODE_BINOP: {
            /* Logical and comparison operators produce boolean results */
            return (node->data.binop.op == OP_AND || 
                    node->data.binop.op == OP_OR ||
                    node->data.binop.op == OP_EQ ||
                    node->data.binop.op == OP_NEQ ||
                    node->data.binop.op == OP_LT ||
                    node->data.binop.op == OP_GT ||
                    node->data.binop.op == OP_LTE ||
                    node->data.binop.op == OP_GTE);
        }
        case NODE_UNARYOP:
            return (node->data.unaryop.op == OP_NOT);
        default:
            return 0;
    }
}

// Helper function to flatten case list structure
void collectSwitchCases(ASTNode* node, ASTNode** cases, int* caseCount, int* hasDefault, int maxCases) {
    if (!node || *caseCount >= maxCases) return;
    
    if (node->type == NODE_CASE_LIST) {
        /* Recursively collect from case_item and next */
        collectSwitchCases(node->data.case_list.case_item, cases, caseCount, hasDefault, maxCases);
        collectSwitchCases(node->data.case_list.next, cases, caseCount, hasDefault, maxCases);
    } else if (node->type == NODE_CASE) {
        cases[*caseCount] = node;
        (*caseCount)++;
    } else if (node->type == NODE_DEFAULT_CASE) {
        cases[*caseCount] = node;
        (*caseCount)++;
        *hasDefault = 1;
    }
}

// Helper function to convert integer in $t register to float in $f register
void genIntToFloatConversion(int tReg, int fReg) {
    fprintf(output, "    mtc1 $t%d, $f%d\n", tReg, fReg);
    fprintf(output, "    cvt.s.w $f%d, $f%d\n", fReg, fReg);
}

// Helper function to check if expression is a string
int isExprString(ASTNode* node) {
    if (!node) return 0;
    return (node->type == NODE_STRING);
}

void genExpr(ASTNode* node);

// Helper function to check if an expression contains function calls
// Function calls can reset tempReg, so we need to save registers
int containsFunctionCall(ASTNode* node) {
    if (!node) return 0;

    switch(node->type) {
        case NODE_FUNC_CALL:
            return 1;  // This is a function call
        case NODE_BINOP:
            return containsFunctionCall(node->data.binop.left) ||
                   containsFunctionCall(node->data.binop.right);
        case NODE_UNARYOP:
            return containsFunctionCall(node->data.unaryop.operand);
        case NODE_ARRAY_ACCESS:
            return containsFunctionCall(node->data.array_access.index);
        case NODE_ARRAY_2D_ACCESS:
            return containsFunctionCall(node->data.array_2d_access.indexX) ||
                   containsFunctionCall(node->data.array_2d_access.indexY);
        default:
            return 0;  // Literals, variables, etc. don't contain function calls
    }
}


void genExpr(ASTNode* node) {
    if (!node) {
        DEBUG_PRINT("[DEBUG] genExpr: NULL node\n");
        return;
    }

    DEBUG_PRINT("[DEBUG] genExpr: Processing node type %d\n", node->type);

    switch(node->type) {
        case NODE_NUM:
            if(node->data.num.is_float) {
                fprintf(output, "    li.s $f0, %.6f\n", node->data.num.value.fval);
                tempReg = 0;
            } else {
                fprintf(output, "    li $t%d, %d\n", tempReg, node->data.num.value.ival);
                tempReg++;
            }
            break;

        case NODE_BOOL: {
            fprintf(output, "    li $t%d, %d\n", tempReg, node->data.boolVal.bool_value);
            tempReg++;
            break;
        }

        /* ===== NEW: STRING LITERAL SUPPORT ===== */
        case NODE_STRING: {
            int stringId = addString(node->data.string_literal.value);
            
            /* Generate string data section reference */
            fprintf(output, "    la $t%d, str_%d      # Load address of string literal\n", 
                    tempReg, stringId);
            
            tempReg++;
            break;
        }
        /* ===== END: STRING LITERAL SUPPORT ===== */

        case NODE_VAR: {
            int offset = getVarOffset(node->data.name);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s not declared\n", node->data.name);
                exit(1);
            }
            char* type = getVarType(node->data.name);

            /* ===== NEW: GLOBAL VARIABLES SUPPORT ===== */
            if (isGlobalVar(node->data.name)) {
                // Global variable - use label-based addressing
                if (type && strcmp(type, "float") == 0) {
                    fprintf(output, "    lwc1 $f0, %s\n", node->data.name);
                    tempReg = 0;
                } else {
                    fprintf(output, "    lw $t%d, %s\n", getNextTemp(), node->data.name);
                }
            } else {
                // Local variable - use stack-relative addressing
                // FIX: Adjust offset for any dynamic stack pushes
                int adjustedOffset = offset + stackAdjustment;
                if (type && strcmp(type, "float") == 0) {
                    fprintf(output, "    lwc1 $f0, %d($sp)\n", adjustedOffset);
                    tempReg = 0;
                } else {
                    fprintf(output, "    lw $t%d, %d($sp)\n", getNextTemp(), adjustedOffset);
                }
            }
            /* ===== END: GLOBAL VARIABLES SUPPORT ===== */
            break;
        }
            
        case NODE_UNARYOP: {
            if (node->data.unaryop.op == OP_NOT) {
                genExpr(node->data.unaryop.operand);
                int operandReg = (tempReg > 0) ? tempReg - 1 : 0;
                fprintf(output, "    # Logical NOT\n");
                fprintf(output, "    seq $t%d, $t%d, $zero  # NOT operation\n", 
                        operandReg, operandReg);
                tempReg = operandReg + 1;
            }
            break;
        }
        
        case NODE_BINOP: {
            int isFloat = isExprFloat(node);
            int leftIsFloat = isExprFloat(node->data.binop.left);
            int rightIsFloat = isExprFloat(node->data.binop.right);

            if (isFloat) {
                genExpr(node->data.binop.left);

                // If left operand is int but we're doing float operation, convert it
                if (!leftIsFloat) {
                    genIntToFloatConversion(0, 4);  // Convert $t0 to $f4
                } else {
                    // Left result is in $f0, move to $f4
                    fprintf(output, "    mov.s $f4, $f0\n");
                }

                // Save $f4 to stack temporarily
                fprintf(output, "    addi $sp, $sp, -4\n");
                fprintf(output, "    swc1 $f4, 0($sp)\n");

                genExpr(node->data.binop.right);

                // If right operand is int but we're doing float operation, convert it
                if (!rightIsFloat) {
                    genIntToFloatConversion(0, 2);  // Convert $t0 to $f2
                } else {
                    // Right result is in $f0, move to $f2
                    fprintf(output, "    mov.s $f2, $f0\n");
                }

                // Restore left from stack to $f4
                fprintf(output, "    lwc1 $f4, 0($sp)\n");
                fprintf(output, "    addi $sp, $sp, 4\n");

                if (node->data.binop.op == OP_ADD) {
                    fprintf(output, "    add.s $f0, $f4, $f2\n");
                } else if (node->data.binop.op == OP_SUB) {
                    fprintf(output, "    sub.s $f0, $f4, $f2\n");
                } else if (node->data.binop.op == OP_MUL) {
                    fprintf(output, "    mul.s $f0, $f4, $f2\n");
                } else if (node->data.binop.op == OP_DIV) {
                    fprintf(output, "    div.s $f0, $f4, $f2\n");
                } else {
                    fprintf(stderr, "Error: unsupported binary op for float\n");
                    exit(1);
                }
                tempReg = 0;
            } else {
                int leftReg, rightReg;
                int needsSave = containsFunctionCall(node->data.binop.right);

                // Evaluate left operand
                genExpr(node->data.binop.left);
                leftReg = (tempReg > 0) ? tempReg - 1 : 0;

                int resultReg, savedLeftReg;
                if (needsSave) {
                    // FIX: Push left operand to stack to preserve across recursive calls
                    // Track the stack adjustment so variable loads use correct offsets
                    fprintf(output, "    addi $sp, $sp, -4\n");
                    fprintf(output, "    sw $t%d, 0($sp)     # Save left operand\n", leftReg);
                    stackAdjustment += 4;  // Track that we pushed 4 bytes

                    // Evaluate right operand (may include recursive calls)
                    genExpr(node->data.binop.right);
                    rightReg = (tempReg > 0) ? tempReg - 1 : 0;

                    // Pop left operand from stack
                    savedLeftReg = (rightReg == 0) ? 1 : 0;  // Use different register
                    fprintf(output, "    lw $t%d, 0($sp)     # Restore left operand\n", savedLeftReg);
                    fprintf(output, "    addi $sp, $sp, 4\n");
                    stackAdjustment -= 4;  // Track that we popped 4 bytes
                    resultReg = savedLeftReg;
                } else {
                    // No save needed - evaluate right operand
                    genExpr(node->data.binop.right);
                    rightReg = (tempReg > 0) ? tempReg - 1 : 0;

                    // Left is still in leftReg
                    savedLeftReg = leftReg;
                    resultReg = leftReg;
                }

                // Arithmetic operators
                if (node->data.binop.op == OP_ADD) {
                    fprintf(output, "    add $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_SUB) {
                    fprintf(output, "    sub $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_MUL) {
                    fprintf(output, "    mul $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_DIV) {
                    fprintf(output, "    div $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                }
                // Comparison operators
                else if (node->data.binop.op == OP_GT) {
                    fprintf(output, "    sgt $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_LT) {
                    fprintf(output, "    slt $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_GTE) {
                    fprintf(output, "    sge $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_LTE) {
                    fprintf(output, "    sle $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_EQ) {
                    fprintf(output, "    seq $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_NEQ) {
                    fprintf(output, "    sne $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_AND) {
                    fprintf(output, "    and $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else if (node->data.binop.op == OP_OR) {
                    fprintf(output, "    or $t%d, $t%d, $t%d\n", resultReg, savedLeftReg, rightReg);
                } else {
                    fprintf(stderr, "Error: unsupported binary op for int\n");
                    exit(1);
                }
                tempReg = resultReg + 1;
            }
            break;
        }
        
        case NODE_ARRAY_ACCESS: {
            genExpr(node->data.array_access.index);
            int idxReg = (tempReg > 0) ? tempReg - 1 : 0;
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();
            int resReg  = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_access.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_access.name);
                exit(1);
            }

            // Check if this is an array parameter (passed by reference)
            char* varType = getVarType(node->data.array_access.name);

            /* ===== NEW: GLOBAL VARIABLES SUPPORT ===== */
            if (isGlobalVar(node->data.array_access.name)) {
                // Global array: use label-based addressing
                fprintf(output, "    la    $t%d, %s   # load address of global array %s\n",
                        baseReg, node->data.array_access.name, node->data.array_access.name);
            } else if (varType && strstr(varType, "[]")) {
                // Array parameter: baseOffset contains a pointer, dereference it
                fprintf(output, "    lw    $t%d, %d($sp)   # load pointer to array %s\n",
                        baseReg, baseOffset, node->data.array_access.name);
            } else {
                // Regular local array: calculate address from base offset
                fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                        baseReg, baseOffset, node->data.array_access.name);
            }
            /* ===== END: GLOBAL VARIABLES SUPPORT ===== */

            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addu  $t%d, $t%d, $t%d  # element address\n",
                    addrReg, baseReg, idxReg);

            if (varType && (strcmp(varType, "float") == 0 || strstr(varType, "float[]"))) {
                fprintf(output, "    lwc1 $f0, 0($t%d)     # load float value\n", addrReg);
                tempReg = 0;
            } else {
                fprintf(output, "    lw    $t%d, 0($t%d)     # load int value\n", resReg, addrReg);
                tempReg = resReg + 1;  // Update tempReg to point past the result register
            }
            break;
        }

        case NODE_ARRAY_ASSIGN: {
            if (!isArrayVar(node->data.array_assign.name)) {
                fprintf(stderr, "Error: %s is not an array\n", node->data.array_assign.name);
                exit(1);
            }

            genExpr(node->data.array_assign.index);
            int idxReg = (tempReg > 0) ? tempReg - 1 : 0;

            // Save index in a higher register before evaluating value
            int savedIdxReg = tempReg;  // Use next register for saved index
            fprintf(output, "    move $t%d, $t%d\n", savedIdxReg, idxReg);
            tempReg++;

            genExpr(node->data.array_assign.value);
            int valReg = (tempReg > 0) ? tempReg - 1 : 0;

            idxReg = savedIdxReg;  // Use the saved index
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_assign.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_assign.name);
                exit(1);
            }

            // Check if this is an array parameter (passed by reference)
            char* varType = getVarType(node->data.array_assign.name);

            /* ===== NEW: GLOBAL VARIABLES SUPPORT ===== */
            if (isGlobalVar(node->data.array_assign.name)) {
                // Global array: use label-based addressing
                fprintf(output, "    la    $t%d, %s   # load address of global array %s\n",
                        baseReg, node->data.array_assign.name, node->data.array_assign.name);
            } else if (varType && strstr(varType, "[]")) {
                // Array parameter: baseOffset contains a pointer, dereference it
                fprintf(output, "    lw    $t%d, %d($sp)   # load pointer to array %s\n",
                        baseReg, baseOffset, node->data.array_assign.name);
            } else {
                // Regular local array: calculate address from base offset
                fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                        baseReg, baseOffset, node->data.array_assign.name);
            }
            /* ===== END: GLOBAL VARIABLES SUPPORT ===== */

            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addu  $t%d, $t%d, $t%d  # element address\n",
                    addrReg, baseReg, idxReg);

            if (varType && (strcmp(varType, "float") == 0 || strstr(varType, "float[]"))) {
                fprintf(output, "    swc1 $f0, 0($t%d)     # store float value\n", addrReg);
            } else {
                fprintf(output, "    sw    $t%d, 0($t%d)     # store int value\n", valReg, addrReg);
            }

            tempReg = 0;
            break;
        }

        case NODE_ARRAY_2D_ACCESS: {
            int sizeY = getArray2DSizeY(node->data.array_2d_access.name);
            if (sizeY == -1) {
                fprintf(stderr, "Error: Array %s not found or not 2D\n",
                        node->data.array_2d_access.name);
                exit(1);
            }

            genExpr(node->data.array_2d_access.indexX);
            int idxXReg = (tempReg > 0) ? tempReg - 1 : 0;
            genExpr(node->data.array_2d_access.indexY);
            int idxYReg = (tempReg > 0) ? tempReg - 1 : 0;

            int baseOffset = getVarOffset(node->data.array_2d_access.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n",
                        node->data.array_2d_access.name);
                exit(1);
            }

            int sizeYReg = getNextTemp();
            int tempMultReg = getNextTemp();
            int offsetReg = getNextTemp();
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();
            int resultReg = getNextTemp();

            fprintf(output, "    # 2D array access: %s[i][j]\n", node->data.array_2d_access.name);
            fprintf(output, "    li $t%d, %d           # sizeY = %d\n", sizeYReg, sizeY, sizeY);
            fprintf(output, "    mult $t%d, $t%d       # multiply indexX * sizeY\n", idxXReg, sizeYReg);
            fprintf(output, "    mflo $t%d             # get result from LO register\n", tempMultReg);
            fprintf(output, "    add $t%d, $t%d, $t%d  # (indexX*sizeY + indexY)\n", offsetReg, tempMultReg, idxYReg);
            fprintf(output, "    sll $t%d, $t%d, 2     # multiply by 4\n", offsetReg, offsetReg);
            fprintf(output, "    addiu $t%d, $sp, %d   # base address\n", baseReg, baseOffset);
            fprintf(output, "    addu $t%d, $t%d, $t%d # final address\n", addrReg, baseReg, offsetReg);

            char* type = getVarType(node->data.array_2d_access.name);
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    lwc1 $f0, 0($t%d)     # load float value\n", addrReg);
                tempReg = 0;
            } else {
                fprintf(output, "    lw    $t%d, 0($t%d)     # load int value\n", resultReg, addrReg);
            }

            tempReg = resultReg + 1;
            break;
        }

        case NODE_FUNC_CALL: {
            fprintf(output, "    # Call function: %s (from genExpr)\n", node->data.func_call.name);
            ASTNode* arg = node->data.func_call.args;
            int argNum = 0;

            // Count arguments first
            int totalArgs = 0;
            ASTNode* countArg = arg;
            while (countArg) {
                if (countArg->type == NODE_ARG_LIST) {
                    totalArgs++;
                    countArg = countArg->data.arg_list.next;
                } else {
                    totalArgs++;
                    break;
                }
            }

            // FIX: Evaluate all arguments BEFORE allocating stack space
            // Use unique temp registers or push to stack immediately to avoid overwrites
            int savedRegs[10];  // Track which register holds each argument
            int savedIsFloat[10];

            argNum = 0;
            arg = node->data.func_call.args;
            int nextTempReg = 0;  // Track available temp registers

            while (arg) {
                if (arg->type == NODE_ARG_LIST) {
                    int argIndex = totalArgs - 1 - argNum;

                    // Check if this argument is an array variable
                    if (arg->data.arg_list.expr->type == NODE_VAR &&
                        isArrayVar(arg->data.arg_list.expr->data.name)) {
                        // Pass array by reference (address)
                        int offset = getVarOffset(arg->data.arg_list.expr->data.name);
                        int regToUse = nextTempReg++;
                        fprintf(output, "    addi $t%d, $sp, %d  # Load array address for arg %d\n", regToUse, offset, argIndex);
                        savedRegs[argIndex] = regToUse;
                        savedIsFloat[argIndex] = 0;
                    } else {
                        // Regular scalar argument - evaluate into specific register
                        tempReg = nextTempReg;
                        genExpr(arg->data.arg_list.expr);
                        savedRegs[argIndex] = nextTempReg;
                        savedIsFloat[argIndex] = (arg->data.arg_list.expr->type == NODE_NUM && arg->data.arg_list.expr->data.num.is_float);
                        nextTempReg++;
                        if (nextTempReg > 7) nextTempReg = 7;  // Clamp to available temp regs
                    }

                    argNum++;
                    arg = arg->data.arg_list.next;
                } else {
                    // Last argument (non-list node)
                    int argIndex = 0;

                    // Check if this argument is an array variable
                    if (arg->type == NODE_VAR && isArrayVar(arg->data.name)) {
                        // Pass array by reference (address)
                        int offset = getVarOffset(arg->data.name);
                        int regToUse = nextTempReg++;
                        fprintf(output, "    addi $t%d, $sp, %d  # Load array address for arg %d\n", regToUse, offset, argIndex);
                        savedRegs[argIndex] = regToUse;
                        savedIsFloat[argIndex] = 0;
                    } else {
                        // Regular scalar argument
                        tempReg = nextTempReg;
                        genExpr(arg);
                        savedRegs[argIndex] = nextTempReg;
                        savedIsFloat[argIndex] = (arg->type == NODE_NUM && arg->data.num.is_float);
                    }
                    break;
                }
            }

            // NOW allocate space for arguments on stack
            if (totalArgs > 0) {
                fprintf(output, "    addi $sp, $sp, -%d  # Allocate space for %d arguments\n", totalArgs * 4, totalArgs);
            }

            // Store all evaluated arguments to their stack locations
            for (int i = 0; i < totalArgs; i++) {
                int storeOffset = i * 4;
                if (savedIsFloat[i]) {
                    fprintf(output, "    swc1 $f0, %d($sp)  # Store float argument %d\n", storeOffset, i);
                } else {
                    fprintf(output, "    sw $t%d, %d($sp)  # Store argument %d\n", savedRegs[i], storeOffset, i);
                }
            }

            fprintf(output, "    jal %s\n", node->data.func_call.name);
            fprintf(output, "    move $t0, $v0\n");

            // Deallocate argument space
            if (totalArgs > 0) {
                fprintf(output, "    addi $sp, $sp, %d  # Deallocate argument space\n", totalArgs * 4);
            }

            // FIX: Function result is in $t0, so next temp should be $t1
            tempReg = 1;
            break;
        }

        default:
            break;
    }
}

void genStmt(ASTNode* node) {
    if (!node) {
        DEBUG_PRINT("[DEBUG] genStmt: NULL node\n");
        return;
    }

    DEBUG_PRINT("[DEBUG] genStmt: Processing node type %d\n", node->type);

    switch(node->type) {
        case NODE_DECL: {
            DEBUG_PRINT("[DEBUG] NODE_DECL: var=%s, scope=%d\n",
                    node->data.decl.varName, getCurrentScope());

            // Check if this is a global declaration (scope 0)
            if (getCurrentScope() == 0) {
                // Global variables: skip - they're not allocated on the stack
                DEBUG_PRINT("[DEBUG] Skipping global variable: %s (globals not supported on stack)\n",
                        node->data.decl.varName);
                // Don't add to symbol table - globals aren't tracked in our simple implementation
            } else {
                // Local variable: allocate on stack
                int offset = addVar(node->data.decl.varName, node->data.decl.varType);
                if (offset == -1) {
                    fprintf(stderr, "Error: Variable %s already declared\n", node->data.decl.varName);
                    exit(1);
                }
                fprintf(output, "    # Declared %s at scope %d, offset %d\n",
                        node->data.decl.varName, getCurrentScope(), offset);
            }
            break;
        }

        case NODE_DECL_ASSIGN: {
            int offset = addVar(node->data.declAssign.id, node->data.declAssign.type);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s already declared\n", node->data.declAssign.id);
                exit(1);
            }
            fprintf(output, "    # Declared %s at scope %d, offset %d\n",
                    node->data.declAssign.id, getCurrentScope(), offset);
            tempReg = 0;
            genExpr(node->data.declAssign.expr);
            char* type = getVarType(node->data.declAssign.id);
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    swc1 $f0, %d($sp)\n", offset);
            } else {
                int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                fprintf(output, "    sw $t%d, %d($sp)\n", resultReg, offset);
            }
            tempReg = 0;
            break;
        }

        case NODE_ASSIGN: {
            int offset = getVarOffset(node->data.assign.var);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s not declared\n", node->data.assign.var);
                exit(1);
            }
            genExpr(node->data.assign.value);
            char* type = getVarType(node->data.assign.var);

            /* ===== NEW: GLOBAL VARIABLES SUPPORT ===== */
            if (isGlobalVar(node->data.assign.var)) {
                // Global variable - use label-based addressing
                if (type && strcmp(type, "float") == 0) {
                    fprintf(output, "    swc1 $f0, %s\n", node->data.assign.var);
                } else {
                    int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    sw $t%d, %s\n", resultReg, node->data.assign.var);
                }
            } else {
                // Local variable - use stack-relative addressing
                if (type && strcmp(type, "float") == 0) {
                    fprintf(output, "    swc1 $f0, %d($sp)\n", offset);
                } else {
                    int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    sw $t%d, %d($sp)\n", resultReg, offset);
                }
            }
            /* ===== END: GLOBAL VARIABLES SUPPORT ===== */
            tempReg = 0;
            break;
        }
        
        case NODE_PRINT: {
            tempReg = 0;
            
            // Check if we're printing a string
            int isString = isExprString(node->data.expr);
            
            if (isString) {
                // For strings, generate the expression to load address
                genExpr(node->data.expr);
                int stringReg = tempReg > 0 ? tempReg - 1 : 0;
                fprintf(output, "    # Print string (no newline)\n");
                fprintf(output, "    move $a0, $t%d\n", stringReg);
                fprintf(output, "    li $v0, 4\n");
                fprintf(output, "    syscall\n");
            } else {
                // Non-string case
                genExpr(node->data.expr);

                // Use the helper function to determine if expression is float or boolean
                int isFloat = isExprFloat(node->data.expr);
                int isBool = isExprBool(node->data.expr);

                if (isFloat) {
                    fprintf(output, "    # Print float (no newline)\n");
                    fprintf(output, "    mov.s $f12, $f0\n");
                    fprintf(output, "    li $v0, 2\n");
                    fprintf(output, "    syscall\n");
                } else if (isBool) {
                    /* Print "true" or "false" for booleans instead of 1 or 0 (no newline)*/
                    int printReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    # Print boolean (no newline)\n");
                    
                    static int boolPrintCount = 0;
                    int currentPrint = boolPrintCount++;
                    
                    fprintf(output, "    beqz $t%d, print_false_%d\n", 
                            printReg, currentPrint);
                    
                    /* Print "true" */
                    fprintf(output, "    la $a0, true_str\n");
                    fprintf(output, "    li $v0, 4\n");
                    fprintf(output, "    syscall\n");
                    fprintf(output, "    j print_bool_end_%d\n", currentPrint);
                    
                    /* Print "false" */
                    fprintf(output, "print_false_%d:\n", currentPrint);
                    fprintf(output, "    la $a0, false_str\n");
                    fprintf(output, "    li $v0, 4\n");
                    fprintf(output, "    syscall\n");
                    
                    fprintf(output, "print_bool_end_%d:\n", currentPrint);
                } else {
                    int printReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    # Print integer (no newline)\n");
                    fprintf(output, "    move $a0, $t%d\n", printReg);
                    fprintf(output, "    li $v0, 1\n");
                    fprintf(output, "    syscall\n");
                }
            }
            tempReg = 0;
            break;
        }

        case NODE_PRINTLN: {
            tempReg = 0;
            
            // Check if we're printing a string
            int isString = isExprString(node->data.expr);
            
            if (isString) {
                // For strings, generate the expression to load address
                genExpr(node->data.expr);
                int stringReg = tempReg > 0 ? tempReg - 1 : 0;
                fprintf(output, "    # Print string with newline\n");
                fprintf(output, "    move $a0, $t%d\n", stringReg);
                fprintf(output, "    li $v0, 4\n");
                fprintf(output, "    syscall\n");
            } else {
                // Non-string case
                genExpr(node->data.expr);

                // Use the helper function to determine if expression is float or boolean
                int isFloat = isExprFloat(node->data.expr);
                int isBool = isExprBool(node->data.expr);

                if (isFloat) {
                    fprintf(output, "    # Print float with newline\n");
                    fprintf(output, "    mov.s $f12, $f0\n");
                    fprintf(output, "    li $v0, 2\n");
                    fprintf(output, "    syscall\n");
                } else if (isBool) {
                    /* Print "true" or "false" for booleans instead of 1 or 0*/
                    int printReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    # Print boolean with newline\n");
                    
                    static int boolPrintCount = 0;
                    int currentPrint = boolPrintCount++;
                    
                    fprintf(output, "    beqz $t%d, println_false_%d\n", 
                            printReg, currentPrint);
                    
                    /* Print "true" */
                    fprintf(output, "    la $a0, true_str\n");
                    fprintf(output, "    li $v0, 4\n");
                    fprintf(output, "    syscall\n");
                    fprintf(output, "    j println_bool_end_%d\n", currentPrint);
                    
                    /* Print "false" */
                    fprintf(output, "println_false_%d:\n", currentPrint);
                    fprintf(output, "    la $a0, false_str\n");
                    fprintf(output, "    li $v0, 4\n");
                    fprintf(output, "    syscall\n");
                    
                    fprintf(output, "println_bool_end_%d:\n", currentPrint);
                } else {
                    int printReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    # Print integer with newline\n");
                    fprintf(output, "    move $a0, $t%d\n", printReg);
                    fprintf(output, "    li $v0, 1\n");
                    fprintf(output, "    syscall\n");
                }
            }
            /* Print newline */
            fprintf(output, "    # Print newline\n");
            fprintf(output, "    li $v0, 11\n");
            fprintf(output, "    li $a0, 10\n");
            fprintf(output, "    syscall\n");
            tempReg = 0;
            break;
        }

        case NODE_IF: {
            // Generate a unique label for this if statement
            static int ifCount = 0;
            int currentIf = ifCount++;

            fprintf(output, "    # If statement\n");

            // Generate code for the condition
            tempReg = 0;
            genExpr(node->condition);

            // Get the register that contains the condition result
            int condReg = (tempReg > 0) ? tempReg - 1 : 0;

            // Branch if condition is false (or zero)
            fprintf(output, "    beqz $t%d, else_%d\n", condReg, currentIf);
    
            // Generate code for the 'then' block
            genStmt(node->left);

            // Jump over the else block
            fprintf(output, "    j end_if_%d\n", currentIf);

            // Else label
            fprintf(output, "else_%d:\n", currentIf);

            // Generate code for the 'else' block (if it exists)
            if (node->right) {
                genStmt(node->right);
            }
    
            // End of if statement
            fprintf(output, "end_if_%d:\n", currentIf);
            tempReg = 0;
            break;
        }
            
        case NODE_STMT_LIST:
            genStmt(node->data.stmtlist.stmt);
            genStmt(node->data.stmtlist.next);
            break;
            
        case NODE_ARRAY_DECL: {
            DEBUG_PRINT("[DEBUG] NODE_ARRAY_DECL: array=%s, size=%d, type=%s, scope=%d\n",
                    node->data.array_decl.name,
                    node->data.array_decl.size,
                    node->data.array_decl.type ? node->data.array_decl.type : "NULL",
                    getCurrentScope());

            // Check if this is a global declaration (scope 0)
            if (getCurrentScope() == 0) {
                // Global arrays: skip - they're not allocated on the stack
                DEBUG_PRINT("[DEBUG] Skipping global array: %s (globals not supported on stack)\n",
                        node->data.array_decl.name);
                // Don't add to symbol table - globals aren't tracked in our simple implementation
            } else {
                // Local array: allocate on stack
                DEBUG_PRINT("[DEBUG] About to call addArrayVar...\n");
                int offset = addArrayVar(node->data.array_decl.name, node->data.array_decl.size, node->data.array_decl.type);
                DEBUG_PRINT("[DEBUG] addArrayVar returned offset: %d\n", offset);
                if (offset == -1) {
                    fprintf(stderr, "Error: Array %s already declared\n", node->data.array_decl.name);
                    exit(1);
                }
                fprintf(output, "    # Declared array %s of size %d at scope %d, offset %d\n",
                        node->data.array_decl.name, node->data.array_decl.size, getCurrentScope(), offset);
            }
            break;
        }

        case NODE_ARRAY_ASSIGN: {
            if (!isArrayVar(node->data.array_assign.name)) {
                fprintf(stderr, "Error: %s is not an array\n", node->data.array_assign.name);
                exit(1);
            }

            genExpr(node->data.array_assign.index);
            int idxReg = (tempReg > 0) ? tempReg - 1 : 0;

            // Save index in a higher register before evaluating value
            int savedIdxReg = tempReg;  // Use next register for saved index
            fprintf(output, "    move $t%d, $t%d\n", savedIdxReg, idxReg);
            tempReg++;

            genExpr(node->data.array_assign.value);
            int valReg = (tempReg > 0) ? tempReg - 1 : 0;

            idxReg = savedIdxReg;  // Use the saved index
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_assign.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_assign.name);
                exit(1);
            }

            // Check if this is an array parameter (passed by reference)
            char* varType = getVarType(node->data.array_assign.name);

            /* ===== NEW: GLOBAL VARIABLES SUPPORT ===== */
            if (isGlobalVar(node->data.array_assign.name)) {
                // Global array: use label-based addressing
                fprintf(output, "    la    $t%d, %s   # load address of global array %s\n",
                        baseReg, node->data.array_assign.name, node->data.array_assign.name);
            } else if (varType && strstr(varType, "[]")) {
                // Array parameter: baseOffset contains a pointer, dereference it
                fprintf(output, "    lw    $t%d, %d($sp)   # load pointer to array %s\n",
                        baseReg, baseOffset, node->data.array_assign.name);
            } else {
                // Regular local array: calculate address from base offset
                fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                        baseReg, baseOffset, node->data.array_assign.name);
            }
            /* ===== END: GLOBAL VARIABLES SUPPORT ===== */

            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addu  $t%d, $t%d, $t%d  # element address\n",
                    addrReg, baseReg, idxReg);
            char* type = getVarType(node->data.array_assign.name);
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    swc1 $f0, 0($t%d)     # store float value\n", addrReg);
            } else {
                fprintf(output, "    sw    $t%d, 0($t%d)     # store int value\n", valReg, addrReg);
            }

            tempReg = 0;
            break;
        }

        case NODE_ARRAY_DECL_ASSIGN: { 
            ASTNode* init = node->data.array_decl_assign.initList;
            int size = node->data.array_decl_assign.size;

            ASTNode* expressions[100];
            int count = 0;

            ASTNode* cur = init;
            while (cur != NULL && count < 100) {
                if (cur->type == NODE_STMT_LIST) {
                    ASTNode* expr = cur->data.stmtlist.stmt;
                    if (expr != NULL) {
                        expressions[count++] = expr;
                    }
                    cur = cur->data.stmtlist.next;
                } else {
                    expressions[count++] = cur;
                    break;
                }
            }

            if (size == 0) {
                size = count;
            }

            int offset = addArrayVar(node->data.array_decl_assign.name, size, node->data.array_decl_assign.type);
            if (offset == -1) { 
                fprintf(stderr, "Error: Variable %s already declared\n", node->data.array_decl_assign.name); 
                exit(1); 
            }

            for (int i = 0; i < count && i < size; i++) {
                tempReg = 0;
                genExpr(expressions[count - 1 - i]);
                fprintf(output, "    # Storing value at array position %d\n", i);
                fprintf(output, "    addiu $t1, $sp, %d      # base address\n", offset);
                fprintf(output, "    li    $t2, %d           # offset = %d * 4\n", i * 4, i);
                fprintf(output, "    addu  $t3, $t1, $t2    # element address\n");
                char* type = node->data.array_decl_assign.type;
                if (type && strcmp(type, "float") == 0) {
                    fprintf(output, "    swc1 $f0, 0($t3)      # store float value\n");
                } else {
                    fprintf(output, "    sw    $t0, 0($t3)      # store int value\n");
                }
            }
    
            tempReg = 0;
            break;
        }

        case NODE_ARRAY_2D_DECL: {
            int offset = addArray2DVar(node->data.array_2d_decl.name, 
                                      node->data.array_2d_decl.sizeX, 
                                      node->data.array_2d_decl.sizeY, 
                                      node->data.array_2d_decl.type);
            if (offset == -1) {
                fprintf(stderr, "Error: Array %s already declared\n", node->data.array_2d_decl.name);
                exit(1);
            }
            fprintf(output, "    # Declared array %s of size %d x %d at scope %d, offset %d\n", 
                    node->data.array_2d_decl.name, 
                    node->data.array_2d_decl.sizeX, 
                    node->data.array_2d_decl.sizeY, 
                    getCurrentScope(), 
                    offset);
            break;
        }

        case NODE_ARRAY_2D_ELEM_ASSIGN: {
            if (!is2DArrayVar(node->data.array_2d_elem_assign.name)) {
                fprintf(stderr, "Error: %s is not a 2D array\n", node->data.array_2d_elem_assign.name);
                exit(1);
            }

            int sizeY = getArray2DSizeY(node->data.array_2d_elem_assign.name);
            tempReg = 0;
    
            genExpr(node->data.array_2d_elem_assign.indexX);
            int idxXReg = 0;
            genExpr(node->data.array_2d_elem_assign.indexY);
            int idxYReg = 1;
            genExpr(node->data.array_2d_elem_assign.value);
            int valReg = 2;

            int baseOffset = getVarOffset(node->data.array_2d_elem_assign.name);

            fprintf(output, "    # 2D array assignment: %s[i][j] = value\n", 
                    node->data.array_2d_elem_assign.name);
            fprintf(output, "    li $t3, %d            # sizeY = %d\n", sizeY, sizeY);
            fprintf(output, "    mult $t%d, $t3        # indexX * sizeY\n", idxXReg);
            fprintf(output, "    mflo $t4              # get result\n");
            fprintf(output, "    add $t4, $t4, $t%d    # (indexX*sizeY + indexY)\n", idxYReg);
            fprintf(output, "    sll $t4, $t4, 2       # multiply by 4\n");
            fprintf(output, "    addiu $t5, $sp, %d    # base address\n", baseOffset);
            fprintf(output, "    addu $t6, $t5, $t4    # final address\n");

            char* type = getVarType(node->data.array_2d_elem_assign.name);
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    swc1 $f0, 0($t6)       # store float value\n");
            } else {
                fprintf(output, "    sw $t%d, 0($t6)       # store int value\n", valReg);
            }

            tempReg = 0;
            break;
        }

        case NODE_FUNC_DECL: {
            DEBUG_PRINT("[DEBUG] Processing function: %s\n", node->data.func_decl.name);
            fprintf(output, "\n%s:\n", node->data.func_decl.name);
            fprintf(output, "    # Function: %s (Scope Level: %d)\n",
                    node->data.func_decl.name, getCurrentScope() + 1);

            // ✅ FIXED: Enter new scope instead of resetting symbol table
            enterScope();

            addVar("$ra_slot", "int");

            // CALLING CONVENTION:
            // Parameters are passed in caller's stack frame at: caller_sp + 4 + (paramNum * 4)
            // After prologue, we allocate local variable space below $ra

            currentFunctionPrologueSize = 0;  // Reset prologue size tracking

            // FIX: Use smaller, more reasonable stack allocation
            // Main needs more space for its local arrays, but other functions should use minimal space
            if (strcmp(node->data.func_decl.name, "main") == 0) {
                fprintf(output, "    addi $sp, $sp, -200\n");  // Reduced from 400
                currentFunctionPrologueSize += 200;
            } else {
                // For other functions, allocate minimal space (just 20 bytes for a few local vars)
                // This prevents stack overflow in recursive functions
                fprintf(output, "    addi $sp, $sp, -20\n");  // Reduced from 100
                currentFunctionPrologueSize += 20;
            }

            fprintf(output, "    addi $sp, $sp, -4\n");
            fprintf(output, "    sw $ra, 0($sp)\n");
            currentFunctionPrologueSize += 4;

            // Load parameters
            // Parameters are at: $sp + currentFunctionPrologueSize + (paramNum * 4)
            // This accounts for the space allocated in the prologue
            ASTNode* param = node->data.func_decl.params;

            DEBUG_PRINT("[DEBUG] Loading parameters for function %s\n", node->data.func_decl.name);

            // First, flatten the parameter list into an array (using a helper recursive function)
            ASTNode* paramList[20];  // Max 20 params
            int totalParams = 0;

            // Recursive helper to extract all NODE_PARAM nodes from the nested structure
            void flattenParams(ASTNode* p) {
                if (!p) return;
                DEBUG_PRINT("[DEBUG] flattenParams: node type %d\n", p->type);

                if (p->type == NODE_PARAM) {
                    // Base case: found an actual parameter
                    paramList[totalParams++] = p;
                    DEBUG_PRINT("[DEBUG] Added param, total: %d\n", totalParams);
                } else if (p->type == NODE_PARAM_LIST) {
                    // Recursive case: traverse the list
                    flattenParams(p->data.param_list.param);  // Process the param field (could be another list or a param)
                    flattenParams(p->data.param_list.next);   // Process the next in the list
                }
            }

            flattenParams(param);
            DEBUG_PRINT("[DEBUG] Found %d parameters\n", totalParams);

            // Process parameters in order
            for (int i = 0; i < totalParams; i++) {
                ASTNode* paramNode = paramList[i];
                DEBUG_PRINT("[DEBUG] Processing param %d\n", i);

                if (paramNode->type == NODE_PARAM) {
                    char* paramName = paramNode->data.param.name;
                    char* paramType = paramNode->data.param.type;
                    DEBUG_PRINT("[DEBUG] Processing parameter: %s (type: %s)\n", paramName, paramType);
                    int offset = addVar(paramName, paramType);

                    // Parameters are stored by caller, but after prologue we've moved $sp
                    // So parameters are now at: $sp + currentFunctionPrologueSize + (i * 4)
                    int paramLocation = currentFunctionPrologueSize + (i * 4);

                    // Check if parameter is an array type
                    if (strstr(paramType, "[]")) {
                        // Array parameter: load pointer (address) from caller's frame
                        fprintf(output, "    lw $t0, %d($sp)  # Load array address for param %s\n",
                                paramLocation, paramName);
                        // Store the pointer in the variable's location for access later
                        fprintf(output, "    sw $t0, %d($sp)  # Store array pointer at local offset %d\n",
                                offset, offset);
                    } else if (strcmp(paramType, "float") == 0) {
                        fprintf(output, "    lwc1 $f0, %d($sp)  # Load float param\n", paramLocation);
                        fprintf(output, "    swc1 $f0, %d($sp)  # Store param %s (float) at local offset %d\n",
                                offset, paramName, offset);
                    } else {
                        fprintf(output, "    lw $t0, %d($sp)  # Load int param\n", paramLocation);
                        fprintf(output, "    sw $t0, %d($sp)  # Store param %s (int) at local offset %d\n",
                                offset, paramName, offset);
                    }
                } else {
                    DEBUG_PRINT("[DEBUG] ERROR: paramNode type is not NODE_PARAM: %d\n", paramNode->type);
                }
            }

            // Generate function body
            DEBUG_PRINT("[DEBUG] Generating function body for %s\n", node->data.func_decl.name);
            genStmt(node->data.func_decl.body);
            DEBUG_PRINT("[DEBUG] Finished generating function body for %s\n", node->data.func_decl.name);

            // Generate implicit return for functions that don't have explicit return
            // (This handles void functions and functions that fall through)
            // FIX: Special handling for main - exit instead of return
            if (strcmp(node->data.func_decl.name, "main") == 0) {
                fprintf(output, "    # Exit from main\n");
                fprintf(output, "    li $v0, 10     # Syscall 10 = exit\n");
                fprintf(output, "    syscall\n");
            } else {
                fprintf(output, "    # Implicit return from function\n");
                fprintf(output, "    lw $ra, 0($sp)\n");
                fprintf(output, "    addi $sp, $sp, %d\n", currentFunctionPrologueSize);  // Deallocate ALL prologue space
                fprintf(output, "    jr $ra\n");
            }

            // ✅ FIXED: Exit scope after function ends
            exitScope();
            break;
        }

        case NODE_BLOCK: {
            genStmt(node->data.block.stmts);
            break;
        }
        
        case NODE_RETURN: {
            genExpr(node->data.ret.value);
            char* type = NULL;
            if (currentFunctionType) type = currentFunctionType;
            if (!type && node->data.ret.value->type == NODE_NUM && node->data.ret.value->data.num.is_float) {
                type = "float";
            }
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    mov.s $f0, $f0\n");  // Float return value
            } else {
                // FIX: Use the correct register that contains the result
                int resultReg = (tempReg > 0) ? tempReg - 1 : 0;
                fprintf(output, "    move $v0, $t%d\n", resultReg);  // Integer return value
            }
            fprintf(output, "    lw $ra, 0($sp)\n");
            fprintf(output, "    addi $sp, $sp, %d\n", currentFunctionPrologueSize);  // Deallocate ALL prologue space
            fprintf(output, "    jr $ra\n");
            break;
        }
        
        case NODE_FUNC_CALL: {
            fprintf(output, "    # Call function: %s (from genStmt)\n", node->data.func_call.name);
            ASTNode* arg = node->data.func_call.args;
            int argNum = 0;

            // Count arguments first
            int totalArgs = 0;
            ASTNode* countArg = arg;
            while (countArg) {
                if (countArg->type == NODE_ARG_LIST) {
                    totalArgs++;
                    countArg = countArg->data.arg_list.next;
                } else {
                    totalArgs++;
                    break;
                }
            }

            // FIX: Evaluate all arguments BEFORE allocating stack space
            // Use unique temp registers or push to stack immediately to avoid overwrites
            int savedRegs[10];  // Track which register holds each argument
            int savedIsFloat[10];

            argNum = 0;
            arg = node->data.func_call.args;
            int nextTempReg = 0;  // Track available temp registers

            while (arg) {
                if (arg->type == NODE_ARG_LIST) {
                    int argIndex = totalArgs - 1 - argNum;

                    // Check if this argument is an array variable
                    if (arg->data.arg_list.expr->type == NODE_VAR &&
                        isArrayVar(arg->data.arg_list.expr->data.name)) {
                        // Pass array by reference (address)
                        int offset = getVarOffset(arg->data.arg_list.expr->data.name);
                        int regToUse = nextTempReg++;
                        fprintf(output, "    addi $t%d, $sp, %d  # Load array address for arg %d\n", regToUse, offset, argIndex);
                        savedRegs[argIndex] = regToUse;
                        savedIsFloat[argIndex] = 0;
                    } else {
                        // Regular scalar argument - evaluate into specific register
                        tempReg = nextTempReg;
                        genExpr(arg->data.arg_list.expr);
                        savedRegs[argIndex] = nextTempReg;
                        savedIsFloat[argIndex] = (arg->data.arg_list.expr->type == NODE_NUM && arg->data.arg_list.expr->data.num.is_float);
                        nextTempReg++;
                        if (nextTempReg > 7) nextTempReg = 7;  // Clamp to available temp regs
                    }

                    argNum++;
                    arg = arg->data.arg_list.next;
                } else {
                    // Last argument (non-list node)
                    int argIndex = 0;

                    // Check if this argument is an array variable
                    if (arg->type == NODE_VAR && isArrayVar(arg->data.name)) {
                        // Pass array by reference (address)
                        int offset = getVarOffset(arg->data.name);
                        int regToUse = nextTempReg++;
                        fprintf(output, "    addi $t%d, $sp, %d  # Load array address for arg %d\n", regToUse, offset, argIndex);
                        savedRegs[argIndex] = regToUse;
                        savedIsFloat[argIndex] = 0;
                    } else {
                        // Regular scalar argument
                        tempReg = nextTempReg;
                        genExpr(arg);
                        savedRegs[argIndex] = nextTempReg;
                        savedIsFloat[argIndex] = (arg->type == NODE_NUM && arg->data.num.is_float);
                    }
                    break;
                }
            }

            // NOW allocate space for arguments on stack
            if (totalArgs > 0) {
                fprintf(output, "    addi $sp, $sp, -%d  # Allocate space for %d arguments\n", totalArgs * 4, totalArgs);
            }

            // Store all evaluated arguments to their stack locations
            for (int i = 0; i < totalArgs; i++) {
                int storeOffset = i * 4;
                if (savedIsFloat[i]) {
                    fprintf(output, "    swc1 $f0, %d($sp)  # Store float argument %d\n", storeOffset, i);
                } else {
                    fprintf(output, "    sw $t%d, %d($sp)  # Store argument %d\n", savedRegs[i], storeOffset, i);
                }
            }

            fprintf(output, "    jal %s\n", node->data.func_call.name);
            fprintf(output, "    move $t0, $v0\n");

            // Deallocate argument space
            if (totalArgs > 0) {
                fprintf(output, "    addi $sp, $sp, %d  # Deallocate argument space\n", totalArgs * 4);
            }

            // FIX: Function result is in $t0, so next temp should be $t1
            tempReg = 1;
            break;
        }
        case NODE_SWITCH: {
            fprintf(output, "\n    # Switch Statement\n");

            /* Evaluate switch expression and store in switchReg */
            tempReg = 0;
            genExpr(node->data.switch_stmt.expr);
            int switchReg = tempReg > 0 ? tempReg - 1 : 0;
            static int switchCount = 0;
            int currentSwitch = switchCount++;
            char endLabel[32];
            sprintf(endLabel, "end_switch_%d", currentSwitch);

            /* Flatten the nested case list structure */
            ASTNode* cases[100];
            int caseCount = 0;
            int hasDefault = 0;
            collectSwitchCases(node->data.switch_stmt.cases, cases, &caseCount, &hasDefault, 100);

            /* First pass: generate all comparisons for cases */
            for(int i = 0; i < caseCount; i++) {
                if(cases[i]->type == NODE_CASE) {
                    int caseValue = cases[i]->data.case_stmt.value;
                    fprintf(output, "    li $t7, %d\n", caseValue);
                    fprintf(output, "    beq $t%d, $t7, case_%d_%d\n", switchReg, currentSwitch, caseValue);
                }
            }

            /* Jump to default or end if no case matched */
            if(hasDefault) {
                fprintf(output, "    j default_%d\n", currentSwitch);
            } else {
                fprintf(output, "    j %s\n", endLabel);
            }

            /* Second pass: generate case labels and code */
            for(int i = 0; i < caseCount; i++) {
                if(cases[i]->type == NODE_CASE) {
                    int caseValue = cases[i]->data.case_stmt.value;
                    fprintf(output, "case_%d_%d:\n", currentSwitch, caseValue);
                    genStmt(cases[i]->data.case_stmt.stmts);
                    fprintf(output, "    j %s\n", endLabel);
                } else if(cases[i]->type == NODE_DEFAULT_CASE) {
                    fprintf(output, "default_%d:\n", currentSwitch);
                    genStmt(cases[i]->data.default_case.stmts);
                    fprintf(output, "    j %s\n", endLabel);
                }
            }

            /* End label */
            fprintf(output, "%s:\n", endLabel);
            fprintf(output, "    # End of switch statement\n");

            tempReg = 0;
            break;
        }
        case NODE_BREAK:{
            /*Break statement -jump to end of switch/loop*/
            fprintf(output, "                      #Break statement\n");
            /*Requires tracking of current break target*/
            /* for now, assume its handled by parent*/
            break;
        }
        /* ===== NEW: WHEN LOOP FEATURE ===== */
        case NODE_WHEN_STMT: {
            fprintf(output, "\n    # WHEN LOOP - if-else chain that loops\n");
            fprintf(output, "    # 1. Check primary: if true, execute & exit\n");
            fprintf(output, "    # 2. Else check OR branches: if true, execute & loop\n");
            fprintf(output, "    # 3. Else execute else block & loop\n");
            
            static int whenCount = 0;
            int currentWhen = whenCount++;
            char loopStartLabel[32], loopEndLabel[32];
            sprintf(loopStartLabel, "when_start_%d", currentWhen);
            sprintf(loopEndLabel, "when_end_%d", currentWhen);
            
            // Push this when loop's end label onto stack for break-when statements
            char* endLabelCopy = malloc(32);
            strcpy(endLabelCopy, loopEndLabel);
            pushWhenLabel(endLabelCopy);
            
            // Loop start label
            fprintf(output, "%s:\n", loopStartLabel);
            
            // Step 1: Evaluate primary condition
            fprintf(output, "    # Step 1: Check primary condition\n");
            tempReg = 0;
            genExpr(node->data.when_stmt.primaryCond);
            int condReg = tempReg > 0 ? tempReg - 1 : 0;
            
            // Generate label for skipping primary block if condition is false
            char skipPrimaryLabel[32];
            sprintf(skipPrimaryLabel, "skip_primary_%d", currentWhen);
            
            // If primary condition is FALSE, skip to OR branches
            fprintf(output, "    beqz $t%d, %s      # If false, check OR branches\n", 
                    condReg, skipPrimaryLabel);
            
            // Execute primary block (primary condition is true)
            fprintf(output, "    # Primary condition is true - execute block and exit\n");
            genStmt(node->data.when_stmt.primaryBlock);
            
            // Jump to end (exit the loop)
            fprintf(output, "    j %s              # Exit when loop\n", loopEndLabel);
            
            // Label for when primary is false - check OR branches
            fprintf(output, "%s:\n", skipPrimaryLabel);
            fprintf(output, "    # Step 2: Check OR branches\n");
            
            if (node->data.when_stmt.orBranches) {
                // Process each OR branch
                ASTNode* orCurrent = node->data.when_stmt.orBranches;
                int orBranchNum = 0;
                char skipAllOrLabel[32];
                sprintf(skipAllOrLabel, "skip_all_or_%d", currentWhen);
                
                while (orCurrent) {
                    if (orCurrent->type == NODE_WHEN_OR_LIST) {
                        if (orCurrent->data.when_or_list.branch &&
                            orCurrent->data.when_or_list.branch->type == NODE_WHEN_OR_BRANCH) {
                            
                            ASTNode* branch = orCurrent->data.when_or_list.branch;
                            char skipOrLabel[32];
                            sprintf(skipOrLabel, "skip_or_%d_%d", currentWhen, orBranchNum);
                            
                            // Evaluate branch condition
                            fprintf(output, "    # Check OR branch %d condition\n", orBranchNum);
                            tempReg = 0;
                            genExpr(branch->data.when_or_branch.condition);
                            int branchCondReg = tempReg > 0 ? tempReg - 1 : 0;
                            
                            // If condition is false, skip to next OR branch
                            fprintf(output, "    beqz $t%d, %s   # Skip if false\n", 
                                    branchCondReg, skipOrLabel);
                            
                            // Execute branch block
                            fprintf(output, "    # OR branch %d is true - execute block and loop\n", orBranchNum);
                            genStmt(branch->data.when_or_branch.block);
                            
                            // Jump back to loop start
                            fprintf(output, "    j %s              # Loop back to start\n", loopStartLabel);
                            
                            fprintf(output, "%s:\n", skipOrLabel);
                            orBranchNum++;
                        }
                        orCurrent = orCurrent->data.when_or_list.next;
                    } else if (orCurrent->type == NODE_WHEN_OR_BRANCH) {
                        // Single branch (shouldn't normally happen)
                        char skipOrLabel[32];
                        sprintf(skipOrLabel, "skip_or_%d_%d", currentWhen, orBranchNum);
                        
                        tempReg = 0;
                        genExpr(orCurrent->data.when_or_branch.condition);
                        int branchCondReg = tempReg > 0 ? tempReg - 1 : 0;
                        
                        fprintf(output, "    beqz $t%d, %s\n", branchCondReg, skipOrLabel);
                        genStmt(orCurrent->data.when_or_branch.block);
                        fprintf(output, "    j %s\n", loopStartLabel);
                        fprintf(output, "%s:\n", skipOrLabel);
                        break;
                    } else {
                        break;
                    }
                }
            }
            
            // Step 3: No OR branch matched, execute else if present
            fprintf(output, "    # Step 3: No OR branch matched - execute else block\n");
            if (node->data.when_stmt.elseBlock) {
                genStmt(node->data.when_stmt.elseBlock);
            }
            
            // Loop back to start (after else)
            fprintf(output, "    j %s              # Loop back to start\n", loopStartLabel);
            
            // Loop end label
            fprintf(output, "%s:\n", loopEndLabel);
            fprintf(output, "    # End of when loop\n");
            
            // Pop the when label from stack
            popWhenLabel();
            
            tempReg = 0;
            break;
        }
        
        case NODE_WHEN_OR_LIST: {
            // Process when-or branches
            ASTNode* current = node;
            while (current) {
                if (current->type == NODE_WHEN_OR_LIST) {
                    if (current->data.when_or_list.branch &&
                        current->data.when_or_list.branch->type == NODE_WHEN_OR_BRANCH) {
                        
                        ASTNode* branch = current->data.when_or_list.branch;
                        static int orBranchCount = 0;
                        int branchId = orBranchCount++;
                        char skipLabel[32];
                        sprintf(skipLabel, "skip_or_%d", branchId);
                        
                        // Evaluate branch condition
                        fprintf(output, "    # Check OR branch condition\n");
                        tempReg = 0;
                        genExpr(branch->data.when_or_branch.condition);
                        int branchCondReg = tempReg > 0 ? tempReg - 1 : 0;
                        
                        // If condition is false, skip this branch
                        fprintf(output, "    beqz $t%d, %s   # Skip if false\n", 
                                branchCondReg, skipLabel);
                        
                        // Execute branch block
                        fprintf(output, "    # Execute OR branch block\n");
                        genStmt(branch->data.when_or_branch.block);
                        
                        fprintf(output, "%s:\n", skipLabel);
                    }
                    current = current->data.when_or_list.next;
                } else if (current->type == NODE_WHEN_OR_BRANCH) {
                    // Single branch
                    static int orBranchCount = 0;
                    int branchId = orBranchCount++;
                    char skipLabel[32];
                    sprintf(skipLabel, "skip_or_%d", branchId);
                    
                    tempReg = 0;
                    genExpr(current->data.when_or_branch.condition);
                    int branchCondReg = tempReg > 0 ? tempReg - 1 : 0;
                    
                    fprintf(output, "    beqz $t%d, %s   # Skip if false\n", 
                            branchCondReg, skipLabel);
                    genStmt(current->data.when_or_branch.block);
                    fprintf(output, "%s:\n", skipLabel);
                    break;
                } else {
                    break;
                }
            }
            tempReg = 0;
            break;
        }
        
        case NODE_BREAK_WHEN: {
            fprintf(output, "    # BREAK WHEN - exit when loop if condition is true\n");
            tempReg = 0;
            genExpr(node->data.break_when.expr);
            int exprReg = tempReg > 0 ? tempReg - 1 : 0;
            
            char* whenEndLabel = currentWhenLabel();
            if (whenEndLabel) {
                // If condition is true, jump to when loop end
                fprintf(output, "    bnez $t%d, %s     # If true, exit when loop\n", 
                        exprReg, whenEndLabel);
            } else {
                fprintf(output, "    # Warning: break when outside of when loop\n");
            }
            tempReg = 0;
            break;
        }
        /* ===== END: WHEN LOOP FEATURE ===== */
        
        case NODE_WHILE_STMT: {
            fprintf(output, "\n    # WHILE LOOP - standard while (condition)\n");
            
            static int whileCount = 0;
            int currentWhile = whileCount++;
            char loopStartLabel[32], loopEndLabel[32];
            sprintf(loopStartLabel, "while_start_%d", currentWhile);
            sprintf(loopEndLabel, "while_end_%d", currentWhile);
            
            // Loop start label
            fprintf(output, "%s:\n", loopStartLabel);
            
            // Evaluate condition
            fprintf(output, "    # Evaluate while condition\n");
            tempReg = 0;
            genExpr(node->data.while_stmt.condition);
            int condReg = tempReg > 0 ? tempReg - 1 : 0;
            
            // If condition is FALSE, exit loop (beqz = branch if equal to zero)
            fprintf(output, "    beqz $t%d, %s      # If false, exit loop\n", 
                    condReg, loopEndLabel);
            
            // Execute loop body
            fprintf(output, "    # While condition is true - execute body\n");
            genStmt(node->data.while_stmt.block);
            
            // Jump back to loop start
            fprintf(output, "    j %s              # Loop back to start\n", loopStartLabel);
            
            // Loop end label
            fprintf(output, "%s:\n", loopEndLabel);
            fprintf(output, "    # End of while loop\n");
            
            tempReg = 0;
            break;
        }
        

        
        default:
            break;
    }
}


// Helper function to collect all string literals from AST
void collectStrings(ASTNode* node) {
    if (!node) return;
    
    switch(node->type) {
        case NODE_STRING:
            addString(node->data.string_literal.value);
            break;
        case NODE_BINOP:
            collectStrings(node->data.binop.left);
            collectStrings(node->data.binop.right);
            break;
        case NODE_UNARYOP:
            collectStrings(node->data.unaryop.operand);
            break;
        case NODE_PRINT:
            collectStrings(node->data.expr);
            break;
        case NODE_PRINTLN:
            collectStrings(node->data.expr);
            break;
        case NODE_ASSIGN:
            collectStrings(node->data.assign.value);
            break;
        case NODE_DECL_ASSIGN:
            collectStrings(node->data.declAssign.expr);
            break;
        case NODE_ARRAY_ASSIGN:
            collectStrings(node->data.array_assign.index);
            collectStrings(node->data.array_assign.value);
            break;
        case NODE_ARRAY_ACCESS:
            collectStrings(node->data.array_access.index);
            break;
        case NODE_ARRAY_DECL_ASSIGN:
            collectStrings(node->data.array_decl_assign.initList);
            break;
        case NODE_ARRAY_2D_ELEM_ASSIGN:
            collectStrings(node->data.array_2d_elem_assign.indexX);
            collectStrings(node->data.array_2d_elem_assign.indexY);
            collectStrings(node->data.array_2d_elem_assign.value);
            break;
        case NODE_IF:
            collectStrings(node->condition);
            collectStrings(node->left);
            collectStrings(node->right);
            break;
        case NODE_SWITCH: {
            collectStrings(node->data.switch_stmt.expr);
            collectStrings(node->data.switch_stmt.cases);
            break;
        }
        case NODE_CASE:
            collectStrings(node->data.case_stmt.stmts);
            break;
        case NODE_DEFAULT_CASE:
            collectStrings(node->data.default_case.stmts);
            break;
        case NODE_CASE_LIST:
            collectStrings(node->data.case_list.case_item);
            collectStrings(node->data.case_list.next);
            break;
        case NODE_WHILE_STMT:
            collectStrings(node->data.while_stmt.condition);
            collectStrings(node->data.while_stmt.block);
            break;
        case NODE_WHEN_STMT:
            collectStrings(node->data.when_stmt.primaryCond);
            collectStrings(node->data.when_stmt.primaryBlock);
            collectStrings(node->data.when_stmt.orBranches);
            collectStrings(node->data.when_stmt.elseBlock);
            break;
        case NODE_WHEN_OR_LIST:
            if (node->data.when_or_list.branch) {
                collectStrings(node->data.when_or_list.branch);
            }
            if (node->data.when_or_list.next) {
                collectStrings(node->data.when_or_list.next);
            }
            break;
        case NODE_WHEN_OR_BRANCH:
            collectStrings(node->data.when_or_branch.condition);
            collectStrings(node->data.when_or_branch.block);
            break;
        case NODE_RETURN:
            collectStrings(node->data.ret.value);
            break;
        case NODE_FUNC_CALL: {
            ASTNode* arg = node->data.func_call.args;
            while (arg) {
                if (arg->type == NODE_ARG_LIST) {
                    collectStrings(arg->data.arg_list.expr);
                    arg = arg->data.arg_list.next;
                } else {
                    collectStrings(arg);
                    break;
                }
            }
            break;
        }
        case NODE_STMT_LIST:
            collectStrings(node->data.stmtlist.stmt);
            collectStrings(node->data.stmtlist.next);
            break;
        case NODE_BLOCK:
            collectStrings(node->data.block.stmts);
            break;
        case NODE_EXPR_LIST:
            collectStrings(node->data.list.expr);
            collectStrings(node->data.list.next);
            break;
        case NODE_FUNC_DECL:
            collectStrings(node->data.func_decl.body);
            break;
        case NODE_BREAK_WHEN:
            collectStrings(node->data.break_when.expr);
            break;
        default:
            break;
    }
}

/* ===== NEW: GLOBAL VARIABLES SUPPORT ===== */
// Helper function to collect and generate global variable declarations
void collectGlobalVars(ASTNode* node) {
    if (!node) return;

    switch(node->type) {
        case NODE_DECL:
            // Global variable declaration at scope 0
            if (getCurrentScope() == 0) {
                DEBUG_PRINT("[DEBUG] Found global variable: %s\n", node->data.decl.varName);
                fprintf(output, "%s: .word 0\n", node->data.decl.varName);
                addVar(node->data.decl.varName, node->data.decl.varType);
            }
            break;

        case NODE_DECL_ASSIGN:
            // Global variable with initialization at scope 0
            if (getCurrentScope() == 0) {
                DEBUG_PRINT("[DEBUG] Found global variable with init: %s\n", node->data.declAssign.id);
                // For now, initialize to 0 (proper initialization would require constant folding)
                fprintf(output, "%s: .word 0\n", node->data.declAssign.id);
                addVar(node->data.declAssign.id, node->data.declAssign.type);
            }
            break;

        case NODE_ARRAY_DECL:
            // Global array declaration at scope 0
            if (getCurrentScope() == 0) {
                int size = node->data.array_decl.size;
                DEBUG_PRINT("[DEBUG] Found global array: %s[%d]\n", node->data.array_decl.name, size);
                fprintf(output, "%s: .space %d\n", node->data.array_decl.name, size * 4);
                addArrayVar(node->data.array_decl.name, size, node->data.array_decl.type);
            }
            break;

        case NODE_STMT_LIST:
            // Process statement list, but stop at function declarations
            if (node->data.stmtlist.stmt && node->data.stmtlist.stmt->type != NODE_FUNC_DECL) {
                collectGlobalVars(node->data.stmtlist.stmt);
            }
            if (node->data.stmtlist.next) {
                collectGlobalVars(node->data.stmtlist.next);
            }
            break;

        default:
            break;
    }
}
/* ===== END: GLOBAL VARIABLES SUPPORT ===== */

// Helper function to collect all global function names
void collectGlobalFunctions(ASTNode* node, char** funcNames, int* funcCount) {
    if (!node || !funcNames || !funcCount) return;

    switch(node->type) {
        case NODE_FUNC_DECL: {
            if (*funcCount < 100) {  // Prevent overflow
                funcNames[*funcCount] = node->data.func_decl.name;
                (*funcCount)++;
            }
            break;
        }

        case NODE_STMT_LIST:
            collectGlobalFunctions(node->data.stmtlist.stmt, funcNames, funcCount);
            collectGlobalFunctions(node->data.stmtlist.next, funcNames, funcCount);
            break;

        default:
            break;
    }
}

void generateMIPS(ASTNode* root, const char* filename) {
    DEBUG_PRINT("[DEBUG] ===== Starting MIPS Code Generation =====\n");
    output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "Cannot open output file %s\n", filename);
        exit(1);
    }

    DEBUG_PRINT("[DEBUG] Output file opened: %s\n", filename);
    initSymTab();
    DEBUG_PRINT("[DEBUG] Symbol table initialized\n");

    /* ===== NEW: STRING LITERAL SUPPORT ===== */
    /* Pre-pass: collect all string literals before code generation */
    DEBUG_PRINT("[DEBUG] Collecting strings...\n");
    collectStrings(root);
    DEBUG_PRINT("[DEBUG] String collection complete. Found %d strings\n", stringCount);
    /* ===== END: STRING LITERAL SUPPORT ===== */

    DEBUG_PRINT("[DEBUG] Writing .data section\n");
    fprintf(output, ".data\n");
    fprintf(output, "true_str: .asciiz \"true\"\n");
    fprintf(output, "false_str: .asciiz \"false\"\n");

    /* ===== NEW: STRING LITERAL SUPPORT ===== */
    /* Output all string literals */
    for (int i = 0; i < stringCount; i++) {
        fprintf(output, "str_%d: .asciiz \"%s\"\n", i, stringTable[i].value);
    }
    /* ===== END: STRING LITERAL SUPPORT ===== */

    /* ===== NEW: GLOBAL VARIABLES SUPPORT ===== */
    /* Pre-pass: collect global variable declarations */
    DEBUG_PRINT("[DEBUG] Collecting global variables...\n");
    collectGlobalVars(root);
    DEBUG_PRINT("[DEBUG] Global variable collection complete\n");
    /* ===== END: GLOBAL VARIABLES SUPPORT ===== */

    fprintf(output, "\n.text\n");

    // Collect all global function names
    DEBUG_PRINT("[DEBUG] Collecting global functions...\n");
    char* funcNames[100];
    int funcCount = 0;
    collectGlobalFunctions(root, funcNames, &funcCount);
    DEBUG_PRINT("[DEBUG] Found %d functions\n", funcCount);

    // Output .globl directives for all functions
    DEBUG_PRINT("[DEBUG] Writing .globl directives\n");
    for (int i = 0; i < funcCount; i++) {
        DEBUG_PRINT("[DEBUG] .globl %s\n", funcNames[i]);
        fprintf(output, ".globl %s\n", funcNames[i]);
    }
    fprintf(output, "\n");

    // Note: QtSpim will automatically start execution at 'main' since it's .globl
    // No explicit __start needed

    DEBUG_PRINT("[DEBUG] Starting statement generation...\n");
    genStmt(root);
    DEBUG_PRINT("[DEBUG] Statement generation complete!\n");

    DEBUG_PRINT("[DEBUG] Closing output file\n");
    fclose(output);
    DEBUG_PRINT("[DEBUG] ===== MIPS Code Generation Complete =====\n");
}