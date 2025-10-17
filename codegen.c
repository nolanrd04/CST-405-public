#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symtab.h"
#include "tac.h"

extern TACList optimizedList;  // to access the optimized TAC list

FILE* output;
int tempReg = 0;

int getNextTemp() {
    int reg = tempReg;
    tempReg = (tempReg + 1) % 8;  // Wrap around after $t7
    return reg;
}

void genExpr(ASTNode* node) {
    if (!node) return;
    
    switch(node->type) {
        case NODE_NUM:
            fprintf(output, "    li $t%d, %d\n", getNextTemp(), node->data.num);
            break;
            
        case NODE_VAR: {
            int offset = getVarOffset(node->data.name);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s not declared\n", node->data.name);
                exit(1);
            }
            fprintf(output, "    lw $t%d, %d($sp)\n", getNextTemp(), offset);
            break;
        }
        
        case NODE_BINOP:
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
            break;
        
        case NODE_ARRAY_ACCESS: {
            /* index -> temp (genExpr leaves result in last temp) */
            genExpr(node->data.array_access.index);
            int idxReg = tempReg - 1;         /* index is in $t{idxReg} */

            /* allocate temps for base address, element addr, and result */
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();
            int resReg  = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_access.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_access.name);
                exit(1);
            }

            /* scale index (in-place) */
            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);

            /* compute base address and element address using addiu/addu */
            fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                    baseReg, baseOffset, node->data.array_access.name);
            fprintf(output, "    addu  $t%d, $t%d, $t%d  # element address\n",
                    addrReg, baseReg, idxReg);

            /* load element into result temp */
            fprintf(output, "    lw    $t%d, 0($t%d)     # load array element\n",
                    resReg, addrReg);

            /* leave result in last temp (tempReg already advanced) */
            break;
        }

        case NODE_ARRAY_ASSIGN: 
        {
            if (!isArrayVar(node->data.array_assign.name)) {
                fprintf(stderr, "Error: %s is not an array\n", node->data.array_assign.name);
                exit(1);
            }

            /* evaluate index then value (value last -> in last temp) */
            genExpr(node->data.array_assign.index);
            int idxReg = tempReg - 1;

            genExpr(node->data.array_assign.value);
            int valReg = tempReg - 1;

            /* allocate separate temps for base address and element address */
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_assign.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_assign.name);
                exit(1);
            }

            /* scale index, compute address, store value; use addiu/addu */
            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                    baseReg, baseOffset, node->data.array_assign.name);
            fprintf(output, "    addu  $t%d, $t%d, $t%d  # element address\n",
                    addrReg, baseReg, idxReg);
            fprintf(output, "    sw    $t%d, 0($t%d)     # store value\n",
                    valReg, addrReg);

            tempReg = 0;
            break;
        }
        case NODE_ARRAY_2D_ACCESS:
        {
            // For arr[i][j], memory layout is: base + (i * sizeY + j) * 4
    
            // Get the sizeY dimension first
            int sizeY = getArray2DSizeY(node->data.array_2d_access.name);
            if (sizeY == -1) {
                fprintf(stderr, "Error: Array %s not found or not 2D\n", 
                        node->data.array_2d_access.name);
                exit(1);
            }
    
            // Generate code for indexX
            genExpr(node->data.array_2d_access.indexX);
            int idxXReg = tempReg - 1;
    
            // Generate code for indexY
            genExpr(node->data.array_2d_access.indexY);
            int idxYReg = tempReg - 1;
    
            int baseOffset = getVarOffset(node->data.array_2d_access.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", 
                        node->data.array_2d_access.name);
                exit(1);
            }
    
            // Allocate temporary registers
            int sizeYReg = getNextTemp();
            int tempMultReg = getNextTemp();
            int offsetReg = getNextTemp();
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();
            int resultReg = getNextTemp();
    
            fprintf(output, "    # 2D array access: %s[i][j]\n", node->data.array_2d_access.name);
    
            // Load sizeY into a register
            fprintf(output, "    li $t%d, %d           # sizeY = %d\n", sizeYReg, sizeY, sizeY);
    
            // Multiply indexX * sizeY
            fprintf(output, "    mult $t%d, $t%d       # multiply indexX * sizeY\n", idxXReg, sizeYReg);
            fprintf(output, "    mflo $t%d             # get result from LO register\n", tempMultReg);
    
            // Add indexY: (indexX * sizeY + indexY)
            fprintf(output, "    add $t%d, $t%d, $t%d  # (indexX*sizeY + indexY)\n", offsetReg, tempMultReg, idxYReg);
    
            // Multiply by 4 (bytes per int)
            fprintf(output, "    sll $t%d, $t%d, 2     # multiply by 4\n", offsetReg, offsetReg);
    
            // Get base address
            fprintf(output, "    addiu $t%d, $sp, %d   # base address\n", baseReg, baseOffset);
    
            // Calculate final address
            fprintf(output, "    addu $t%d, $t%d, $t%d # final address\n", addrReg, baseReg, offsetReg);
    
            // Load the value
            fprintf(output, "    lw $t%d, 0($t%d)      # load array[i][j]\n", resultReg, addrReg);
    
            tempReg = resultReg + 1;
            break;
        }

        case NODE_FUNC_CALL: {
    fprintf(output, "    # Call function: %s\n", node->data.func_call.name);
    
    // Evaluate and store arguments at correct offsets
    ASTNode* arg = node->data.func_call.args;
    int argNum = 0;
    
    while (arg) {
        if (arg->type == NODE_ARG_LIST) {
            tempReg = 0;
            genExpr(arg->data.arg_list.expr);  // ✅ Changed from .arg to .expr
            // Store at offset 4, 8, 12, ... (starting after our frame)
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

        default:
            break;
    }
}

void genStmt(ASTNode* node) {
    if (!node) return;
    
    switch(node->type) {
        case NODE_DECL: {
            int offset = addVar(node->data.name);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s already declared\n", node->data.name);
                exit(1);
            }
            fprintf(output, "    # Declared %s at offset %d\n", node->data.name, offset);
            break;
        }
        case NODE_DECL_ASSIGN: {
            // Declare the variable
            int offset = addVar(node->data.declAssign.id);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s already declared\n", node->data.declAssign.id);
                exit(1);
            }
            fprintf(output, "    # Declared %s at offset %d\n", node->data.declAssign.id, offset);
            tempReg = 0;
            // Generate code for the initializer expression
            genExpr(node->data.declAssign.expr);
            // Store the result in the variable's stack slot
            fprintf(output, "    sw $t0, %d($sp)\n", offset);
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
            fprintf(output, "    sw $t0, %d($sp)\n", offset);
            tempReg = 0;
            break;
        }
        
        case NODE_PRINT:
            genExpr(node->data.expr);
            fprintf(output, "    # Print integer\n");
            fprintf(output, "    move $a0, $t%d\n", tempReg - 1);
            fprintf(output, "    li $v0, 1\n");
            fprintf(output, "    syscall\n");
            fprintf(output, "    # Print newline\n");
            fprintf(output, "    li $v0, 11\n");
            fprintf(output, "    li $a0, 10\n");
            fprintf(output, "    syscall\n");
            tempReg = 0;
            break;
            
        case NODE_STMT_LIST:
            genStmt(node->data.stmtlist.stmt);
            genStmt(node->data.stmtlist.next);
            break;
            
        case NODE_ARRAY_DECL:
        {
            int offset = addArrayVar(node->data.array_decl.name, node->data.array_decl.size);
            if (offset == -1) {
                fprintf(stderr, "Error: Array %s already declared\n", node->data.array_decl.name);
                exit(1);
            }
            fprintf(output, "    # Declared array %s of size %d at offset %d\n", node->data.array_decl.name, node->data.array_decl.size, offset);
            break;
        }

        case NODE_ARRAY_ASSIGN:
        {
            if (!isArrayVar(node->data.array_assign.name)) {
                fprintf(stderr, "Error: %s is not an array\n", node->data.array_assign.name);
                exit(1);
            }

            /* evaluate index then value (value last -> in last temp) */
            genExpr(node->data.array_assign.index);
            int idxReg = tempReg - 1;

            genExpr(node->data.array_assign.value);
            int valReg = tempReg - 1;

            /* allocate separate temps for base address and element address */
            int baseReg = getNextTemp();
            int addrReg = getNextTemp();

            int baseOffset = getVarOffset(node->data.array_assign.name);
            if (baseOffset == -1) {
                fprintf(stderr, "Error: Array %s not declared\n", node->data.array_assign.name);
                exit(1);
            }

            /* scale index, compute address, store value; use addiu/addu */
            fprintf(output, "    sll  $t%d, $t%d, 2      # idx * 4\n", idxReg, idxReg);
            fprintf(output, "    addiu $t%d, $sp, %d   # base of %s\n",
                    baseReg, baseOffset, node->data.array_assign.name);
            fprintf(output, "    addu  $t%d, $t%d, $t%d  # element address\n",
                    addrReg, baseReg, idxReg);
            fprintf(output, "    sw    $t%d, 0($t%d)     # store value\n",
                    valReg, addrReg);

            tempReg = 0;
            break;
        }


        // Fixed NODE_ARRAY_DECL_ASSIGN case
        case NODE_ARRAY_DECL_ASSIGN:
        { 
            ASTNode* init = node->data.array_decl_assign.initList;
            int size = node->data.array_decl_assign.size;

            // First, count elements and build a forward array
            ASTNode* expressions[100]; // Assume max 100 elements
            int count = 0;

            // Traverse list and extract actual expressions
            ASTNode* cur = init;
            while (cur != NULL && count < 100) 
            {
                if (cur->type == NODE_STMT_LIST) {
                    // Get the expression from this list node
                ASTNode* expr = cur->data.stmtlist.stmt;
                if (expr != NULL) {
                    expressions[count++] = expr;
                }
                cur = cur->data.stmtlist.next;
                } else {
                // This is a direct expression, not wrapped in list
                expressions[count++] = cur;
                break;
                }
            }

            if (size == 0) {
                size = count; // Infer size
            }

            int offset = addArrayVar(node->data.array_decl_assign.name, size);
            if (offset == -1) { 
                fprintf(stderr, "Error: Variable %s already declared\n", node->data.array_decl_assign.name); 
                exit(1); 
            }

            // Store the actual initializer values - SIMPLIFIED APPROACH
            for (int i = 0; i < count && i < size; i++) {
            // Reset temp register counter for each iteration
                tempReg = 0;
        
            // Generate expression into $t0
                genExpr(expressions[count - 1 - i]);
        
            // Use fixed registers for address calculation
                fprintf(output, "    # Storing value at array position %d\n", i);
                fprintf(output, "    addiu $t1, $sp, %d      # base address\n", offset);
                fprintf(output, "    li    $t2, %d           # offset = %d * 4\n", i * 4, i);
                fprintf(output, "    addu  $t3, $t1, $t2    # element address\n");
                fprintf(output, "    sw    $t0, 0($t3)      # store value\n");
            }
    
            tempReg = 0; // Reset for next statement
            break;
        }
        case NODE_ARRAY_2D_DECL:
        {
            int offset = addArray2DVar(node->data.array_2d_decl.name, node->data.array_2d_decl.sizeX, node->data.array_2d_decl.sizeY); /* change to 2d array add */
                if (offset == -1) {
                    fprintf(stderr, "Error: Array %s already declared\n", node->data.array_2d_decl.name);
                    exit(1);
                }
                fprintf(output, "    # Declared array %s of size %d , %d at offset %d\n", node->data.array_2d_decl.name, node->data.array_2d_decl.sizeX, node->data.array_2d_decl.sizeY, offset); /* what is %s and %d*/
                break;
        }

        case NODE_ARRAY_2D_ELEM_ASSIGN:
        {
            if (!is2DArrayVar(node->data.array_2d_elem_assign.name)) {
                fprintf(stderr, "Error: %s is not a 2D array\n", node->data.array_2d_elem_assign.name);
                exit(1);
            }

            int sizeY = getArray2DSizeY(node->data.array_2d_elem_assign.name);
    
            // Reset temp counter for clean register usage
            tempReg = 0;
    
            // Generate indexX -> goes into $t0
            genExpr(node->data.array_2d_elem_assign.indexX);
            int idxXReg = 0;  // We know it's in $t0
    
            // Generate indexY -> goes into $t1
            genExpr(node->data.array_2d_elem_assign.indexY);
            int idxYReg = 1;  // We know it's in $t1
    
            // Generate value -> goes into $t2
            genExpr(node->data.array_2d_elem_assign.value);
            int valReg = 2;   // We know it's in $t2

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
            fprintf(output, "    sw $t%d, 0($t6)       # store value\n", valReg);

            tempReg = 0;
            break;
        }

        case NODE_FUNC_DECL: {
    fprintf(output, "\n%s:\n", node->data.func_decl.name);
    fprintf(output, "    # Function: %s\n", node->data.func_decl.name);
    
    // Special handling for main
    if (strcmp(node->data.func_decl.name, "main") == 0) {
        fprintf(output, "    addi $sp, $sp, -400\n");
    }
    
    fprintf(output, "    addi $sp, $sp, -100\n");
    fprintf(output, "    sw $ra, 0($sp)\n");
    
    // ✅ Load parameters from CALLER's frame (100 bytes up from current $sp)
    ASTNode* param = node->data.func_decl.params;
    int paramNum = 0;
    int paramOffset = 4;  // Start at offset 4 in OUR frame (0 is $ra)
    
    while (param) {
        if (param->type == NODE_PARAM_LIST) {
            if (param->data.param_list.param->type == NODE_PARAM) {
                char* paramName = param->data.param_list.param->data.param.name;
                addVar(paramName);
                
                // ✅ Load from caller's frame: 100 (our frame) + 4 (caller's offset)
                fprintf(output, "    lw $t0, %d($sp)\n", 100 + 4 + (paramNum * 4));
                fprintf(output, "    sw $t0, %d($sp)  # Store param %s\n", paramOffset, paramName);
                
                paramNum++;
                paramOffset += 4;
            }
            param = param->data.param_list.next;
        } else if (param->type == NODE_PARAM) {
            char* paramName = param->data.param.name;
            addVar(paramName);
            
            fprintf(output, "    lw $t0, %d($sp)\n", 100 + 4 + (paramNum * 4));
            fprintf(output, "    sw $t0, %d($sp)  # Store param %s\n", paramOffset, paramName);
            break;
        } else {
            break;
        }
    }
    
    genStmt(node->data.func_decl.body);
    break;
}

        case NODE_BLOCK: {
            // Process all statements in the block
            genStmt(node->data.block.stmts);
            break;
        }
        
        case NODE_RETURN: {
            // Evaluate return expression and put in $v0
            genExpr(node->data.ret.value);
            fprintf(output, "    move $v0, $t0\n");  // Return value in $v0
            
            // Restore and return
            fprintf(output, "    lw $ra, 0($sp)\n");
            fprintf(output, "    addi $sp, $sp, 100\n");
            fprintf(output, "    jr $ra\n");
            break;
        }
        
case NODE_FUNC_CALL: {
    fprintf(output, "    # Call function: %s\n", node->data.func_call.name);
    
    // ✅ Evaluate and store arguments at correct offsets
    ASTNode* arg = node->data.func_call.args;
    int argNum = 0;
    
    while (arg) {
        if (arg->type == NODE_ARG_LIST) {
            tempReg = 0;
            genExpr(arg->data.arg_list.expr);
            // ✅ Store at offset 4, 8, 12, ... (starting after our frame)
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

        default:
            break;
    }
}

// Helper function - add this before generateMIPS()

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
    
    // ✅ Just generate everything - let functions create their own labels
    genStmt(root);
    
    fclose(output);
}