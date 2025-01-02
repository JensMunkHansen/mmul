#!/bin/bash

# Output file
output_file="benchmark_wasm_output.txt"

# Redirect stdout and stderr to the output file
exec > "$output_file" 2>&1

node build_wasm/baseline/mmul_bench.cjs --benchmark_min_time=5.0s
node build_wasm/baseline_nonpower/mmul_bench.cjs --benchmark_min_time=5.0s
node build_wasm/blocked/mmul_bench.cjs --benchmark_min_time=3.0s
node build_wasm/blocked_column/mmul_bench.cjs --benchmark_min_time=3.0s
node build_wasm/blocked_column_multi_output/mmul_bench.cjs --benchmark_min_time=3.0s

# Linear memory is divided into pages of 64 kB each 

# Working thread should fit into 64 kB pages

# consider
# wasmtime run --mapdir /::/path/to/host/filesystem my_wasm.wasm
