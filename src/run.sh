#!/bin/bash

./baseline/mmul_bench --benchmark_min_time=5.0s
./baseline_nonpower/mmul_bench --benchmark_min_time=5.0s
./blocked/mmul_bench --benchmark_min_time=5.0s
./blocked_column/mmul_bench --benchmark_min_time=5.0s
./blocked_column_multi_output/mmul_bench --benchmark_min_time=5.0s
