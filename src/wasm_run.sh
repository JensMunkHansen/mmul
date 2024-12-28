#!/bin/bash

#node build/baseline/mmul_bench.cjs --benchmark_min_time=3.0s
#node build/baseline_offset/mmul_bench.cjs --benchmark_min_time=3.0s
#node build/blocked/mmul_bench.cjs --benchmark_min_time=3.0s
node build/blocked_column/mmul_bench.cjs --benchmark_min_time=3.0s

# consider
# wasmtime run --mapdir /::/path/to/host/filesystem my_wasm.wasm
