/* SYMBOL TABLE IMPLEMENTATION
 * Manages variable declarations and lookups
 * Essential for semantic analysis (checking if variables are declared)
 * Provides memory layout information for code generation
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* Global symbol table instance */
SymbolTable symtab;

/* Global Function Table*/
FunctionTable funcTable;

/* Initialize an empty symbol table */
void initSymTab() {
    symtab.count = 0;       /* No variables yet */
    symtab.nextOffset = 0;  /* Start at stack offset 0 */
    symtab.currentScope = 0; /* Start at global scope */
    symtab.scopeOffsets[0] = 0; /* Global scope offset */
}

/* NEW = Enter new scope (when entering function or block)*/
void enterScope(){
    symtab.currentScope++;
    /*Save current offset for when we exit this scope*/
    symtab.scopeOffsets[symtab.currentScope] = symtab.nextOffset;

    printf("SCOPE = Entering scope %d (offset: %d)\n", symtab.currentScope, symtab.nextOffset);
}

/*Exit current scope (when leaving function or block)*/
void exitScope(){
    printf("SCOPE = Exiting scope %d\n", symtab.currentScope);

    /*Remove all variables declared in this scope*/
    int i = symtab.count - 1;
    while (i>= 0 && symtab.vars[i].scope >= symtab.currentScope){
        printf("SCOPE = Removing variable %s from scope %d\n", symtab.vars[i].name, symtab.currentScope);
        free(symtab.vars[i].name); // Free the allocated name
        symtab.count--;
        i--;
    }

    /*Restore offset from before this scope*/
    symtab.currentScope--;
    if (symtab.currentScope >= 0){
        symtab.nextOffset = symtab.scopeOffsets[symtab.currentScope];
    } else {
        symtab.nextOffset = 0; // Should not happen, but just in case
        symtab.currentScope = 0;
    }
}

/*Get Current scope level*/
int getCurrentScope(){
    return symtab.currentScope;
}

/*Check if variable is declared in current scope*/
int isVarDeclaredInCurrentScope(char* name){
    for (int i = 0; i < symtab.count; i++) {
        if (strcmp(symtab.vars[i].name, name) == 0 && symtab.vars[i].scope == symtab.currentScope) {
            return 1;  /* Found in current scope */
        }
    }
    return 0;  /* Not found in current scope */
}


/* Add a new variable to the symbol table */
int addVar(char* name) {
    /* Check for duplicate declaration */
    if (isVarDeclaredInCurrentScope(name)) {
        printf("SYMTAB ERROR: Variable %s already declared in current scope\n", name);
        return -1;  /* Error: variable already exists */
    }
    
    /* Add new symbol entry */
    symtab.vars[symtab.count].name = strdup(name);
    symtab.vars[symtab.count].offset = symtab.nextOffset;
    symtab.vars[symtab.count].scope = symtab.currentScope; /* Set scope level */
    
    /* Advance offset by 4 bytes (size of int in MIPS) */
    symtab.nextOffset += 4;
    symtab.count++;
    
    /* Return the offset for this variable */
    return symtab.vars[symtab.count - 1].offset;
}

/* Look up a variable's stack offset */
int getVarOffset(char* name) {
    /*Search Backwards (most recent declarations first)*/
    /*This implements shadowing: inner scope varaibles hide outer ones*/

    for (int i = symtab.count -1; i>=0; i--) {
        if (strcmp(symtab.vars[i].name, name) == 0) {
            return symtab.vars[i].offset;  /* Found it */
        }
    }
    return -1;  /* Variable not found - semantic error */
}

/* Check if a variable has been declared */
int isVarDeclared(char* name) {
    return getVarOffset(name) != -1;  /* True if found, false otherwise */
}

/* ##### ARRAYS ##### */
int addArrayVar(char* name, int size) {
    /* Check for duplicate declaration */
    if (isVarDeclaredInCurrentScope(name)) {
        printf("SYMTAB ERROR: Variable %s already declared in current scope\n", name);
        return -1;  /* Error: variable already exists */
    }
    
    /* Add new symbol entry */
    symtab.vars[symtab.count].name = strdup(name);
    symtab.vars[symtab.count].offset = symtab.nextOffset;
    symtab.vars[symtab.count].isArray = 1; // Mark as array
    symtab.vars[symtab.count].arraySize = size; // Store array size
    symtab.vars[symtab.count].scope = symtab.currentScope; /* NEW Set scope level */
    
    /* Advance offset by size * 4 bytes (size of int in MIPS) */
    symtab.nextOffset += size * 4;
    symtab.count++;

    printf("SYMTAB: Added array '%s[%d]' at scope %d, offset %d\n", name, size, symtab.currentScope, symtab.vars[symtab.count - 1].offset);
    
    /* Return the offset for this array variable */
    return symtab.vars[symtab.count - 1].offset;
}

int isArrayVar(char* name) {
    for (int i = 0; i < symtab.count; i++) {
        if (strcmp(symtab.vars[i].name, name) == 0) {
            return symtab.vars[i].isArray;  /* Return 1 if array, 0 if not */
        }
    }
    return 0;  /* Variable not found or not an array */
}

int getArraySize(char* name) {
    for (int i = 0; i < symtab.count; i++) {
        if (strcmp(symtab.vars[i].name, name) == 0) {
            if (symtab.vars[i].isArray) {
                return symtab.vars[i].arraySize;  /* Return size if array */
            } else {
                return -1;  /* Not an array */
            }
        }
    }
    return 0;  /* Variable not found */
}

int addArray2DVar(char* name, int sizeX, int sizeY)
{
    /* Check for duplicate declaration IN CURRENT SCOPE*/
    if (isVarDeclaredInCurrentScope(name)) {
        printf("SYMTAB ERROR: Variable %s already declared in current scope\n", name);
        return -1;  /* Error: variable already exists */
    }
    
    /* Add new symbol entry */
    symtab.vars[symtab.count].name = strdup(name);
    symtab.vars[symtab.count].offset = symtab.nextOffset;
    symtab.vars[symtab.count].isArray = 1; // Mark as array
    symtab.vars[symtab.count].array2DSizeX = sizeX;
    symtab.vars[symtab.count].array2DSizeY = sizeY;
    symtab.vars[symtab.count].scope = symtab.currentScope; /* NEW Store scope level */
    
    /* Advance offset by size^2 * 4 bytes (size of int in MIPS) */
    symtab.nextOffset += (sizeX * sizeY) * 4;
    symtab.count++;

    printf("SYMTAB: Added 2D array '%s[%d][%d]' at scope %d, offset %d\n", name, sizeX, sizeY, symtab.currentScope, symtab.vars[symtab.count - 1].offset);
    
    /* Return the offset for this array variable */
    return symtab.vars[symtab.count - 1].offset;
}

/* Get the X dimension of a 2D array */
int getArray2DSizeX(char* name) {
    for (int i = 0; i < symtab.count; i++) {
        if (strcmp(symtab.vars[i].name, name) == 0) {
            if (symtab.vars[i].isArray) {
                return symtab.vars[i].array2DSizeX;
            } else {
                return -1;  /* Not an array */
            }
        }
    }
    return -1;  /* Variable not found */
}

/* Get the Y dimension of a 2D array */
int getArray2DSizeY(char* name) {
    for (int i = 0; i < symtab.count; i++) {
        if (strcmp(symtab.vars[i].name, name) == 0) {
            if (symtab.vars[i].isArray) {
                return symtab.vars[i].array2DSizeY;
            } else {
                return -1;  /* Not an array */
            }
        }
    }
    return -1;  /* Variable not found */
}

/* Check if a variable is a 2D array */
int is2DArrayVar(char* name) {
    for (int i = 0; i < symtab.count; i++) {
        if (strcmp(symtab.vars[i].name, name) == 0) {
            if (symtab.vars[i].isArray && 
                symtab.vars[i].array2DSizeX > 0 && 
                symtab.vars[i].array2DSizeY > 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* NEW Function table operations*/
int addFunction(char*name, char* returnType){
    /*Check for duplicate declaration*/
    if (isFunctionDeclared(name)){
        printf("SYMTAB ERROR: Function %s already declared\n", name);
        return -1; // Error: function already exists
    }

    /*Add new function entry*/
    funcTable.funcs[funcTable.count].name = strdup(name);
    funcTable.funcs[funcTable.count].returnType = strdup(returnType);
    funcTable.funcs[funcTable.count].paramCount = 0; // No parameters yet
    funcTable.count++;

    printf("SYMTAB: Added function '%s' with return type '%s'\n", name, returnType);
    return 0; // Success
}

int isFunctionDeclared(char* name){
    for (int i = 0; i < funcTable.count; i++){
        if (strcmp(funcTable.funcs[i].name, name) == 0){
            return 1; // Found
        }
    }
    return 0; // Not found
}

void addFunctionParam(char* funcName, char* paramType, char* paramName){
    for (int i = 0; i < funcTable.count; i++){
        if (strcmp(funcTable.funcs[i].name, funcName) == 0){
            int paramIndex = funcTable.funcs[i].paramCount;
            if (paramIndex < 10){ // Max 10 parameters
                funcTable.funcs[i].paramNames[paramIndex] = strdup(paramName);
                funcTable.funcs[i].paramTypes[paramIndex] = strdup(paramType);
                funcTable.funcs[i].paramCount++;
                printf("SYMTAB: Added parameter '%s %s' to function '%s'\n", paramType, paramName, funcName);
            } else {
                printf("SYMTAB ERROR: Function %s has too many parameters\n", funcName);
            }
            return;
        }
    }
    printf("SYMTAB ERROR: Function %s not found when adding parameter\n", funcName);
}

int getFunctionParamCount(char* name){
    for (int i = 0; i < funcTable.count; i++){
        if (strcmp(funcTable.funcs[i].name, name) == 0){
            return funcTable.funcs[i].paramCount;
        }
    }
    return -1; // Function not found
}

char* getFunctionReturnType(char* name){
    for (int i = 0; i < funcTable.count; i++){
        if (strcmp(funcTable.funcs[i].name, name) == 0){
            return funcTable.funcs[i].returnType;
        }
    }
    return NULL; // Function not found
}

/* lookupSymbol removed: use getVarOffset/isVarDeclared/isVarDeclaredInCurrentScope
   or other accessor functions to query symbols. */
