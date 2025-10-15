#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tac.h"
#include "symtab.h"

TACList tacList;
TACList optimizedList;

typedef struct {
    char* var;
    char* value;
} ValueProp;

ValueProp values[100];
int valueCount = 0;


void initTAC() {
    tacList.head = NULL;
    tacList.tail = NULL;
    tacList.tempCount = 0;
    optimizedList.head = NULL;
    optimizedList.tail = NULL;
}

char* newTemp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", tacList.tempCount++);
    return temp;
}

TACInstr* createTAC(TACOp op, char* arg1, char* arg2, char* result) {
    TACInstr* instr = malloc(sizeof(TACInstr));
    instr->op = op;
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->result = result ? strdup(result) : NULL;
    instr->next = NULL;
    return instr;
}

static char* propagateValue(const char* name) {
    if (!name) return NULL;
    for (int i = valueCount - 1; i >= 0; --i) {
        if (values[i].var && strcmp(values[i].var, name) == 0) {
            return values[i].value; // note: points into values table
        }
    }
    return (char*)name;
}

void appendTAC(TACInstr* instr) {
    if (!tacList.head) {
        tacList.head = tacList.tail = instr;
    } else {
        tacList.tail->next = instr;
        tacList.tail = instr;
    }
}

void appendOptimizedTAC(TACInstr* instr) {
    if (!optimizedList.head) {
        optimizedList.head = optimizedList.tail = instr;
    } else {
        optimizedList.tail->next = instr;
        optimizedList.tail = instr;
    }
}

char* generateTACExpr(ASTNode* node) {
    if (!node) return NULL;
    
    switch(node->type) {
        case NODE_NUM: {
            char* temp = malloc(20);
            sprintf(temp, "%d", node->data.num);
            return temp;
        }
        
        case NODE_VAR:
            return strdup(node->data.name);
        
        case NODE_BINOP: {
            char* left = generateTACExpr(node->data.binop.left);
            char* right = generateTACExpr(node->data.binop.right);
            char* temp = newTemp();
            
            if (node->data.binop.op == '+') {
                appendTAC(createTAC(TAC_ADD, left, right, temp));
            }

            if (node->data.binop.op == '-')
            {
                appendTAC(createTAC(TAC_SUB, left, right, temp));
            }

            if (node->data.binop.op == '*')
            {
                appendTAC(createTAC(TAC_MUL, left, right, temp));
            }
            
            return temp;
        }

        case NODE_ARRAY_ACCESS:
        {
            char* indexExpr = generateTACExpr(node->data.array_access.index);
            char* temp = newTemp();
            // ✅ Need to pass the array NAME as arg2!
            appendTAC(createTAC(TAC_ARRAY_ACCESS, indexExpr, node->data.array_access.name, temp));
            return temp;
        }

        case NODE_ARRAY_2D_ACCESS:
        {
            // Generate TAC for 2D array access
            // Need to compute: base + (indexX * sizeY + indexY) * 4
            char* indexXExpr = generateTACExpr(node->data.array_2d_access.indexX);
            char* indexYExpr = generateTACExpr(node->data.array_2d_access.indexY);
            char* temp = newTemp();
    
            // Create a TAC instruction with both indices
            // We'll store both indices in a special format: "indexX,indexY"
            char* combinedIndex = malloc(50);
            sprintf(combinedIndex, "%s,%s", indexXExpr, indexYExpr);
    
            appendTAC(createTAC(TAC_ARRAY_2D_ACCESS, combinedIndex, node->data.array_2d_access.name, temp));
            return temp;
        }

        case NODE_FUNC_CALL: {
            // Generate TAC for arguments first
            ASTNode* arg = node->data.func_call.args;
            int argCount = 0;
            
            while (arg) {
                if (arg->type == NODE_ARG_LIST) {
                    char* argExpr = generateTACExpr(arg->data.arg_list.expr);
                    appendTAC(createTAC(TAC_ARG, argExpr, NULL, NULL));
                    argCount++;
                    arg = arg->data.arg_list.next;
                } else {
                    char* argExpr = generateTACExpr(arg);
                    appendTAC(createTAC(TAC_ARG, argExpr, NULL, NULL));
                    argCount++;
                    break;
                }
            }
            
            // Generate the call
            char* temp = newTemp();
            char argCountStr[20];
            sprintf(argCountStr, "%d", argCount);
            appendTAC(createTAC(TAC_CALL, node->data.func_call.name, argCountStr, temp));
            
            return temp;
        }
        
        default:
            return NULL;
    }
}

void generateTAC(ASTNode* node) {
    if (!node) return;
    
    switch(node->type) {
        case NODE_DECL:
            appendTAC(createTAC(TAC_DECL, NULL, NULL, node->data.name));
            break;
            
        case NODE_ASSIGN: {
            char* expr = generateTACExpr(node->data.assign.value);
            appendTAC(createTAC(TAC_ASSIGN, expr, NULL, node->data.assign.var));
            break;
        }
        case NODE_DECL_ASSIGN: {
            // First declare the variable
            appendTAC(createTAC(TAC_DECL, NULL, NULL, node->data.declAssign.id));
            
            // Then generate expression and assign
            char* expr = generateTACExpr(node->data.declAssign.expr);
            appendTAC(createTAC(TAC_ASSIGN, expr, NULL, node->data.declAssign.id));
            break;
        }
        
        case NODE_PRINT: {
            char* expr = generateTACExpr(node->data.expr);
            appendTAC(createTAC(TAC_PRINT, expr, NULL, NULL));
            break;
        }
        
        case NODE_STMT_LIST:
            generateTAC(node->data.stmtlist.stmt);
            generateTAC(node->data.stmtlist.next);
            break;

        case NODE_ARRAY_DECL:
            appendTAC(createTAC(TAC_ARRAY_DECL, NULL, NULL, node->data.array_decl.name));
            break;

        case NODE_ARRAY_ASSIGN:
            char* indexExpr = generateTACExpr(node->data.array_assign.index);
            char* valueExpr = generateTACExpr(node->data.array_assign.value);
            appendTAC(createTAC(TAC_ARRAY_ASSIGN, indexExpr, valueExpr,
               node->data.array_assign.name));
            break;
        

        case NODE_ARRAY_2D_DECL:
        {
            int rows = node->data.array_2d_decl.sizeX;
            int cols = node->data.array_2d_decl.sizeY;
            
            printf("DEBUG: Processing 2D array '%s' declaration [%d][%d]\n", 
                   node->data.array_2d_decl.name, rows, cols);
            
            // Generate the TAC instruction
            appendTAC(createTAC(TAC_ARRAY_2D_DECL, NULL, NULL, 
                      node->data.array_2d_decl.name));
            
            // ✅ Verify the symbol table lookup works:
            int checkCols = getArray2DSizeY(node->data.array_2d_decl.name);
            printf("DEBUG: getArray2DSizeY('%s') returned: %d (expected %d)\n", 
                   node->data.array_2d_decl.name, checkCols, cols);
            
            break;
        }

        
        case NODE_ARRAY_2D_ELEM_ASSIGN:{
            // Generate TAC for the value being assigned
            char* valueExpr = generateTACExpr(node->data.array_2d_elem_assign.value);

            // Generate TAC for the indices
            char* indexXExpr = generateTACExpr(node->data.array_2d_elem_assign.indexX);
            char* indexYExpr = generateTACExpr(node->data.array_2d_elem_assign.indexY);

            // Look up the number of columns from the symbol table
            int numCols = getArray2DSizeY(node->data.array_2d_elem_assign.name);

            // Multiply: t1 = indexX * numCols
            char* t1 = newTemp();
            char numColsStr[20];
            sprintf(numColsStr, "%d", numCols);
            appendTAC(createTAC(TAC_MUL, indexXExpr, numColsStr, t1));

            // Add: t2 = t1 + indexY
            char* t2 = newTemp();
            appendTAC(createTAC(TAC_ADD, t1, indexYExpr, t2));

            // Final 1D array assignment: arr[t2] = valueExpr
            appendTAC(createTAC(TAC_ARRAY_ASSIGN, t2, valueExpr,
                node->data.array_2d_elem_assign.name));
            break;
        }

        case NODE_FUNC_DECL: {
            // Function: returnType funcName(params) { body }
            appendTAC(createTAC(TAC_FUNC_DECL, 
                               node->data.func_decl.returnType,
                               NULL,
                               node->data.func_decl.name));
            
            // Generate parameter declarations
            ASTNode* param = node->data.func_decl.params;
            while (param) {
                if (param->type == NODE_PARAM_LIST) {
                    generateTAC(param->data.param_list.param);
                    param = param->data.param_list.next;
                } else if (param->type == NODE_PARAM) {
                    appendTAC(createTAC(TAC_PARAM,
                                       param->data.param.type,
                                       NULL,
                                       param->data.param.name));
                    break;
                } else {
                    break;
                }
            }
            
            // Mark function body start
            appendTAC(createTAC(TAC_FUNC_BEGIN, NULL, NULL, node->data.func_decl.name));
            
            // Generate body
            generateTAC(node->data.func_decl.body);
            
            // Mark function end
            appendTAC(createTAC(TAC_FUNC_END, NULL, NULL, node->data.func_decl.name));
            break;
        }

        case NODE_PARAM: {
            appendTAC(createTAC(TAC_PARAM,
                               node->data.param.type,
                               NULL,
                               node->data.param.name));
            break;
        }
        
        case NODE_BLOCK: {
            generateTAC(node->data.block.stmts);
            break;
        }
        
        case NODE_RETURN: {
            char* expr = generateTACExpr(node->data.ret.value);
            appendTAC(createTAC(TAC_RETURN, expr, NULL, NULL));
            break;
        }
        
        case NODE_FUNC_CALL: {
            // This is handled in generateTACExpr for function calls in expressions
            // If it's a statement (discarded return value), handle here
            generateTACExpr(node);
            // Result is ignored for statement-level calls
            break;
        }
            
        default:
            break;
    }
}

void printTAC() {
    printf("Unoptimized TAC Instructions:\n");
    printf("─────────────────────────────\n");
    TACInstr* curr = tacList.head;
    int lineNum = 1;
    while (curr) {
        printf("%2d: ", lineNum++);
        switch(curr->op) {
            case TAC_DECL:
                printf("DECL %s", curr->result);
                printf("          // Declare variable '%s'\n", curr->result);
                break;
            case TAC_ADD:
                printf("%s = %s + %s", curr->result, curr->arg1, curr->arg2);
                printf("     // Add: store result in %s\n", curr->result);
                break;
            case TAC_SUB:
                printf("%s = %s - %s", curr->result, curr->arg1, curr->arg2);
                printf("     // Subtract: store result in %s\n", curr->result);
                break;
            case TAC_MUL:
                printf("%s = %s * %s", curr->result, curr->arg1, curr->arg2);
                printf("     // Multiply: store result in %s\n", curr->result);
                break;
            case TAC_DIV:
                printf("%s = %s / %s", curr->result, curr->arg1, curr->arg2);
                printf("     // Divide: store result in %s\n", curr->result);
                break;
            case TAC_ASSIGN:
                printf("%s = %s", curr->result, curr->arg1);
                printf("           // Assign value to %s\n", curr->result);
                break;
            case TAC_PRINT:
                printf("PRINT %s", curr->arg1);
                printf("          // Output value of %s\n", curr->arg1);
                break;
            default:
                break;
            case TAC_ARRAY_DECL:
                printf("ARRAY_DECL %s", curr->result);
                printf("     // Declare array '%s'\n", curr->result);
                break;

            case TAC_ARRAY_ASSIGN:
                printf("%s[%s] = %s", curr->result, curr->arg1, curr->arg2);
                printf("   // Array assignment\n");
                break;

            case TAC_ARRAY_ACCESS:
                printf("%s = array[%s]", curr->result, curr->arg1);
                printf("  // Array access\n");
                break;
            case TAC_ARRAY_2D_DECL:
                printf("ARRAY_2D_DECL %s", curr->result);
                printf("     // Declare 2D array '%s'\n", curr->result);
                break;
            case TAC_ARRAY_2D_ACCESS:
                printf("%s = %s[%s]", curr->result, curr->arg2, curr->arg1);
                printf("  // 2D Array access\n");
                break;
            // functions
            case TAC_FUNC_DECL:
                printf("FUNC %s %s\n", curr->arg1, curr->result);
                break;
            case TAC_FUNC_BEGIN:
                printf("BEGIN_FUNC %s\n", curr->result);
                break;
            case TAC_FUNC_END:
                printf("END_FUNC %s\n", curr->result);
                break;
            case TAC_PARAM:
                printf("PARAM %s %s\n", curr->arg1, curr->result);
                break;
            case TAC_ARG:
                printf("ARG %s\n", curr->arg1);
                break;
            case TAC_CALL:
                printf("%s = CALL %s(%s args)\n", curr->result, curr->arg1, curr->arg2);
                break;
            case TAC_RETURN:
                printf("RETURN %s\n", curr->arg1);
                break;
        }
        curr = curr->next;
    }
}

// Simple optimization: constant folding and copy propagation
void optimizeTAC() {
    TACInstr* curr = tacList.head;
    int valueCount = 0;
    
    while (curr) {
        TACInstr* newInstr = NULL;
        
        switch(curr->op) {
            case TAC_DECL:
                newInstr = createTAC(TAC_DECL, NULL, NULL, curr->result);
                break;
                
            case TAC_ADD: // DONE
            {
                // Check if both operands are constants
                char* left = curr->arg1;
                char* right = curr->arg2;
                
                // Look up values in propagation table (search from most recent)
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && left && strcmp(values[i].var, left) == 0) {
                        left = values[i].value;
                        break;
                    }
                }
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && right && strcmp(values[i].var, right) == 0) {
                        right = values[i].value;
                        break;
                    }
                }
                
                // Constant folding
                if (isdigit(left[0]) && isdigit(right[0])) {
                    int result = atoi(left) + atoi(right);
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);
                    
                    // Store for propagation
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;
                    
                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    newInstr = createTAC(TAC_ADD, left, right, curr->result);
                }
                break;
            }

            case TAC_SUB: // DONE
            {
                // Check if both operands are constants
                char* left = curr->arg1;
                char* right = curr->arg2;
                
                // Look up values in propagation table (search from most recent)
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && left && strcmp(values[i].var, left) == 0) {
                        left = values[i].value;
                        break;
                    }
                }
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && right && strcmp(values[i].var, right) == 0) {
                        right = values[i].value;
                        break;
                    }
                }
                
                // Constant folding
                if (isdigit(left[0]) && isdigit(right[0])) {
                    int result = atoi(left) - atoi(right);
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);
                    
                    // Store for propagation
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;
                    
                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    newInstr = createTAC(TAC_SUB, left, right, curr->result);
                }
                break;
            }

            case TAC_MUL: // DONE
            {
                char* left = curr->arg1;
                char* right = curr->arg2;

                // Look up values in propagation table
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && left && strcmp(values[i].var, left) == 0) {
                        left = values[i].value;
                        break;
                    }
                }
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && right && strcmp(values[i].var, right) == 0) {
                        right = values[i].value;
                        break;
                    }
                }

                // Constant folding
                if (isdigit(left[0]) && isdigit(right[0])) {
                    int result = atoi(left) * atoi(right);
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);

                    // Store for propagation
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;

                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    newInstr = createTAC(TAC_MUL, left, right, curr->result);
                }
                break;
            }

            case TAC_DIV: // DONE
            {
                char* left = curr->arg1;
                char* right = curr->arg2;

                // Look up values in propagation table
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && left && strcmp(values[i].var, left) == 0) {
                        left = values[i].value;
                        break;
                    }
                }
                for (int i = valueCount - 1; i >= 0; i--) {
                    if (values[i].var && right && strcmp(values[i].var, right) == 0) {
                        right = values[i].value;
                        break;
                    }
                }

                // Constant folding
                if (isdigit(left[0]) && isdigit(right[0]) && atoi(right) != 0) {
                    int result = atoi(left) / atoi(right);
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);

                    // Store for propagation
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;

                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    newInstr = createTAC(TAC_DIV, left, right, curr->result);
                }
                break;
            }
            
            case TAC_ASSIGN: {
                char* value = curr->arg1;
    
                if (value) {
                    for (int i = valueCount - 1; i >= 0; i--) {
                        if (values[i].var && strcmp(values[i].var, value) == 0) {
                            value = values[i].value;
                            break;
                        }
                    }
                }
    
                // Store for propagation
                values[valueCount].var = strdup(curr->result);
                values[valueCount].value = value ? strdup(value) : strdup("0");
                valueCount++;
    
                newInstr = createTAC(TAC_ASSIGN, value, NULL, curr->result);
                break;
            }

            
            case TAC_PRINT: // DONE
            {
                char* value = curr->arg1;
                
                // Look up value in propagation table
                for (int i = valueCount - 1; i >= 0; i--) {  // Search from most recent
                    if (strcmp(values[i].var, value) == 0) {
                        value = values[i].value;
                        break;
                    }
                }
                
                newInstr = createTAC(TAC_PRINT, value, NULL, NULL);
                break;
            }

            case TAC_ARRAY_DECL: {
                /* curr->arg1 is the size expression or NULL for "unsized" arrays */
                char* sizeStr = curr->arg1 ? propagateValue(curr->arg1) : NULL;

                /* If size is numeric after propagation, we can treat it as a constant size */
                if (sizeStr && isdigit(sizeStr)) {
                    /* Optionally duplicate if createTAC/store expects ownership of arg strings */
                    char* constSize = strdup(sizeStr);
                    newInstr = createTAC(TAC_ARRAY_DECL, constSize, NULL, curr->result);
                    /* If createTAC copies the string internally you could free(constSize) here. */
                } else {
                    /* dynamic size (variable or unknown) -- just pass through */
                    newInstr = createTAC(TAC_ARRAY_DECL, sizeStr ? strdup(sizeStr) : NULL, NULL, curr->result);
                }
                
                break;
            }
            
            case TAC_ARRAY_ASSIGN: 
            {
                char* index = curr->arg1;
                char* value = curr->arg2;
                if (index) {
                    for (int i = valueCount - 1; i >= 0; i--) {
                        if (values[i].var && strcmp(values[i].var, index) == 0) {
                            index = values[i].value;
                            break;
                        }
                    }
                }
    
                if (value) {
                    for (int i = valueCount - 1; i >= 0; i--) {
                        if (values[i].var && strcmp(values[i].var, value) == 0) {
                            value = values[i].value;
                            break;
                        }
                    }
                }
    
                newInstr = createTAC(TAC_ARRAY_ASSIGN, index, value, curr->result);
                break;
            }

            case TAC_ARRAY_ACCESS: // DONE
            {
                char* index = propagateValue(curr->arg1);
                char* arrayName = curr->arg2;

                newInstr = createTAC(TAC_ARRAY_ACCESS, strdup(index), arrayName, curr->result);

                // Optional: propagate constant array element value
                // const Symbol* info = lookupSymbol(curr->result);
                // if (info && info->isArray && isdigit(index[0])) {
                //     int idx = atoi(index);
                //     if (idx >= 0 && idx < info->arraySize) {
                //         char valueStr[20];
                //         sprintf(valueStr, "%d", info->arrayValues[idx]);
                //         values[valueCount++] = (ValueProp){ strdup(curr->result), strdup(valueStr) };
                //     }
                // }
                break;
            }

            case TAC_ARRAY_EXPR: 
            {
                // During optimization, TAC_ARRAY_EXPR should just be propagated through
                // The actual array element assignments should already exist in the TAC stream
    
                // Simply propagate any values if needed
                char* arrayName = curr->result;
    
                // Just pass through - the individual array assignments will be optimized separately
                newInstr = createTAC(TAC_ARRAY_EXPR, curr->arg1, curr->arg2, arrayName);
                break;
            }

            case TAC_ARRAY_2D_DECL:
                newInstr = createTAC(TAC_ARRAY_2D_DECL, NULL, NULL, curr->result);
                break;
            
            case TAC_ARRAY_2D_ACCESS:
            {
                char* indices = curr->arg1;
    
                // Try to optimize the indices if they're in propagation table
                // For now, just pass through - 2D index calculation is complex
                newInstr = createTAC(TAC_ARRAY_2D_ACCESS, indices, curr->arg2, curr->result);
                break;
            }
            case TAC_FUNC_DECL:
                newInstr = createTAC(TAC_FUNC_DECL, curr->arg1, curr->arg2, curr->result);
                // Reset propagation table at function boundaries
                valueCount = 0;
                break;
            
            case TAC_FUNC_BEGIN:
                newInstr = createTAC(TAC_FUNC_BEGIN, NULL, NULL, curr->result);
                break;
            
            case TAC_FUNC_END:
                newInstr = createTAC(TAC_FUNC_END, NULL, NULL, curr->result);
                break;
            
            case TAC_PARAM:
                newInstr = createTAC(TAC_PARAM, curr->arg1, NULL, curr->result);
                break;
            
            case TAC_ARG: {
                char* value = propagateValue(curr->arg1);
                newInstr = createTAC(TAC_ARG, value, NULL, NULL);
                break;
            }
            
            case TAC_CALL:
                // Don't propagate through function calls (unknown side effects)
                newInstr = createTAC(TAC_CALL, curr->arg1, curr->arg2, curr->result);
                break;
            
            case TAC_RETURN: {
                char* value = propagateValue(curr->arg1);
                newInstr = createTAC(TAC_RETURN, value, NULL, NULL);
                break;
            }
            case TAC_LABEL:
                // Labels are just passed through, no optimization
                newInstr = createTAC(TAC_LABEL, curr->arg1, curr->arg2, curr->result);
                break;

        }
        
        if (newInstr) {
            appendOptimizedTAC(newInstr);
        }
        
        curr = curr->next;
    }
}

void printOptimizedTAC() {
    printf("Optimized TAC Instructions:\n");
    printf("─────────────────────────────\n");
    TACInstr* curr = optimizedList.head;
    int lineNum = 1;
    while (curr) {
        printf("%2d: ", lineNum++);
        switch(curr->op) {
            case TAC_DECL:
                printf("DECL %s\n", curr->result);
                break;
            case TAC_ADD:
                printf("%s = %s + %s     // Runtime addition needed\n", 
                       curr->result, curr->arg1, curr->arg2);
                break;
            case TAC_SUB:
                printf("%s = %s - %s     // Runtime subtraction needed\n",
                       curr->result, curr->arg1, curr->arg2);
                break;
            case TAC_MUL:
                printf("%s = %s * %s     // Runtime multiplication needed\n",
                       curr->result, curr->arg1, curr->arg2);
                break;
            case TAC_DIV:
                printf("%s = %s / %s     // Runtime division needed\n",
                       curr->result, curr->arg1, curr->arg2);
                break;
            case TAC_ASSIGN:
                printf("%s = %s", curr->result, curr->arg1);
                if (curr->arg1 && isdigit(curr->arg1[0])) {
                    printf("           // Constant value: %s\n", curr->arg1);
                } else {
                    printf("           // Copy value\n");
                }
                break;
            case TAC_PRINT:
                printf("PRINT %s", curr->arg1);
                if (curr->arg1 && isdigit(curr->arg1[0])) {
                    printf("          // Print constant: %s\n", curr->arg1);
                } else {
                    printf("          // Print variable\n");
                }
                break;
                
            // arrays
            case TAC_ARRAY_DECL:
                printf("ARRAY_DECL %s\n", curr->result);
                break;
            case TAC_ARRAY_ASSIGN:
                printf("%s[%s] = %s\n", curr->result, curr->arg1, curr->arg2);
                break;
            case TAC_ARRAY_ACCESS:
                printf("%s = %s[%s]\n", curr->result, curr->arg2, curr->arg1);
                break;
                
            case TAC_ARRAY_2D_DECL:
                printf("ARRAY_2D_DECL %s\n", curr->result);
                break;
            case TAC_ARRAY_2D_ACCESS:
                printf("%s = %s[%s]\n", curr->result, curr->arg2, curr->arg1);
                break;

            // functions
            case TAC_FUNC_DECL:
                printf("FUNC %s %s\n", curr->arg1, curr->result);
                break;
            case TAC_FUNC_BEGIN:
                printf("BEGIN_FUNC %s\n", curr->result);
                break;
            case TAC_FUNC_END:
                printf("END_FUNC %s\n", curr->result);
                break;
            case TAC_PARAM:
                printf("PARAM %s %s\n", curr->arg1, curr->result);
                break;
            case TAC_ARG:
                printf("ARG %s\n", curr->arg1);
                break;
            case TAC_CALL:
                printf("%s = CALL %s(%s args)\n", curr->result, curr->arg1, curr->arg2);
                break;
            case TAC_RETURN:
                printf("RETURN %s\n", curr->arg1);
                break;
                
            default:
                printf("UNKNOWN_OP_%d\n", curr->op);  // ✅ Debug unknown ops
                break;
        }
        curr = curr->next;
    }
}