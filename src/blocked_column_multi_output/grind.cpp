// Main benchmark function of MMul

#include <algorithm>
#include <cstdlib>
#include <random>
#include <thread>
#include <vector>

// Blocked column multi-output serial implementation
void blocked_column_multi_output_mmul(const float *A, const float *B, float *C,
                                      std::size_t N) {
  // For each chunk of columns
  for (std::size_t col_chunk = 0; col_chunk < N; col_chunk += 16)
    // For each chunk of rows
    for (std::size_t row_chunk = 0; row_chunk < N; row_chunk += 16)
      // For each block of elements in this row of this column chunk
      // Solve for 16 elements at a time
      for (std::size_t tile = 0; tile < N; tile += 16)
        // Apply that tile to each row of the row chunk
        for (std::size_t row = 0; row < 16; row++)
          // For each row in the tile
          for (std::size_t tile_row = 0; tile_row < 16; tile_row++)
            // Solve for each element in this tile row
            for (std::size_t idx = 0; idx < 16; idx++)
              C[(row + row_chunk) * N + col_chunk + idx] +=
                  A[(row + row_chunk) * N + tile + tile_row] *
                  B[tile * N + tile_row * N + col_chunk + idx];
}

// Blocked column multi-output MMul with aligned memory benchmark
static void blocked_column_multi_output_aligned_mmul_bench(std::size_t N) {
  // Dimensions of our matrix

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<float> dist(-10, 10);

  // Create input matrices
  float *A = static_cast<float *>(aligned_alloc(64, N * N * sizeof(float)));
  float *B = static_cast<float *>(aligned_alloc(64, N * N * sizeof(float)));
  float *C = static_cast<float *>(aligned_alloc(64, N * N * sizeof(float)));

  // Initialize them with random values (and C to 0)
  std::generate(A, A + N * N, [&] { return dist(rng); });
  std::generate(B, B + N * N, [&] { return dist(rng); });
  std::generate(C, C + N * N, [&] { return 0.0f; });

  // Main benchmark loop
  blocked_column_multi_output_mmul(A, B, C, N);

  // Free memory
  free(A);
  free(B);
  free(C);
}

int main(int argc, char* argv[]) {
  blocked_column_multi_output_aligned_mmul_bench(1024);
  return 0;
}
