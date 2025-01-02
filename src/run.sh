#!/bin/bash

# Output file
output_file="benchmark_output.txt"

# Redirect stdout and stderr to the output file
exec > "$output_file" 2>&1

./build/baseline/mmul_bench --benchmark_min_time=5.0s
./build/baseline_nonpower/mmul_bench --benchmark_min_time=5.0s
./build/blocked/mmul_bench --benchmark_min_time=3.0s
./build/blocked_column/mmul_bench --benchmark_min_time=3.0s
./build/blocked_column_multi_output/mmul_bench --benchmark_min_time=3.0s
./build/mkl/mmul_bench --benchmark_min_time=3.0s
