cmake_minimum_required(VERSION 3.15)
project(benchmark_mkl LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization and architecture flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -mtune=native -flto -fuse-linker-plugin")

# Find MKL
find_package(MKL REQUIRED)

# Stupid Intel, we populate it ourselves
set(MKL_INCLUDE_DIR "${MKL_DIR}/../../../include")
set(MKL_LIBRARY_DIR "${MKL_DIR}/../..")
set(MKL_LIBRARIES mkl_rt)

# Find Google Benchmark
find_package(benchmark REQUIRED)

# Define the executable
add_executable(mmul_bench mkl_sgemm.cpp)

# Include directories for MKL
target_include_directories(mmul_bench PRIVATE ${MKL_DIR}/../../../include)

# Link MKL and other libraries
target_link_libraries(mmul_bench PRIVATE ${MKL_LIBRARIES} benchmark pthread)

target_link_directories(mmul_bench PRIVATE ${MKL_LIBRARY_DIR})

# Add runtime library paths for MKL
set_target_properties(mmul_bench PROPERTIES
    BUILD_RPATH "${MKL_LIBRARY_DIR}"
    INSTALL_RPATH "${MKL_LIBRARY_DIR}"
)
