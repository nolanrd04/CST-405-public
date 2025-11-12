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
    tacList.labelCount = 0;
    optimizedList.head = NULL;
    optimizedList.tail = NULL;
}

char* newTemp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", tacList.tempCount++);
    return temp;
}

char* newLabel() {
    char* label = malloc(10);
    sprintf(label, "L%d", tacList.labelCount++);
    return strdup(label);
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

/*Generate TAC for parameter list*/
void generateParamListTAC(ASTNode* node, char* funcName){
    if(!node) return;

    if(node->type == NODE_PARAM){
        //Single Parameter
        appendTAC(createTAC(TAC_PARAM, node->data.param.type, node->data.param.name, funcName));
    }else if (node->type == NODE_PARAM_LIST){
        //Process first parameter
        if (node->data.param_list.param){
            generateParamListTAC(node->data.param_list.param, funcName);
        }
        //Process rest of parameters
        if(node->data.param_list.next){
            generateParamListTAC(node->data.param_list.next, funcName);
        }
    }
}

char* generateTACExpr(ASTNode* node) {
    if (!node) return NULL;
    
    switch(node->type) {
        case NODE_NUM: {
            char* temp = malloc(20);
            if (node->data.num.is_float) {
                snprintf(temp, 32, "%g", node->data.num.value.fval);   // float -> string
            } else {
                snprintf(temp, 32, "%d", node->data.num.value.ival);   // int -> string
            }
            return temp;
        }
        
        case NODE_VAR:
            return strdup(node->data.name);
        
        case NODE_BINOP: {
            char* left = generateTACExpr(node->data.binop.left);
            char* right = generateTACExpr(node->data.binop.right);
            char* temp = newTemp();

            switch (node->data.binop.op) {
                case OP_ADD:
                    appendTAC(createTAC(TAC_ADD, left, right, temp));
                    break;
                case OP_SUB:
                    appendTAC(createTAC(TAC_SUB, left, right, temp));
                    break;
                case OP_MUL:
                    appendTAC(createTAC(TAC_MUL, left, right, temp));
                    break;
                case OP_DIV:
                    appendTAC(createTAC(TAC_DIV, left, right, temp));
                    break;

                // Comparison operators
                case OP_EQ:
                    appendTAC(createTAC(TAC_EQ, left, right, temp));
                    break;
                case OP_NEQ:
                    appendTAC(createTAC(TAC_NEQ, left, right, temp));
                    break;
                case OP_LT:
                    appendTAC(createTAC(TAC_LT, left, right, temp));  
                    break;
                case OP_GT:
                    appendTAC(createTAC(TAC_GT, left, right, temp));  
                    break;
                case OP_LTE:
                    appendTAC(createTAC(TAC_LTE, left, right, temp)); 
                    break;
                case OP_GTE:
                    appendTAC(createTAC(TAC_GTE, left, right, temp)); 
                    break;
                default:
                    // Handles unsupported operators
                    break;
            }
            /* Redundant operation handling
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
            */
            
            return temp;
        }

        case NODE_BOOL: {
            char* temp = malloc(20);
            sprintf(temp, "%d", node->data.boolVal.bool_value);
            return temp;
        }
        
        case NODE_UNARYOP: {
            char* operand = generateTACExpr(node->data.unaryop.operand);
            char* temp = newTemp();
            
            if (node->data.unaryop.op == OP_NOT) {
                appendTAC(createTAC(TAC_NOT, operand, NULL, temp));
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

void generateTAC_If(ASTNode* node) {
    if (!node) return;

    // Generate TAC for the condition expression
    char* cond = generateTACExpr(node->condition);

    // Create labels
    char* labelEnd = newLabel();

    if (node->right) {
        // If-Else case
        char* labelFalse = newLabel();

        // if condition is false, jump to false label
        appendTAC(createTAC(TAC_IFZ, cond, NULL, labelFalse));

        // Then branch
        generateTAC(node->left);
        appendTAC(createTAC(TAC_GOTO, NULL, NULL, labelEnd));

        // Else branch
        appendTAC(createTAC(TAC_LABEL, labelFalse, NULL, NULL));
        generateTAC(node->right);
    } else {
        // If only
        appendTAC(createTAC(TAC_IFZ, cond, NULL, labelEnd));
        generateTAC(node->left);
    }

    // End label
    appendTAC(createTAC(TAC_LABEL, labelEnd, NULL, NULL));
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
                }
            }
            break;
        }
        
        case NODE_BLOCK: {
            generateTAC(node->data.block.stmts);
            break;
        }

        case NODE_SWITCH: {
            /*EValuate switch expression */
            char* switchExpr = generateTACExpr(node->data.switch_stmt.expr);

            /* Genereate end lavel */
            char* endLabel = newTemp();

            /*Process all cases*/
            ASTNode* caseNode = node->data.switch_stmt.cases;
            char* defaultLabel = NULL;

            /*First pass: generate case comparisons*/
            while(caseNode){
                if(caseNode->type == NODE_CASE_LIST ){
                    ASTNode* currentCase = caseNode->data.case_list.case_item;

                    if(currentCase->type == NODE_CASE){
                        /*Generate label for this case */
                        char* caseLabel = newTemp();
                        char*caseValue = malloc(20);
                        sprintf(caseValue, "%d", currentCase->data.case_stmt.value);

                        /*Compare switch expr with case value*/
                        char* cmpResult = newTemp();
                        appendTAC(createTAC(TAC_EQ, switchExpr, caseValue, cmpResult));

                        /*If equal, goto case label*/
                        appendTAC(createTAC(TAC_IF_FALSE, cmpResult, caseLabel, NULL));
                        appendTAC(createTAC(TAC_GOTO, caseLabel, NULL, NULL));

                        /* Store Label for later */
                        appendTAC(createTAC(TAC_LABEL, caseLabel, NULL, NULL));
                        generateTAC(currentCase->data.case_stmt.stmts);
                        appendTAC(createTAC(TAC_GOTO,endLabel, NULL, NULL));
                    }else if(currentCase->type == NODE_DEFAULT_CASE){
                        defaultLabel = newTemp();
                        appendTAC(createTAC(TAC_LABEL,defaultLabel,NULL,NULL));
                        generateTAC(currentCase->data.default_case.stmts);
                    }
                    caseNode = caseNode->data.case_list.next;
                

                    
                } else if(caseNode->type == NODE_CASE){
                    /*Single case*/
                    char* caseLabel = newTemp();
                    char* caseValue = malloc(20);
                    sprintf(caseValue, "%d", caseNode->data.case_stmt.value);

                    /*Compare switch expr with case value*/
                    char* cmpResult = newTemp();
                    appendTAC(createTAC(TAC_EQ, switchExpr, caseValue, cmpResult));

                    /*If equal, goto case label*/
                    appendTAC(createTAC(TAC_IF_FALSE, cmpResult, caseLabel, NULL));
                    appendTAC(createTAC(TAC_GOTO, caseLabel, NULL, NULL));

                    /* Store Label for later */
                    appendTAC(createTAC(TAC_LABEL, caseLabel, NULL, NULL));
                    generateTAC(caseNode->data.case_stmt.stmts);
                    appendTAC(createTAC(TAC_GOTO,endLabel, NULL, NULL));
                    break;
                }else if (caseNode->type == NODE_DEFAULT_CASE) {
                    defaultLabel = newTemp();
                    appendTAC(createTAC(TAC_LABEL,defaultLabel,NULL,NULL));
                    generateTAC(caseNode->data.default_case.stmts);
                    break;
                }
            }
            /*if no case matched, jump to default or end*/
            if(defaultLabel){
                appendTAC(createTAC(TAC_GOTO, defaultLabel, NULL, NULL));
            }
            /*End label */
            appendTAC(createTAC(TAC_LABEL, endLabel, NULL, NULL));
            break;
        }
        case NODE_BREAK:{
            /*Break generates a goto to the end label*/
            appendTAC(createTAC(TAC_GOTO, "break_target",NULL, NULL));
            break;
        }

        case NODE_IF: {
            generateTAC_If(node);
            break;
        }
        
        /* ===== NEW: WHEN LOOP FEATURE ===== */
        case NODE_WHEN_STMT: {
            generateTAC_When(node);
            break;
        }
        case NODE_BREAK_WHEN: {
            // Break-when: evaluate condition, if true goto when_done
            char* exprResult = generateTACExpr(node->data.break_when.expr);
            appendTAC(createTAC(TAC_BREAK_WHEN, exprResult, NULL, NULL));
            break;
        }
        /* ===== END: WHEN LOOP FEATURE ===== */
            
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
            case TAC_AND:
                printf("%s = %s && %s", curr->result, curr->arg1, curr->arg2);
                printf(" // Logical AND\n");
                break;
            case TAC_OR:
                printf("%s = %s || %s", curr->result, curr->arg1, curr->arg2);
                printf(" // Logical OR\n");
                break;
            case TAC_NOT:
                printf("%s = !%s", curr->result, curr->arg1);
                printf(" // Logical NOT\n");
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
            case TAC_LABEL:
                printf("%s:", curr->arg1);
                printf("          // Label\n");
                break;
            case TAC_GOTO:
                printf("GOTO %s", curr->arg1);
                printf("         // Unconditional jump to %s\n", curr->arg1);
                break;
            case TAC_IF_FALSE:
                printf("IF_FALSE %s GOTO %s", curr->arg1, curr->arg2);
                printf(" // Conditional jump\n");
                break;
            case TAC_EQ:
                printf("%s = %s == %s", curr->result, curr->arg1, curr->arg2);
                printf(" // Equality comparison\n");
                break;
            case TAC_SWITCH:
                printf("SWITCH %s", curr->arg1);
                break;
            case TAC_CASE:
                printf("CASE %s:", curr->arg1);
                break;
            case TAC_DEFAULT:
                printf("DEFAULT:");
                printf("         // Switch default case\n");
                break;
            case TAC_FUNC_DECL:
                printf("FUNC %s %s", curr->arg1, curr->result);
                printf("     // Function declaration: %s returns %s\n", curr->result, curr->arg1);
                break;
            case TAC_FUNC_BEGIN:
                printf("BEGIN_FUNC %s", curr->result);
                printf("  // Begin function body\n");
                break;
            case TAC_FUNC_END:
                printf("END_FUNC %s", curr->result);
                printf("    // End function body\n");
                break;
            case TAC_PARAM:
                printf("PARAM %s %s", curr->arg1, curr->result);
                printf("   // Parameter: %s of type %s\n", curr->result, curr->arg1);
                break;
            case TAC_CALL:
                if (curr->arg2) { // arg2 contains argument count
                    printf("%s = CALL %s (%s)", curr->result, curr->arg1, curr->arg2);
                    printf(" // Call function with %s arguments\n", curr->arg2);
                } else {
                    printf("%s = CALL %s", curr->result, curr->arg1);
                    printf("     // Call function\n");
                }
                break;
            case TAC_ARG:
                printf("ARG %s", curr->arg1);
                printf("          // Function call argument\n");
                break;
            case TAC_RETURN:
                if (curr->arg1) {
                    printf("RETURN %s", curr->arg1);
                    printf("        // Return with value\n");
                } else {
                    printf("RETURN");
                    printf("            // Return void\n");
                }
                break;
            case TAC_NEQ:
                printf("%s = %s != %s", curr->result, curr->arg1, curr->arg2);
                printf(" // Not equal comparison\n");
                break;
            case TAC_LT:
                printf("%s = %s < %s", curr->result, curr->arg1, curr->arg2);
                printf("  // Less than comparison\n");
                break;
            case TAC_GT:
                printf("%s = %s > %s", curr->result, curr->arg1, curr->arg2);
                printf("  // Greater than comparison\n");
                break;
            case TAC_LTE:
                printf("%s = %s <= %s", curr->result, curr->arg1, curr->arg2);
                printf(" // Less than or equal comparison\n");
                break;
            case TAC_GTE:
                printf("%s = %s >= %s", curr->result, curr->arg1, curr->arg2);
                printf(" // Greater than or equal comparison\n");
                break;
            case TAC_IFZ:
                printf("IFZ %s GOTO %s", curr->arg1, curr->result);
                printf("   // If zero, jump to %s\n", curr->result);
                break;
            case TAC_ENTER_SCOPE:
                printf("ENTER_SCOPE");
                printf("        // Enter a new scope block\n");
                break;
            case TAC_EXIT_SCOPE:
                printf("EXIT_SCOPE");
                printf("         // Exit current scope block\n");
                break;
            default:
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
                
            case TAC_ADD:
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

            case TAC_AND: {
                char* left = propagateValue(curr->arg1);
                char* right = propagateValue(curr->arg2);
                
                /* Constant folding for boolean AND */
                if (isdigit(left[0]) && isdigit(right[0])) {
                    int result = (atoi(left) != 0) && (atoi(right) != 0);
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);
                    
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;
                    
                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    newInstr = createTAC(TAC_AND, left, right, curr->result);
                }
                break;
            }
            
            case TAC_OR: {
                char* left = propagateValue(curr->arg1);
                char* right = propagateValue(curr->arg2);
                
                /* Constant folding for boolean OR */
                if (isdigit(left[0]) && isdigit(right[0])) {
                    int result = (atoi(left) != 0) || (atoi(right) != 0);
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);
                    
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;
                    
                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    newInstr = createTAC(TAC_OR, left, right, curr->result);
                }
                break;
            }
            
            case TAC_NOT: {
                char* operand = propagateValue(curr->arg1);
                
                /* Constant folding for NOT */
                if (isdigit(operand[0])) {
                    int result = !(atoi(operand));
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);
                    
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;
                    
                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    newInstr = createTAC(TAC_NOT, operand, NULL, curr->result);
                }
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

            // Handle comparison operations with constant folding where possible
            case TAC_EQ:
            case TAC_NEQ:
            case TAC_LT:
            case TAC_GT:
            case TAC_LTE:
            case TAC_GTE:
            {
                char* left = propagateValue(curr->arg1);
                char* right = propagateValue(curr->arg2);
                
                // Constant folding for comparison operators
                if (isdigit(left[0]) && isdigit(right[0])) {
                    int leftVal = atoi(left);
                    int rightVal = atoi(right);
                    int result;
                    
                    switch(curr->op) {
                        case TAC_EQ:  result = (leftVal == rightVal); break;
                        case TAC_NEQ: result = (leftVal != rightVal); break;
                        case TAC_LT:  result = (leftVal < rightVal);  break;
                        case TAC_GT:  result = (leftVal > rightVal);  break;
                        case TAC_LTE: result = (leftVal <= rightVal); break;
                        case TAC_GTE: result = (leftVal >= rightVal); break;
                        default: result = 0; break;
                    }
                    
                    char* resultStr = malloc(20);
                    sprintf(resultStr, "%d", result);
                    
                    values[valueCount].var = strdup(curr->result);
                    values[valueCount].value = resultStr;
                    valueCount++;
                    
                    newInstr = createTAC(TAC_ASSIGN, resultStr, NULL, curr->result);
                } else {
                    // Can't optimize, pass through original comparison
                    newInstr = createTAC(curr->op, left, right, curr->result);
                }
                break;
            }

            // Handle function-related operations
            case TAC_FUNC_DECL:
            case TAC_FUNC_BEGIN:
            case TAC_FUNC_END:
            case TAC_PARAM:
            case TAC_CALL:
            case TAC_ARG:
            case TAC_RETURN:
                // Function operations generally can't be optimized away
                newInstr = createTAC(curr->op, curr->arg1, curr->arg2, curr->result);
                break;

            // Control flow operations
            case TAC_IFZ:
                newInstr = createTAC(curr->op, curr->arg1, curr->arg2, curr->result);
                break;
            case TAC_GOTO:
                newInstr = createTAC(curr->op, curr->arg1, curr->arg2, curr->result);
                break;
            case TAC_LABEL:
                newInstr = createTAC(curr->op, curr->arg1, curr->arg2, curr->result);
                break;

            // Scope operations
            case TAC_ENTER_SCOPE:
            case TAC_EXIT_SCOPE:
                newInstr = createTAC(curr->op, NULL, NULL, NULL);
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
                
            // ✅ ADD THESE MISSING CASES:
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

            case TAC_EQ:
            case TAC_NEQ:
            case TAC_LT:
            case TAC_GT:
            case TAC_LTE:
            case TAC_GTE:
                if (curr->arg1 && curr->arg2 && isdigit(curr->arg1[0]) && isdigit(curr->arg2[0])) {
                    printf("%s = %s", curr->result, curr->arg1);
                    printf("           // Optimized comparison to constant\n");
                } else {
                    const char* op;
                    switch(curr->op) {
                        case TAC_EQ:  op = "=="; break;
                        case TAC_NEQ: op = "!="; break;
                        case TAC_LT:  op = "<";  break;
                        case TAC_GT:  op = ">";  break;
                        case TAC_LTE: op = "<="; break;
                        case TAC_GTE: op = ">="; break;
                        default: op = "??"; break;
                    }
                    printf("%s = %s %s %s     // Runtime comparison needed\n", 
                          curr->result, curr->arg1, op, curr->arg2);
                }
                break;

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
            case TAC_CALL:
                if (curr->arg2) {
                    printf("%s = CALL %s (%s)\n", curr->result, curr->arg1, curr->arg2);
                } else {
                    printf("%s = CALL %s\n", curr->result, curr->arg1);
                }
                break;
            case TAC_ARG:
                printf("ARG %s\n", curr->arg1);
                break;
            case TAC_RETURN:
                printf("RETURN %s\n", curr->arg1 ? curr->arg1 : "");
                break;
            case TAC_IFZ:
                printf("IFZ %s GOTO %s\n", curr->arg1, curr->result);
                break;
            case TAC_ENTER_SCOPE:
                printf("ENTER_SCOPE\n");
                break;
            case TAC_EXIT_SCOPE:
                printf("EXIT_SCOPE\n");
                break;
            /* ===== NEW: WHEN LOOP FEATURE ===== */
            case TAC_WHEN_START:
                printf("WHEN_START %s\n", curr->result);
                break;
            case TAC_WHEN_CHECK:
                printf("WHEN_CHECK %s GOTO %s  // If primary condition true, exit loop\n", 
                       curr->arg1, curr->result);
                break;
            case TAC_WHEN_OR:
                printf("WHEN_OR %s\n", curr->arg1);
                break;
            case TAC_WHEN_END:
                printf("WHEN_END %s\n", curr->result);
                break;
            case TAC_BREAK_WHEN:
                printf("BREAK_WHEN %s  // Break when condition is true\n", curr->arg1);
                break;
            /* ===== END: WHEN LOOP FEATURE ===== */
                
            default:
                printf("UNKNOWN_OP_%d\n", curr->op);  // ✅ Debug unknown ops
                break;
        }
        curr = curr->next;
    }
}

/* ===== NEW: WHEN LOOP FEATURE ===== */
/* Generate TAC for when loop statement 
 * A when loop keeps looping until the primary condition is true
 * Before each iteration, it can execute or-branch blocks if their conditions are true
 * The break when statement can exit the loop
 */
void generateTAC_When(ASTNode* node) {
    if (!node || node->type != NODE_WHEN_STMT) return;

    // Generate labels for the when loop
    char* loopStartLabel = newLabel();
    char* loopEndLabel = newLabel();
    
    // Emit loop start label
    appendTAC(createTAC(TAC_LABEL, loopStartLabel, NULL, NULL));
    
    // Generate TAC for primary condition
    char* primaryCond = generateTACExpr(node->data.when_stmt.primaryCond);
    
    // If primary condition is true, exit loop
    appendTAC(createTAC(TAC_WHEN_CHECK, primaryCond, NULL, loopEndLabel));
    
    // Execute primary block
    generateTAC(node->data.when_stmt.primaryBlock);
    
    // Generate OR branches (if any)
    if (node->data.when_stmt.orBranches) {
        generateTAC_WhenOrList(node->data.when_stmt.orBranches);
    }
    
    // Execute else block (if any), otherwise loop back
    if (node->data.when_stmt.elseBlock) {
        generateTAC(node->data.when_stmt.elseBlock);
    }
    
    // Jump back to loop start
    appendTAC(createTAC(TAC_GOTO, NULL, NULL, loopStartLabel));
    
    // Emit loop end label
    appendTAC(createTAC(TAC_LABEL, loopEndLabel, NULL, NULL));
}

/* Generate TAC for when-or branch list */
void generateTAC_WhenOrList(ASTNode* node) {
    if (!node) return;
    
    if (node->type == NODE_WHEN_OR_LIST) {
        // Process current branch
        if (node->data.when_or_list.branch) {
            ASTNode* branch = node->data.when_or_list.branch;
            if (branch->type == NODE_WHEN_OR_BRANCH) {
                // Generate condition check
                char* branchCond = generateTACExpr(branch->data.when_or_branch.condition);
                
                // Generate label for skipping this branch if false
                char* skipLabel = newLabel();
                
                // If condition is false, skip branch
                appendTAC(createTAC(TAC_IFZ, branchCond, NULL, skipLabel));
                
                // Execute branch block
                generateTAC(branch->data.when_or_branch.block);
                
                // Skip label for when branch was false
                appendTAC(createTAC(TAC_LABEL, skipLabel, NULL, NULL));
            }
        }
        
        // Process rest of branches
        if (node->data.when_or_list.next) {
            generateTAC_WhenOrList(node->data.when_or_list.next);
        }
    } else if (node->type == NODE_WHEN_OR_BRANCH) {
        // Single branch (shouldn't happen, but handle it)
        char* branchCond = generateTACExpr(node->data.when_or_branch.condition);
        char* skipLabel = newLabel();
        appendTAC(createTAC(TAC_IFZ, branchCond, NULL, skipLabel));
        generateTAC(node->data.when_or_branch.block);
        appendTAC(createTAC(TAC_LABEL, skipLabel, NULL, NULL));
    }
}
/* ===== END: WHEN LOOP FEATURE ===== */