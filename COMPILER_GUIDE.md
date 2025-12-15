# C-Minus Compiler - Complete Guide

## Overview

This is a full-featured **C-Minus compiler** that demonstrates all major phases of compilation. It takes C-Minus source code and generates optimized MIPS assembly language.

### Key Features

✅ **Complete Compilation Pipeline** - Lexical analysis, syntax analysis, AST generation, TAC generation, optimization, and MIPS code generation  
✅ **Advanced Optimizations** - Constant folding, copy propagation, algebraic simplification, strength reduction  
✅ **Function Support** - Function declarations, parameters, recursion, return statements  
✅ **Arrays & Loops** - Single/2D arrays, while loops, nested loops  
✅ **Control Flow** - If-else statements, switch/case statements  
✅ **Debugging** - Optional logging to file, detailed phase information  
✅ **Performance Metrics** - Compilation time, memory usage, hash table statistics  

---

## Building the Compiler

### Prerequisites
- `gcc` (or another C compiler)
- `flex` (lexical analyzer generator)
- `bison` (parser generator)
- `make`

### Quick Build

```bash
make clean
make
```

This generates the `minicompiler` executable.

---

## Running the Compiler

### Basic Usage

```bash
./minicompiler <input.cm> <output.s>
```

**Example:**
```bash
./minicompiler test.cm output.s
```

This compiles `test.cm` and generates MIPS assembly in `output.s`.

### With Debug Logging

```bash
./minicompiler <input.cm> <output.s> <logfile.txt>
```

**Example:**
```bash
./minicompiler comprehensive_test.cm output.s debug.log
```

The compiler prints detailed phase information to stdout AND saves it to `debug.log`.

---

## Compiler Architecture

### Phase 1: Lexical Analysis (Scanner)
**File:** `scanner.l`  
**Tool:** Flex

- Tokenizes input source code
- Recognizes keywords, identifiers, operators, literals
- Ignores comments and whitespace
- Outputs tokens to the parser

### Phase 2: Syntax Analysis (Parser)
**File:** `parser.y`  
**Tool:** Bison

- Parses token stream according to grammar rules
- Builds Abstract Syntax Tree (AST) if parsing succeeds
- Reports syntax errors with line numbers

### Phase 3: AST Construction & Display
**Files:** `ast.c`, `ast.h`

- AST represents program structure hierarchically
- Each node is a language construct (declaration, assignment, function, etc.)
- Printed with indentation showing tree structure

### Phase 4: Intermediate Code Generation (TAC)
**Files:** `tac.c`, `tac.h`

Three-Address Code (TAC) is an intermediate representation:
- Each instruction has at most 3 operands: `result = arg1 op arg2`
- Temporary variables (t0, t1, ...) hold intermediate results
- Easier to optimize than AST, simpler than MIPS

**Example:**
```
x = 2 + 3 * 4;
```

Generates TAC:
```
t0 = 3 * 4        // multiply first (precedence)
t1 = 2 + t0       // then add
x = t1            // assign to x
```

### Phase 5: Code Optimization
**Files:** `tac.c` (optimizeTAC function)

Optimizations applied during TAC processing:

#### 1. **Constant Folding**
- Evaluates expressions with only constants at compile-time
- `5 + 3` → `8` (stored directly, no runtime computation)

#### 2. **Algebraic Simplification**
- `x + 0` → `x` (removes unnecessary operations)
- `x * 1` → `x`
- `x * 0` → `0`
- `x - 0` → `x`

#### 3. **Strength Reduction**
- Replaces expensive operations with cheaper ones
- `x * 2` → `x + x` (addition is faster than multiplication)

#### 4. **Copy Propagation**
- Replaces variables with their known values
- If `x = 5`, then later uses of `x` can use `5` directly

### Phase 6: MIPS Code Generation
**File:** `codegen.c`

- Converts TAC to MIPS assembly language
- Manages stack for local variables
- Uses temporary registers (`$t0`-`$t7`)
- Generates system calls for `println()` and `print()` statements

**Generated MIPS features:**
- Stack frame setup/teardown
- Function prologue/epilogue
- Parameter passing on stack
- Return value in `$v0`

---

## C-Minus Language Reference

### Basic Data Types
```c
int x;          // 32-bit integer
int arr[10];    // Array of 10 integers
int mat[5][5];  // 2D array (5x5)
```

### Functions
```c
// Function declaration with parameters
int add(int a, int b) {
    return a + b;
}

// Function with no parameters
void printMessage() {
    println(42);
}

// Recursive function
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}
```

### Control Flow
```c
// If-else
if (x > 5) {
    println(1);
} else {
    println(0);
}

// While loop
int sum = 0;
int i = 1;
while (i <= 10) {
    sum = sum + i;
    i = i + 1;
}

// Switch statement
switch (choice) {
    case 1:
        println(100);
        break;
    case 2:
        println(200);
        break;
    default:
        println(0);
}
```

### Built-in Functions
```c
println(value);    // Print an integer value with newline
print(string);     // Print a string (string literals only)
```

### Operators
```c
// Arithmetic
a + b    // Addition
a - b    // Subtraction
a * b    // Multiplication
a / b    // Division (integer)

// Comparison
a == b   // Equal
a != b   // Not equal
a < b    // Less than
a > b    // Greater than
a <= b   // Less than or equal
a >= b   // Greater than or equal

// Logical
a && b   // AND
a || b   // OR
!a       // NOT
```

### WHEN Loops

The **WHEN loop** is an alternative looping construct that exits **when a condition becomes TRUE** (opposite of while loops which continue while a condition is TRUE).

#### Basic Syntax

```c
when (exit_condition) {
    // Loop body executes repeatedly
    // while exit_condition is FALSE
    // Exits when exit_condition becomes TRUE
}
```

#### How It Works

Unlike `while` loops that continue executing while a condition is **true**, `when` loops continue executing while the condition is **false** and exit when it becomes **true**.

**Comparison:**
- `while (x < 5)` - executes while x is less than 5, stops when x >= 5
- `when (x >= 5)` - executes while x is less than 5, stops when x >= 5 (same result, inverted logic)

#### With OR Branches

The `when` loop supports multiple exit conditions using `or` branches:

```c
when (x == 10) {
    println(x);
} or (x > 15) {
    println("x is too large");
} or (x < 0) {
    println("x is negative");
}
```

The loop exits when **any** of the conditions becomes true.

#### With ELSE Block

An optional `else` block executes every iteration:

```c
int counter = 0;
when (counter >= 5) {
    println("Done!");
} else {
    println("Still going...");
    counter = counter + 1;
}
```

#### Break When Statement

You can explicitly exit a when loop using `break when`:

```c
when (x >= 10) {
    x = x + 1;
    break when (x >= 5);  // Exit if x reaches 5
}
```

#### Example: Countdown Loop

```c
void main() {
    int x;
    
    x = 0;
    
    when (x >= 5) {
        println("Done!");
    } or (x < 5) {
        println("Not yet!");
        x = x + 1;
        break when (x >= 5);
    }
    
    println(x);  // Output: 5
}
```

---

## Example Programs

### Simple Arithmetic
**File:** `test_opt.cm`
```c
void main() {
    int x;
    int y;
    
    x = 2 + 3;      // Constant folding: becomes 5
    y = x * 1;      // Algebraic simplification: becomes x
    println(x);
    println(y);
}
```

### Array Processing
```c
void main(void) {
    int arr[5];
    int i;
    
    // Initialize array
    i = 0;
    while (i < 5) {
        arr[i] = i * 2;
        i = i + 1;
    }
    
    // Print array
    i = 0;
    while (i < 5) {
        println(arr[i]);
        i = i + 1;
    }
}
```

### Recursion
```c
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

void main(void) {
    println(fibonacci(10));
}
```

---

## Understanding the Output

### Compiler Output Format

The compiler displays each compilation phase with visual boxes:

```
╔════════════════════════════════════════════════════════════╗
│ PHASE 1: LEXICAL & SYNTAX ANALYSIS                        │
├────────────────────────────────────────────────────────────┤
│ • Reading source file: test.cm                            │
│ • Tokenizing input (scanner.l)                            │
│ • Parsing grammar rules (parser.y)                        │
│ • Building Abstract Syntax Tree                           │
└────────────────────────────────────────────────────────────┘
✓ Parse successful - program is syntactically correct!
```

### TAC Output Example

**Unoptimized:**
```
1: DECL x
2: t0 = 2 + 3      // Add: store result in t0
3: x = t0          // Assign value to x
4: PRINT x
```

**After Optimization:**
```
1: DECL x
2: x = 5           // Constant value: 5 (folded)
3: PRINT 5         // Direct constant print (propagated)
```

### Optimization Statistics

```
╔════════════════════════════════════════════════════════════╗
║          TAC OPTIMIZATION STATISTICS                       ║
╠════════════════════════════════════════════════════════════╣
║ Instructions Before:            120                       ║
║ Instructions After:             95                        ║
║ Instructions Eliminated:         25 (20.8%)               ║
╠════════════════════════════════════════════════════════════╣
║ OPTIMIZATION BREAKDOWN:                                    ║
║ • Constant Folds:                12                       ║
║ • Algebraic Simplifications:     8                        ║
║ • Strength Reductions:           3                        ║
║ • Copy Propagations:             2                        ║
║ • Dead Code Eliminations:        0                        ║
║ • Common Subexpr Eliminated:     0                        ║
╠════════════════════════════════════════════════════════════╣
║ Total Optimizations Applied:     25                       ║
╚════════════════════════════════════════════════════════════╝
```

### Symbol Table Display

```
╔════════════════════════════════════════════════════════════╗
║              SYMBOL TABLE (Scope 0)                        ║
╠════════════════════════════════════════════════════════════╣
║ Name          | Type    | Offset | Scope | Array           ║
╠════════════════════════════════════════════════════════════╣
║ globalCounter | int     | 0      | 0     | No            ║
║ globalArray   | int     | 4      | 0     | [20]         ║
╚════════════════════════════════════════════════════════════╝
```

---

## Testing the Compiler

### Test Files Included

- **`test_opt.cm`** - Simple optimization test with constants and algebraic simplifications
- **`comprehensive_test.cm`** - Full feature test with functions, arrays, recursion, etc.

### Running Tests

```bash
# Compile a test file
./minicompiler test_opt.cm test_output.s debug.log

# View the generated MIPS assembly
cat test_output.s

# Run the generated MIPS code in a MIPS simulator (if available)
# (SPIM, QtSpim, or Mars simulator)
```

### Expected Behavior

✅ Should generate valid MIPS assembly  
✅ Should show optimization statistics  
✅ Should display symbol table  
✅ Should complete without errors for valid C-Minus programs  

---

## File Structure

```
.
├── scanner.l               # Lexical analyzer (Flex)
├── parser.y                # Parser/grammar (Bison)
├── ast.c / ast.h           # Abstract Syntax Tree
├── tac.c / tac.h           # Three-Address Code & optimization
├── symtab.c / symtab.h     # Symbol table (scope tracking)
├── codegen.c / codegen.h   # MIPS code generation
├── main.c                  # Compiler driver
├── benchmark.c / .h        # Performance timing
├── stringpool.c / .h       # String interning
├── Makefile                # Build configuration
├── test_opt.cm             # Simple test file
├── comprehensive_test.cm   # Full feature test
└── COMPILER_GUIDE.md       # This file
```

---

## Troubleshooting

### Build Errors

**Error:** `make: command not found`
- Solution: Install build tools (gcc, flex, bison, make)

**Error:** `undefined reference to 'yyparse'`
- Solution: Run `make clean` then `make` to regenerate parser

### Compilation Errors

**Error:** `Parse failed - check your syntax!`
- Check for missing semicolons
- Verify function syntax: `type name(params) { body }`
- Ensure all blocks have matching braces `{}`

**Error:** `Segmentation fault during optimization`
- Usually caused by buffer overflow (now fixed in latest version)
- Try with a smaller test file

### Missing MIPS Simulator

To run generated MIPS code, you need:
- **QtSpim** - Qt-based MIPS simulator (GUI)
- **SPIM** - Command-line MIPS simulator
- **Mars** - MIPS Assembler and Runtime Simulator

---

## Performance Features

### Compilation Metrics

The compiler tracks:
- **CPU Time** - Actual processor time used
- **Wall Time** - Real elapsed time
- **Memory Delta** - Memory allocated during compilation
- **Hash Collisions** - Symbol table efficiency
- **Lookup Count** - Total symbol table lookups

Example output:
```
=== Total Compilation Performance ===
CPU Time:     0.023 seconds
Wall Time:    0.028 seconds
Memory Delta: 1236 KB

=== Symbol Table Hash Table Statistics ===
Total lookups: 547
Hash collisions: 2
Collision rate: 0.37%
```

---

## Implementation Details

### Symbol Table
- Hash table with chaining for collision resolution
- Tracks variable scope, type, offset, array info
- Supports nested scopes (functions, blocks)

### TAC Optimization
- Local optimization pass (function-by-function)
- Maintains value propagation table for copy propagation
- Bounds-checked arrays prevent buffer overflows

### MIPS Code Generation
- Stack-based local variable storage
- Caller/callee register conventions
- System calls for I/O operations

---

## Future Enhancements

Potential improvements:
- Global optimization passes (across functions)
- Dead code elimination
- Common subexpression elimination (CSE)
- Register allocation optimization
- Floating-point support
- String literals
- Pointer types
- Bitwise operations

---

## Learning Resources

This compiler demonstrates:
- **Lexical Analysis** - Tokenization with Flex
- **Syntax Analysis** - Context-free grammar with Bison
- **AST Construction** - Tree-based program representation
- **Intermediate Code** - Three-address code (TAC)
- **Code Optimization** - Multiple optimization techniques
- **Code Generation** - Assembly language output
- **Symbol Tables** - Scope and variable tracking
- **Performance Analysis** - Benchmarking and statistics

Great for understanding compiler design concepts!

---

**Compiler Version:** Checkpoint 6 (with optimization statistics & function body support)  
**Language:** C-Minus (Educational subset of C)  
**Target:** MIPS Assembly  
**Status:** Fully functional ✅
