/* MINIMAL C COMPILER - EDUCATIONAL VERSION
 * Demonstrates all phases of compilation with a simple language
 * Supports: int variables, addition, assignment, print
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"
#include "codegen.h"
#include "tac.h"
#include "symtab.h"

/* Performance optimization headers */
#include "benchmark.h"
#include "stringpool.h"

extern int yyparse();
extern FILE* yyin;
extern ASTNode* root;

FILE* log_file = NULL;  /* Global log file pointer */

/* Print to both stdout and log file if enabled */
void log_printf(const char* format, ...) {
    va_list args;

    /* Print to stdout */
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);  /* Flush output immediately */

    /* Print to log file if enabled */
    if (log_file) {
        va_start(args, format);
        vfprintf(log_file, format, args);
        va_end(args);
        fflush(log_file);  /* Flush log file immediately */
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 4) {
        printf("Usage: %s <input.c> <output.s> [log_file]\n", argv[0]);
        printf("Example: ./minicompiler test.c output.s\n");
        printf("Example: ./minicompiler test.c output.s debug.log\n");
        return 1;
    }

    /* Open log file if provided */
    if (argc == 4) {
        log_file = fopen(argv[3], "w");
        if (!log_file) {
            fprintf(stderr, "Warning: Cannot open log file '%s', continuing without logging\n", argv[3]);
        } else {
            printf("Logging debug output to: %s\n", argv[3]);
        }
    }
    
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", argv[1]);
        return 1;
    }

    log_printf("\n");
    log_printf("╔════════════════════════════════════════════════════════════╗\n");
    log_printf("║          MINIMAL C COMPILER - EDUCATIONAL VERSION         ║\n");
    log_printf("╚════════════════════════════════════════════════════════════╝\n");
    log_printf("\n");

    /* Initialize performance optimization systems */
    init_string_pool();

    /* CRITICAL: Initialize symbol table BEFORE parsing */
    initSymTab();

    /* Start overall compilation benchmark */
    BenchmarkResult* total_bench = start_benchmark();

    /* PHASE 1: Lexical and Syntax Analysis */
    log_printf("┌──────────────────────────────────────────────────────────┐\n");
    log_printf("│ PHASE 1: LEXICAL & SYNTAX ANALYSIS                       │\n");
    log_printf("├──────────────────────────────────────────────────────────┤\n");
    log_printf("│ • Reading source file: %s\n", argv[1]);
    log_printf("│ • Tokenizing input (scanner.l)\n");
    log_printf("│ • Parsing grammar rules (parser.y)\n");
    log_printf("│ • Building Abstract Syntax Tree\n");
    log_printf("└──────────────────────────────────────────────────────────┘\n");

    if (yyparse() == 0) {
        log_printf("✓ Parse successful - program is syntactically correct!\n\n");

        /* PHASE 2: AST Display */
        log_printf("┌──────────────────────────────────────────────────────────┐\n");
        log_printf("│ PHASE 2: ABSTRACT SYNTAX TREE (AST)                      │\n");
        log_printf("├──────────────────────────────────────────────────────────┤\n");
        log_printf("│ Tree structure representing the program hierarchy:        │\n");
        log_printf("└──────────────────────────────────────────────────────────┘\n");
        printAST(root, 0);
        log_printf("\n");

        /* PHASE 3: Intermediate Code */
        log_printf("┌──────────────────────────────────────────────────────────┐\n");
        log_printf("│ PHASE 3: INTERMEDIATE CODE GENERATION                    │\n");
        log_printf("├──────────────────────────────────────────────────────────┤\n");
        log_printf("│ Three-Address Code (TAC) - simplified instructions:       │\n");
        log_printf("│ • Each instruction has at most 3 operands                │\n");
        log_printf("│ • Temporary variables (t0, t1, ...) for expressions      │\n");
        log_printf("└──────────────────────────────────────────────────────────┘\n");
        initTAC();
        generateTAC(root);
        printTAC();
        log_printf("\n");

        /* PHASE 4: Optimization */
        log_printf("┌──────────────────────────────────────────────────────────┐\n");
        log_printf("│ PHASE 4: CODE OPTIMIZATION                               │\n");
        log_printf("├──────────────────────────────────────────────────────────┤\n");
        log_printf("│ Applying optimizations:                                  │\n");
        log_printf("│ • Constant folding (evaluate compile-time expressions)   │\n");
        log_printf("│ • Copy propagation (replace variables with values)       │\n");
        log_printf("│ • Algebraic simplification (x+0, x*1, x*0, etc.)         │\n");
        log_printf("│ • Strength reduction (x*2 -> x+x)                        │\n");
        log_printf("└──────────────────────────────────────────────────────────┘\n");
        optimizeTAC();
        printOptimizedTAC();
        printOptimizationStats();
        log_printf("\n");

        /* PHASE 5: Code Generation */
        log_printf("┌──────────────────────────────────────────────────────────┐\n");
        log_printf("│ PHASE 5: MIPS CODE GENERATION                            │\n");
        log_printf("├──────────────────────────────────────────────────────────┤\n");
        log_printf("│ Translating to MIPS assembly:                            │\n");
        log_printf("│ • Variables stored on stack                              │\n");
        log_printf("│ • Using $t0-$t7 for temporary values                     │\n");
        log_printf("│ • System calls for print operations                      │\n");
        log_printf("└──────────────────────────────────────────────────────────┘\n");
        generateMIPS(root, argv[2]);
        /* generateMIPSFromOptimizedTAC("output.s"); */
        log_printf("✓ MIPS assembly code generated to: %s\n", argv[2]);
        log_printf("\n");

        log_printf("╔════════════════════════════════════════════════════════════╗\n");
        log_printf("║                  COMPILATION SUCCESSFUL!                   ║\n");
        log_printf("║         Run the output file in a MIPS simulator           ║\n");
        log_printf("╚════════════════════════════════════════════════════════════╝\n");
        log_printf("\n");
        printSymTab();

        /* Print performance statistics */
        log_printf("\n");
        log_printf("╔════════════════════════════════════════════════════════════╗\n");
        log_printf("║              PERFORMANCE OPTIMIZATION STATS                ║\n");
        log_printf("╚════════════════════════════════════════════════════════════╝\n");

        /* Overall compilation time */
        end_benchmark(total_bench, "Total Compilation");
        fflush(stdout);
        fflush(stderr);

        /* Symbol table statistics */
        log_printf("\n=== Symbol Table Hash Table Statistics ===\n");
        log_printf("Total lookups: %d\n", symtab.lookups);
        log_printf("Hash collisions: %d\n", symtab.collisions);
        log_printf("Collision rate: %.2f%%\n",
                   symtab.lookups > 0 ? (100.0 * symtab.collisions / symtab.lookups) : 0.0);
        log_printf("Variables stored: %d\n", symtab.count);
        log_printf("Hash table size: %d buckets\n", HASH_SIZE);
        fflush(stdout);
        fflush(stderr);

        /* String pool statistics */
        print_string_stats();
        fflush(stdout);
        fflush(stderr);

        log_printf("\n");
        free_benchmark(total_bench);
    } else {
        log_printf("✗ Parse failed - check your syntax!\n");
        log_printf("Common errors:\n");
        log_printf("  • Missing semicolon after statements\n");
        log_printf("  • Undeclared variables\n");
        log_printf("  • Invalid syntax for print statements\n");
        return 1;
    }

    fclose(yyin);

    /* Close log file if opened */
    if (log_file) {
        fclose(log_file);
        printf("Debug output saved to: %s\n", argv[3]);
    }

    return 0;
}