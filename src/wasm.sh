#!/bin/bash

(
    source "$HOME/github/emsdk/emsdk_env.sh"
    emcmake cmake -Bbuild -S. -Dbenchmark_DIR=/home/jmh/programming/cpp/performance/benchmark/wasminstall/lib/cmake/benchmark
    cmake --build build
)

