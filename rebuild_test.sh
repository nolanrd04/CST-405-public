#!/bin/bash
echo "=== Cleaning ==="
make clean
echo ""
echo "=== Building compiler ==="
make
echo ""
echo "=== Compiling comprehensive_test.cm ==="
./minicompiler comprehensive_test.cm output.s
echo ""
echo "=== Checking if new code is in output.s ==="
grep -c "move.*# Save index" output.s || echo "MOVE instruction NOT FOUND in output.s"
echo ""
echo "=== Timestamp of output.s ==="
ls -la output.s
