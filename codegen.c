#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symtab.h"
#include "tac.h"

extern TACList optimizedList;

FILE* output;
int tempReg = 0;
char* currentFunctionType = NULL;

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

// Helper function to convert integer in $t register to float in $f register
void genIntToFloatConversion(int tReg, int fReg) {
    fprintf(output, "    mtc1 $t%d, $f%d\n", tReg, fReg);
    fprintf(output, "    cvt.s.w $f%d, $f%d\n", fReg, fReg);
}

void genExpr(ASTNode* node) {
    if (!node) return;
    
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
            
        case NODE_VAR: {
            int offset = getVarOffset(node->data.name);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s not declared\n", node->data.name);
                exit(1);
            }    
            char* type = getVarType(node->data.name);
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    lwc1 $f0, %d($sp)\n", offset);
                tempReg = 0;
            } else {
                fprintf(output, "    lw $t%d, %d($sp)\n", getNextTemp(), offset);
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
                genExpr(node->data.binop.left);
                leftReg = tempReg - 1;
                genExpr(node->data.binop.right);
                rightReg = tempReg - 1;
                if (leftReg < 0) leftReg = 0;
                if (rightReg < 0) rightReg = 0;
        
                // Arithmetic operators
                if (node->data.binop.op == OP_ADD) {
                    fprintf(output, "    add $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_SUB) {
                fprintf(output, "    sub $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_MUL) {
                    fprintf(output, "    mul $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_DIV) {
                    fprintf(output, "    div $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                }
                // Comparison operators
                else if (node->data.binop.op == OP_GT) {
                    fprintf(output, "    sgt $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_LT) {
                    fprintf(output, "    slt $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_GTE) {
                    fprintf(output, "    sge $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_LTE) {
                    fprintf(output, "    sle $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_EQ) {
                    fprintf(output, "    seq $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == OP_NEQ) {
                    fprintf(output, "    sne $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else {
                    fprintf(stderr, "Error: unsupported binary op for int\n");
                    exit(1);
                }
                tempReg = leftReg + 1;
            }
            break;
        }
        
        case NODE_ARRAY_ACCESS: {
            genExpr(node->data.array_access.index);
            int idxReg = tempReg - 1;
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();
            int resReg  = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_access.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_access.name);
                exit(1);
            }

            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                    baseReg, baseOffset, node->data.array_access.name);
            fprintf(output, "    addu  $t%d, $t%d, $t%d  # element address\n",
                    addrReg, baseReg, idxReg);
            fprintf(output, "    lw    $t%d, 0($t%d)     # load array element\n",
                    resReg, addrReg);

            char* type = getVarType(node->data.array_access.name);
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    lwc1 $f0, 0($t%d)     # load float value\n", addrReg);
                tempReg = 0;
            } else {
                fprintf(output, "    lw    $t%d, 0($t%d)     # load int value\n", resReg, addrReg);
            }
            break;
        }

        case NODE_ARRAY_ASSIGN: {
            if (!isArrayVar(node->data.array_assign.name)) {
                fprintf(stderr, "Error: %s is not an array\n", node->data.array_assign.name);
                exit(1);
            }

            genExpr(node->data.array_assign.index);
            int idxReg = tempReg - 1;
            genExpr(node->data.array_assign.value);
            int valReg = tempReg - 1;
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_assign.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_assign.name);
                exit(1);
            }

            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                    baseReg, baseOffset, node->data.array_assign.name);
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

        case NODE_ARRAY_2D_ACCESS: {
            int sizeY = getArray2DSizeY(node->data.array_2d_access.name);
            if (sizeY == -1) {
                fprintf(stderr, "Error: Array %s not found or not 2D\n", 
                        node->data.array_2d_access.name);
                exit(1);
            }
    
            genExpr(node->data.array_2d_access.indexX);
            int idxXReg = tempReg - 1;
            genExpr(node->data.array_2d_access.indexY);
            int idxYReg = tempReg - 1;
    
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
            fprintf(output, "    # Call function: %s\n", node->data.func_call.name);
            ASTNode* arg = node->data.func_call.args;
            int argNum = 0;

            while (arg) {
                if (arg->type == NODE_ARG_LIST) {
                    tempReg = 0;
                    genExpr(arg->data.arg_list.expr);

                    if(arg->data.arg_list.expr->type == NODE_NUM && arg->data.arg_list.expr->data.num.is_float) {
                        fprintf(output, "    swc1 $f0, %d($sp)\n", 4 + (argNum * 4));
                    } else {
                        int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                        fprintf(output, "    sw $t%d, %d($sp)\n", resultReg, 4 + (argNum * 4));
                    }

                    argNum++;
                    arg = arg->data.arg_list.next;
                } else {
                    tempReg = 0;
                    genExpr(arg);

                    if (arg->type == NODE_NUM && arg->data.num.is_float) {
                        fprintf(output, "    swc1 $f0, %d($sp)\n", 4 + (argNum * 4));
                    } else {
                        int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                        fprintf(output, "    sw $t%d, %d($sp)\n", resultReg, 4 + (argNum * 4));
                    }
                    break;
                }
            }

            fprintf(output, "    jal %s\n", node->data.func_call.name);
            fprintf(output, "    move $t0, $v0\n");
            tempReg = 0;
            break;
        }

        default:
            break;
    }
}

void genStmt(ASTNode* node) {
    if (!node) return;
    
    switch(node->type) {
        case NODE_DECL: {
            int offset = addVar(node->data.decl.varName, node->data.decl.varType);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s already declared\n", node->data.name);
                exit(1);
            }
            fprintf(output, "    # Declared %s at scope %d, offset %d\n", 
                    node->data.decl.varName, getCurrentScope(), offset);
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
            if (type && strcmp(type, "float") == 0) {
                fprintf(output, "    swc1 $f0, %d($sp)\n", offset);
            } else {
                int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                fprintf(output, "    sw $t%d, %d($sp)\n", resultReg, offset);
            }
            tempReg = 0;
            break;
        }
        
        case NODE_PRINT: {
            tempReg = 0;
            genExpr(node->data.expr);

            // ✅ Use the helper function to determine if expression is float
            int isFloat = isExprFloat(node->data.expr);
            if (isFloat) {
                fprintf(output, "    # Print float\n");
                fprintf(output, "    mov.s $f12, $f0\n");
                fprintf(output, "    li $v0, 2\n");
                fprintf(output, "    syscall\n");
            } else {
                int printReg = tempReg > 0 ? tempReg - 1 : 0;
                fprintf(output, "    # Print integer\n");
                fprintf(output, "    move $a0, $t%d\n", printReg);
                fprintf(output, "    li $v0, 1\n");
                fprintf(output, "    syscall\n");
            }
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
            
            // Branch if condition is false (or zero)
            fprintf(output, "    beqz $t0, else_%d\n", currentIf);
    
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
            int offset = addArrayVar(node->data.array_decl.name, node->data.array_decl.size, node->data.array_decl.type);
            if (offset == -1) {
                fprintf(stderr, "Error: Array %s already declared\n", node->data.array_decl.name);
                exit(1);
            }
            fprintf(output, "    # Declared array %s of size %d at scope %d, offset %d\n", 
                    node->data.array_decl.name, node->data.array_decl.size, getCurrentScope(), offset);
            break;
        }

        case NODE_ARRAY_ASSIGN: {
            if (!isArrayVar(node->data.array_assign.name)) {
                fprintf(stderr, "Error: %s is not an array\n", node->data.array_assign.name);
                exit(1);
            }

            genExpr(node->data.array_assign.index);
            int idxReg = tempReg - 1;
            genExpr(node->data.array_assign.value);
            int valReg = tempReg - 1;
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_assign.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_assign.name);
                exit(1);
            }

            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                    baseReg, baseOffset, node->data.array_assign.name);
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
            fprintf(output, "\n%s:\n", node->data.func_decl.name);
            fprintf(output, "    # Function: %s (Scope Level: %d)\n", 
                    node->data.func_decl.name, getCurrentScope() + 1);
            
            // ✅ FIXED: Enter new scope instead of resetting symbol table
            enterScope();

            addVar("$ra_slot", "int");

            if (strcmp(node->data.func_decl.name, "main") == 0) {
                fprintf(output, "    addi $sp, $sp, -400\n");
            }
    
            fprintf(output, "    addi $sp, $sp, -100\n");
            fprintf(output, "    sw $ra, 0($sp)\n");
    
            // Load parameters
            ASTNode* param = node->data.func_decl.params;
            int paramNum = 0;
    
            while (param) {
                if (param->type == NODE_PARAM_LIST) {
                    if (param->data.param_list.param->type == NODE_PARAM) {
                        char* paramName = param->data.param_list.param->data.param.name;
                        char* paramType = param->data.param_list.param->data.param.type;
                        int offset = addVar(paramName, paramType);
                
                        if (strcmp(paramType, "float") == 0) {
                            fprintf(output, "    lwc1 $f0, %d($sp)\n", 100 + 4 + (paramNum * 4));
                            fprintf(output, "    swc1 $f0, %d($sp)  # Store param %s (float) at scope %d\n", 
                                    offset, paramName, getCurrentScope());
                        } else {
                            fprintf(output, "    lw $t0, %d($sp)\n", 100 + 4 + (paramNum * 4));
                            fprintf(output, "    sw $t0, %d($sp)  # Store param %s (int) at scope %d\n", 
                                    offset, paramName, getCurrentScope());
                        }
                
                        paramNum++;
                    }
                    param = param->data.param_list.next;
                } else if (param->type == NODE_PARAM) {
                    char* paramName = param->data.param.name;
                    char* paramType = param->data.param.type;
                    int offset = addVar(paramName, paramType);
            
                    if (strcmp(paramType, "float") == 0) {
                        fprintf(output, "    lwc1 $f0, %d($sp)\n", 100 + 4 + (paramNum * 4));
                        fprintf(output, "    swc1 $f0, %d($sp)  # Store param %s (float) at scope %d\n", 
                                offset, paramName, getCurrentScope());
                    } else {
                        fprintf(output, "    lw $t0, %d($sp)\n", 100 + 4 + (paramNum * 4));
                        fprintf(output, "    sw $t0, %d($sp)  # Store param %s (int) at scope %d\n", 
                                offset, paramName, getCurrentScope());
                    }
                    break;
                } else {
                    break;
                }
            }
    
            // Generate function body
            genStmt(node->data.func_decl.body);
            
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
                fprintf(output, "    move $v0, $t0\n");  // Integer return value
            }
            fprintf(output, "    lw $ra, 0($sp)\n");
            fprintf(output, "    addi $sp, $sp, 100\n");
            fprintf(output, "    jr $ra\n");
            break;
        }
        
        case NODE_FUNC_CALL: {
            fprintf(output, "    # Call function: %s\n", node->data.func_call.name);
            ASTNode* arg = node->data.func_call.args;
            int argNum = 0;

            while (arg) {
                if (arg->type == NODE_ARG_LIST) {
                    tempReg = 0;
                    genExpr(arg->data.arg_list.expr);
                    int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    sw $t%d, %d($sp)\n", resultReg, 4 + (argNum * 4));
                    argNum++;
                    arg = arg->data.arg_list.next;
                } else {
                    tempReg = 0;
                    genExpr(arg);
                    int resultReg = tempReg > 0 ? tempReg - 1 : 0;
                    fprintf(output, "    sw $t%d, %d($sp)\n", resultReg, 4 + (argNum * 4));
                    break;
                }
            }

            fprintf(output, "    jal %s\n", node->data.func_call.name);
            fprintf(output, "    move $t0, $v0\n");
            tempReg = 0;
            break;
        }
        case NODE_SWITCH: {
            fprintf(output, "\n    # Switch Statement\n");

            /* Evaluate switch expression and store in switchReg */
            tempReg = 0;
            genExpr(node->data.switch_stmt.expr);
            int switchReg = tempReg > 0 ? tempReg - 1 : 0;
            char* endLabel = newLabel();

            ASTNode* caseNode = node->data.switch_stmt.cases;
            char* defaultLabel = NULL;

            fprintf(output, "    # DEBUG: caseNode = %p, type = %d\n", (void*)caseNode, caseNode ? caseNode->type : -1);

            /* First pass: generate all comparisons for non-default cases */
            ASTNode* currentNode = caseNode;
            while(currentNode) {
                if(currentNode->type == NODE_CASE_LIST) {
                    /* Extract the case_item from the case_list */
                    ASTNode* caseItem = currentNode->data.case_list.case_item;
                    if(caseItem && caseItem->type == NODE_CASE) {
                        int caseValue = caseItem->data.case_stmt.value;
                        fprintf(output, "    li $t7, %d\n", caseValue);
                        fprintf(output, "    beq $t%d, $t7, case_%d\n", switchReg, caseValue);
                    } else if(caseItem && caseItem->type == NODE_DEFAULT_CASE) {
                        defaultLabel = newLabel();
                    }
                    /* Move to next case_list node */
                    currentNode = currentNode->data.case_list.next;
                } else if(currentNode->type == NODE_BREAK) {
                    /* Skip BREAK nodes - they're handled as part of the case */
                    currentNode = currentNode->right;  /* Try to get next sibling */
                } else {
                    currentNode = NULL;
                }
            }

            /* Jump to default if no case matched */
            if(defaultLabel) {
                fprintf(output, "    j %s\n", defaultLabel);
            } else {
                fprintf(output, "    j %s\n", endLabel);
            }

            /* Second pass: generate case labels and code */
            currentNode = caseNode;
            while(currentNode) {
                if(currentNode->type == NODE_CASE_LIST) {
                    ASTNode* caseItem = currentNode->data.case_list.case_item;
                    if(caseItem && caseItem->type == NODE_CASE) {
                        int caseValue = caseItem->data.case_stmt.value;
                        fprintf(output, "case_%d:\n", caseValue);
                        genStmt(caseItem->data.case_stmt.stmts);
                        fprintf(output, "    j %s\n", endLabel);
                    } else if(caseItem && caseItem->type == NODE_DEFAULT_CASE) {
                        if(defaultLabel) {
                            fprintf(output, "%s:\n", defaultLabel);
                            genStmt(caseItem->data.default_case.stmts);
                            fprintf(output, "    j %s\n", endLabel);
                        }
                    }
                    currentNode = currentNode->data.case_list.next;
                } else {
                    currentNode = NULL;
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
        

        
        default:
            break;
    }
}


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
    output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "Cannot open output file %s\n", filename);
        exit(1);
    }

    initSymTab();

    fprintf(output, ".data\n\n");
    fprintf(output, ".text\n");

    // Collect all global function names
    char* funcNames[100];
    int funcCount = 0;
    collectGlobalFunctions(root, funcNames, &funcCount);

    // Output .globl directives for all functions
    for (int i = 0; i < funcCount; i++) {
        fprintf(output, ".globl %s\n", funcNames[i]);
    }
    fprintf(output, "\n");

    genStmt(root);

    fclose(output);
}