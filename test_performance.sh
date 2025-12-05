#!/bin/bash
# Performance Testing Script for Optimized Compiler
# Tests compiler performance with various input sizes

echo "=== Compiler Performance Benchmark Suite ==="
echo "Platform: $(uname -s)"
echo "Date: $(date)"
echo ""

# Create test directory
mkdir -p performance_tests
cd performance_tests

# Generate small test file
echo "=== Generating Test Files ==="
cat > test_small.txt << 'EOF'
int x;
int y;
x = 10;
y = 20;
print(x + y);
EOF
echo "  - test_small.txt created"

# Generate medium test file
cat > test_medium.txt << 'EOF'
int a;
int b;
int c;
int d;
int e;
a = 1;
b = 2;
c = 3;
d = 4;
e = 5;
int result;
result = a + b + c + d + e;
print(result);
EOF

# Add more statements
for i in {1..50}; do
    echo "int var$i;" >> test_medium.txt
    echo "var$i = $i;" >> test_medium.txt
done
echo "print(var50);" >> test_medium.txt
echo "  - test_medium.txt created"

# Generate large test file
echo "  - Generating test_large.txt (this may take a moment)..."
echo "" > test_large.txt
for i in {1..500}; do
    echo "int var$i;" >> test_large.txt
done
for i in {1..500}; do
    echo "var$i = $i;" >> test_large.txt
done
echo "print(var1 + var500);" >> test_large.txt
echo "  - test_large.txt created (500 variables)"

# Generate stress test file
echo "  - Generating test_stress.txt..."
echo "" > test_stress.txt
for i in {1..1000}; do
    echo "int v$i;" >> test_stress.txt
done
for i in {1..1000}; do
    echo "v$i = $i + $((i+1));" >> test_stress.txt
done
echo "print(v1000);" >> test_stress.txt
echo "  - test_stress.txt created (1000 variables with expressions)"

cd ..

echo ""
echo "=== Running Performance Tests ==="
echo ""

# Function to run a test
run_test() {
    local testfile=$1
    local testname=$2

    echo "Testing: $testname"
    echo "  File: $testfile"

    # Check if compiler executable exists
    if [ ! -f "./compiler" ]; then
        echo "  ERROR: Compiler executable './compiler' not found"
        echo "  Please compile your compiler first"
        return
    fi

    # Run compiler and measure time
    echo "  Running..."
    /usr/bin/time -v ./compiler < "performance_tests/$testfile" > /dev/null 2> perf_output.txt

    # Extract timing information
    if [ -f perf_output.txt ]; then
        echo "  Results:"
        grep "User time" perf_output.txt | awk '{print "    CPU Time: " $4 " seconds"}'
        grep "Elapsed" perf_output.txt | awk '{print "    Wall Time: " $8}'
        grep "Maximum resident" perf_output.txt | awk '{print "    Peak Memory: " $6 " KB"}'
        rm perf_output.txt
    fi
    echo ""
}

# Run tests
run_test "test_small.txt" "Small Test (10 variables)"
run_test "test_medium.txt" "Medium Test (55 variables)"
run_test "test_large.txt" "Large Test (500 variables)"
run_test "test_stress.txt" "Stress Test (1000 variables)"

echo "=== Benchmark Complete ==="
echo ""
echo "To compare with unoptimized version:"
echo "  1. Build unoptimized version: make clean && make OPTIMIZE=0"
echo "  2. Re-run this script"
echo "  3. Compare results"
echo ""
echo "Expected Improvements:"
echo "  - Symbol table lookups: 10x+ faster for large programs"
echo "  - Memory usage: 30-50% reduction"
echo "  - Overall compilation: 50%+ speedup"
