#!/bin/bash
set -e

echo "=== STARTING DIFFERENTIAL TESTING ==="
echo "Compiling sources..."
g++ -O3 -std=c++17 src/main.cpp -o tests/solver
g++ -O3 -std=c++17 src/generator.cpp -o tests/generator

echo "Running 100 testcases with random tree structures..."
tree_types=("line" "star" "balanced" "random")

for i in {1..100}
do
    N=$(( (RANDOM % 199) + 2 ))
    type_idx=$(( i % 4 ))
    type=${tree_types[$type_idx]}
    
    ./tests/generator $N $type > data/temp_input.txt
    ./tests/solver --baseline < data/temp_input.txt > results/base_out.txt
    ./tests/solver < data/temp_input.txt > results/opt_out.txt
    
    if diff -w results/base_out.txt results/opt_out.txt > /dev/null; then
        echo "Test case #$i (N=$N, Type=$type): PASSED" >> results/stress_test.log
        echo "Test case #$i (N=$N, Type=$type): PASSED"
    else
        echo "Test case #$i (N=$N, Type=$type): FAILED!"
        exit 1
    fi
done

echo "=== ALL 100 TESTCASES PASSED PERFECTLY ==="