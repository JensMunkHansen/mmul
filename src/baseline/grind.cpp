#include <algorithm>
#include <cstdlib>
#include <random>
#include <thread>
#include <vector>
#include <iostream>

// Serial implementation
void serial_mmul(const float *A, const float *B, float *C, std::size_t N) {
  // For each row...
  for (std::size_t row = 0; row < N; row++)
    // For each col...
    for (std::size_t col = 0; col < N; col++)
      // For each element in the row/col pair...
      for (std::size_t idx = 0; idx < N; idx++)
        // Accumulate the partial results
        C[row * N + col] += A[row * N + idx] * B[idx * N + col];
}

static float serial_mmul_bench(std::size_t N) {
  // Number Dimensions of our matrix

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<float> dist(-10, 10);

  // Create input matrices
  float *A = new float[N * N];
  float *B = new float[N * N];
  float *C = new float[N * N];

  // Initialize them with random values (and C to 0)
  std::generate(A, A + N * N, [&] { return dist(rng); });
  std::generate(B, B + N * N, [&] { return dist(rng); });
  std::generate(C, C + N * N, [&] { return 0.0f; });

  // Main benchmark loop
  serial_mmul(A, B, C, N);

  float output = C[0];
  // Free memory
  delete[] A;
  delete[] B;
  delete[] C;
  return output;
}

int main(int argc, char* argv[]) {
  float output = serial_mmul_bench(1024);
  std::cout << output << std::endl;
  return 0;
}
