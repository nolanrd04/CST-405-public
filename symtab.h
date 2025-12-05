#ifndef SYMTAB_H
#define SYMTAB_H

/* SYMBOL TABLE
 * Tracks all declared variables during compilation
 * Maps variable names to their memory locations (stack offsets)
 * Used for semantic checking and code generation
 * OPTIMIZED: Uses hash table for O(1) average lookup instead of O(n)
 */

#define MAX_VARS 1000      /* Maximum number of variables supported (increased) */
#define HASH_SIZE 211      /* Prime number for better hash distribution */

/* SYMBOL ENTRY - Information about each variable */
typedef struct SymbolNode {
    char* name;     /* Variable identifier */
    char* type;     /* Variable type ("int", "float", etc.) */
    int offset;     /* Stack offset in bytes (for MIPS stack frame) */
    int isArray;    /* 1 if variable is an array, 0 otherwise */
    int arraySize;  /* Size of the array if isArray is 1 */
    int array2DSizeX;
    int array2DSizeY;

    /* Scope Information*/
    int scope;      /* Scope level (0= global, 1 = function, 2+ = nested)*/

    /* Hash table chaining */
    struct SymbolNode* next;  /* For collision resolution */
} Symbol;

/* SYMBOL TABLE STRUCTURE */
typedef struct {
    Symbol* buckets[HASH_SIZE];  /* Hash table buckets */
    Symbol vars[MAX_VARS];  /* Still keep array for scope management */
    int count;              /* Number of variables declared */
    int nextOffset;         /* Next available stack offset */

    /* New Scope tracking*/
    int currentScope;  /* Current scope level */
    int scopeOffsets[MAX_VARS]; /* Stack offsets at each scope level */

    /* Performance counters */
    int lookups;       /* Total number of lookups */
    int collisions;    /* Number of hash collisions */
} SymbolTable;

/* SYMBOL TABLE OPERATIONS */
void initSymTab();               /* Initialize empty symbol table */
int addVar(char* name, char* type);          /* Add new variable, returns offset or -1 if duplicate */
int getVarOffset(char* name);    /* Get stack offset for variable, -1 if not found */
char* getVarType(char* name);
int isVarDeclared(char* name);   /* Check if variable exists (1=yes, 0=no) */

/* Hash function for symbol table */
unsigned int hash_symbol(const char* str);
int addArrayVar(char* name, int size, char* type); /* Add new array variable, returns offset or -1 if duplicate */
int isArrayVar(char* name);      /* Check if variable is an array (1=yes, 0=no) */
int getArraySize(char* name);    /* Get size of array */
int addArray2DVar(char* name, int sizeX, int sizeY, char* type);
int getArray2DSizeX(char* name);
int getArray2DSizeY(char* name);
int is2DArrayVar(char* name);

/* Scope management functions*/
void enterScope();               /* Enter a new scope level */
void exitScope();                /* Exit current scope level */
int getCurrentScope();         /* Get current scope level */
int isVarDeclaredInCurrentScope(char* name); /* Check if variable is declared in current scope */
int isGlobalVar(char* name);     /* Check if variable is global (scope 0) */

/* NEW: Print symbol table for debugging */
void printSymTab();

/* Function symbol table operations*/
typedef struct{
    char* name;                /* Function name */
    char* returnType;            /* Return type ("int", "void") */
    int paramCount;            /* Number of parameters */
    char* paramNames[10];      /* Parameter names (array of strings) */
    char* paramTypes[10];      /* Parameter types (array of strings) */
}FunctionSymbol;

typedef struct{
    FunctionSymbol funcs[MAX_VARS];
    int count;
}FunctionTable;

extern FunctionTable funcTable;
extern SymbolTable symtab;  /* Make symbol table accessible globally */


void initFuncTable();
int addFunction(char* name, char* returnType); /*Add function to table*/
int isFunctionDeclared(char* name); /*Check if function is declared*/
int isBoolType(char* type); /*Check if type is bool*/
void addFunctionParameter(char* funcName, char* paramName, char* paramType); /*Add parameter to function*/
int getFunctionParamCount(char* name); /*Get number of parameters*/
char* getFunctionReturnType(char* name); /*Get return type of function*/
const Symbol* lookupSymbol(const char* name);
void printSymTab();

#endif