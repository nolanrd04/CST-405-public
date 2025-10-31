#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symtab.h"
#include "tac.h"

extern TACList optimizedList;

FILE* output;
int tempReg = 0;
int labelCount =0; /*Coounter for unique labels*/

char*newLabel(){
    char* label = malloc(20);
    sprintf(label, "L%d", labelCount++);
    return label;
}

int getNextTemp() {
    int reg = tempReg;
    tempReg = (tempReg + 1) % 8;
    return reg;
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
            int isFloat = (node->data.binop.left->type == NODE_NUM && 
                   node->data.binop.left->data.num.is_float) ||
                  (node->data.binop.right->type == NODE_NUM && 
                   node->data.binop.right->data.num.is_float) ||
                  (node->data.binop.left->type == NODE_VAR) ||
                  (node->data.binop.right->type == NODE_VAR);

            if (isFloat) {
                genExpr(node->data.binop.left);
                fprintf(output, "    mov.s $f2, $f0\n");
                genExpr(node->data.binop.right);
        
                if (node->data.binop.op == '+') {
                    fprintf(output, "    add.s $f0, $f2, $f0\n");
                } else if (node->data.binop.op == '-') {
                    fprintf(output, "    sub.s $f0, $f2, $f0\n");
                } else if (node->data.binop.op == '*') {
                    fprintf(output, "    mul.s $f0, $f2, $f0\n");
                } else if (node->data.binop.op == '/') {
                    fprintf(output, "    div.s $f0, $f2, $f0\n");
                }
            } else {
                genExpr(node->data.binop.left);
                int leftReg = tempReg - 1;
                genExpr(node->data.binop.right);
                int rightReg = tempReg - 1;

                if (node->data.binop.op == '+') {
                    fprintf(output, "    add $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == '-') {
                    fprintf(output, "    sub $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == '*') {
                    fprintf(output, "    mul $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else if (node->data.binop.op == '/') {
                    fprintf(output, "    div $t%d, $t%d, $t%d\n", leftReg, leftReg, rightReg);
                } else {
                    fprintf(stderr, "Error: unsupported binary op '%c'\n", node->data.binop.op);
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
                        fprintf(output, "    sw $t0, %d($sp)\n", 4 + (argNum * 4));
                    }

                    argNum++;
                    arg = arg->data.arg_list.next;
                } else {
                    tempReg = 0;
                    genExpr(arg);

                    if (arg->type == NODE_NUM && arg->data.num.is_float) {
                        fprintf(output, "    swc1 $f0, %d($sp)\n", 4 + (argNum * 4));
                    } else {
                        fprintf(output, "    sw $t0, %d($sp)\n", 4 + (argNum * 4));
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
                fprintf(output, "    sw $t0, %d($sp)\n", offset);
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
                fprintf(output, "    sw $t0, %d($sp)\n", offset);
            }
            tempReg = 0;
            break;
        }
        
        case NODE_PRINT: {
            tempReg = 0;
            genExpr(node->data.expr);
    
            int isFloat = 0;
            if (node->data.expr->type == NODE_NUM && node->data.expr->data.num.is_float) {
                isFloat = 1;
            } else if (node->data.expr->type == NODE_VAR) {
                char* type = getVarType(node->data.expr->data.name);
                if (type && strcmp(type, "float") == 0) {
                    isFloat = 1;
                }
            } else if (node->data.expr->type == NODE_ARRAY_ACCESS) {
                char* type = getVarType(node->data.expr->data.array_access.name);
                if (type && strcmp(type, "float") == 0) {
                    isFloat = 1;
                }
            } else if (node->data.expr->type == NODE_ARRAY_2D_ACCESS) {
                char* type = getVarType(node->data.expr->data.array_2d_access.name);
                if (type && strcmp(type, "float") == 0) {
                    isFloat = 1;
                }
            }
    
            if (isFloat) {
                fprintf(output, "    # Print float\n");
                fprintf(output, "    mov.s $f12, $f0\n");
                fprintf(output, "    li $v0, 2\n");
                fprintf(output, "    syscall\n");
            } else {
                fprintf(output, "    # Print integer\n");
                fprintf(output, "    move $a0, $t%d\n", tempReg - 1);
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
            fprintf(output, "    move $v0, $t0\n");
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
                    fprintf(output, "    sw $t0, %d($sp)\n", 4 + (argNum * 4));
                    argNum++;
                    arg = arg->data.arg_list.next;
                } else {
                    tempReg = 0;
                    genExpr(arg);
                    fprintf(output, "    sw $t0, %d($sp)\n", 4 + (argNum * 4));
                    break;
                }
            }
    
            fprintf(output, "    jal %s\n", node->data.func_call.name);
            fprintf(output, "    move $t0, $v0\n");
            tempReg = 0;
            break;
        }
        case NODE_SWITCH: {
            fprintf(output, "\n   #Switch Statement\n");

            /*EValiuate switch expression*/
            tempReg =0;
            genExpr( node->data.switch_stmt.expr);
            int switchReg = tempReg -1;
            char* endLabel = newLabel();

            ASTNode* caseNode = node->data.switch_stmt.cases;
            char* defaultLabel = NULL;

            /*Genreate case comparisons and jumps*/
            while(caseNode){
                if(caseNode->type == NODE_CASE_LIST){
                    ASTNode* currentCase = caseNode->data.case_list.case_item;

                    if(currentCase->type == NODE_CASE){
                        char* caseLabel = newLabel();
                        int caseValue = currentCase->data.case_stmt.value;

                        /*Compare and branch*/
                        fprintf(output, "    li $t%d, %d            #Case value %d\n", tempReg, caseValue, caseValue);
                        fprintf(output, "    beq $t%d, $t%d, %s    #If equal, jump to case\n", 
                                switchReg, tempReg, caseLabel);
                        
                        /*Store label and statements for later */
                        fprintf(output, "%s:\n", caseLabel);
                        genStmt(currentCase->data.case_stmt.stmts);
                        fprintf(output, "    j %s                  #Break\n", endLabel);
                    } else if(currentCase->type == NODE_DEFAULT_CASE){
                        defaultLabel = newLabel();
                        fprintf(output, "%s:\n", defaultLabel);
                        genStmt(currentCase->data.default_case.stmts);
                    }

                    caseNode = caseNode->data.case_list.next;
                } else if (caseNode->type ==NODE_CASE){
                    char* caseLabel = newLabel():
                    int caseValue = caseNode->data.case_stmt.value;

                    fprintf(output, "    li $t%d, %d            #Case value %d\n", tempReg, caseValue, caseValue);
                    fprintf(output, "    beq $t%d, $t%d, %s    #If equal, jump to case\n", 
                            switchReg, tempReg, caseLabel);
                    fprintf(output, "%s:\n", caseLabel);
                    genStmt(caseNode->data.case_stmt.stmts);
                    fprintf(output, "    j %s                  #Break\n", endLabel);
                    break;
                }else if(caseNode->type == NODE_DEFAULT_CASE){
                    defaultLabel = newLabel();
                    fprintf(output, "%s:\n", defaultLabel);
                    genStmt(caseNode->data.default_case.stmts);
                    break;
                }
            }
            /*Jump to default if no case matched*/
            if(defaultLabel){
                fprintf(output, "    j %s                  #Jump to default case\n", defaultLabel);
            }
            /*End label*/
            fprintf(output, "%s:\n", endLabel);
            fprintf(output, "    #End of switch statement\n");
            break;

            tempReg =0;
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

void generateMIPS(ASTNode* root, const char* filename) {
    output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "Cannot open output file %s\n", filename);
        exit(1);
    }
    
    initSymTab();
    
    fprintf(output, ".data\n\n");
    fprintf(output, ".text\n");
    fprintf(output, ".globl main\n\n");
    
    genStmt(root);
    
    fclose(output);
}