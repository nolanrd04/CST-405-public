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

/* Initialize an empty symbol table */
void initSymTab() {
    symtab.count = 0;       /* No variables yet */
    symtab.nextOffset = 0;  /* Start at stack offset 0 */
}

/* Add a new variable to the symbol table */
int addVar(char* name) {
    /* Check for duplicate declaration */
    if (isVarDeclared(name)) {
        return -1;  /* Error: variable already exists */
    }
    
    /* Add new symbol entry */
    symtab.vars[symtab.count].name = strdup(name);
    symtab.vars[symtab.count].offset = symtab.nextOffset;
    
    /* Advance offset by 4 bytes (size of int in MIPS) */
    symtab.nextOffset += 4;
    symtab.count++;
    
    /* Return the offset for this variable */
    return symtab.vars[symtab.count - 1].offset;
}

/* Look up a variable's stack offset */
int getVarOffset(char* name) {
    /* Linear search through symbol table */
    for (int i = 0; i < symtab.count; i++) {
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
    if (isVarDeclared(name)) {
        printf("SYMTAB ERROR: Variable %s already declared\n", name);
        return -1;  /* Error: variable already exists */
    }
    
    /* Add new symbol entry */
    symtab.vars[symtab.count].name = strdup(name);
    symtab.vars[symtab.count].offset = symtab.nextOffset;
    symtab.vars[symtab.count].isArray = 1; // Mark as array
    symtab.vars[symtab.count].arraySize = size; // Store array size
    
    /* Advance offset by size * 4 bytes (size of int in MIPS) */
    symtab.nextOffset += size * 4;
    symtab.count++;
    
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
    /* Check for duplicate declaration */
    if (isVarDeclared(name)) {
        printf("SYMTAB ERROR: Variable %s already declared\n", name);
        return -1;  /* Error: variable already exists */
    }
    
    /* Add new symbol entry */
    symtab.vars[symtab.count].name = strdup(name);
    symtab.vars[symtab.count].offset = symtab.nextOffset;
    symtab.vars[symtab.count].isArray = 1; // Mark as array
    symtab.vars[symtab.count].array2DSizeX = sizeX;
    symtab.vars[symtab.count].array2DSizeY = sizeY;
    
    /* Advance offset by size^2 * 4 bytes (size of int in MIPS) */
    symtab.nextOffset += (sizeX * sizeY) * 4;
    symtab.count++;
    
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

const Symbol* lookupSymbol(const char* name) {
    for (int i = 0; i < symtab.count; i++) {
        if (strcmp(symtab.vars[i].name, name) == 0) {
            return &symtab.vars[i];
        }
    }
    return NULL; // Not found
}
