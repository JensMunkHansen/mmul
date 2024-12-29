#!/bin/bash

node build/baseline/mmul_bench.cjs --benchmark_min_time=3.0s
node build/baseline_nonpower/mmul_bench.cjs --benchmark_min_time=3.0s
node build/blocked/mmul_bench.cjs --benchmark_min_time=3.0s
node build/blocked_column/mmul_bench.cjs --benchmark_min_time=3.0s

# Linear memory is divided into pages of 64 kB each 

# Working thread should fit into 64 kB pages

# consider
# wasmtime run --mapdir /::/path/to/host/filesystem my_wasm.wasm
