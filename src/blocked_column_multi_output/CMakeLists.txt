set(TARGET_NAME blocked_column_multi_output)

sps_get_processor_count(MAX_CONCURRENCY)

if (EMSCRIPTEN)
  sps_emscripten_module(
    TARGET_NAME ${TARGET_NAME}
    SOURCE_FILES benchmark.cpp
    THREAD_POOL_SIZE ${MAX_CONCURRENCY}
    ES6_MODULE OFF
    EXIT_RUNTIME ON
    OPTIMIZATION BEST
    LIBRARIES benchmark::benchmark;Threads::Threads
    FILE_SYSTEM ON
    PRE_JS ${CMAKE_CURRENT_SOURCE_DIR}/pre.js
    THREADING_ENABLED ON)

sps_target_compile_flags(${TARGET_NAME}
  THREADING_ENABLED ON
  OPTIMIZATION BEST)
  
else()
  add_executable(${TARGET_NAME} benchmark.cpp)
  # Set compiler flags for optimization and standards
  target_compile_options(${TARGET_NAME} PRIVATE -O3 -march=native -mtune=native -flto -fuse-linker-plugin)
  target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20) # Use C++20 standard.
  
  # Link against benchmark and pthread
  target_link_libraries(${TARGET_NAME} PRIVATE benchmark Threads::Threads)
endif()
set_target_properties(${TARGET_NAME} PROPERTIES
  RUNTIME_OUTPUT_NAME mmul_bench
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


if (NOT EMSCRIPTEN)
  set(TARGET_NAME blocked_column_multi_output_grind)
  add_executable(${TARGET_NAME} grind.cpp)
  target_compile_options(${TARGET_NAME} PRIVATE -O3 -march=native -mtune=native -flto -fuse-linker-plugin)# -mno-avx512f)
  target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20) # Use C++20 standard.
  
  set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_NAME grind
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
