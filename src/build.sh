#!/bin/bash

cmake -Bbuild -S. -DCMAKE_BUILD_TYPE=Release -DMKL_DIR=/opt/intel/oneapi/mkl/latest/lib/cmake/mkl
cmake --build build
