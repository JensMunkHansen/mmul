#include "benchmark/benchmark.h"

#include <algorithm>
#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>

unsigned int numThreads = std::thread::hardware_concurrency();

// Simple ThreadPool implementation
class ThreadPool {
public:
    ThreadPool(size_t num_threads) : active_tasks(0) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                        ++active_tasks;
                    }
                    task();
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        --active_tasks;
                        if (tasks.empty() && active_tasks == 0) {
                            all_tasks_done.notify_all();
                        }
                    }
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }

    template <class F>
    void enqueue(F &&task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }

    void wait_for_all_tasks() {
        std::unique_lock<std::mutex> lock(queue_mutex);
        all_tasks_done.wait(lock, [this] {
            return tasks.empty() && active_tasks == 0;
        });
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::condition_variable all_tasks_done;
    size_t active_tasks;
    bool stop = false;
};

// Serial implementation
void serial_mmul(const float *A, const float *B, float *C, std::size_t N) {
    for (std::size_t row = 0; row < N; row++)
        for (std::size_t col = 0; col < N; col++)
            for (std::size_t idx = 0; idx < N; idx++)
                C[row * N + col] += A[row * N + idx] * B[idx * N + col];
}

// Parallel implementation
void parallel_mmul(const float *A, const float *B, float *C, std::size_t N,
                   std::size_t start_row, std::size_t end_row) {
    for (std::size_t row = start_row; row < end_row; row++)
        for (std::size_t col = 0; col < N; col++)
            for (std::size_t idx = 0; idx < N; idx++)
                C[row * N + col] += A[row * N + idx] * B[idx * N + col];
}

ThreadPool* global_pool = nullptr;

// Serial MMul benchmark
static void serial_mmul_bench(benchmark::State &s) {
    std::size_t N = s.range(0);
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_real_distribution<float> dist(-10, 10);

    float *A = new float[N * N];
    float *B = new float[N * N];
    float *C = new float[N * N];

    std::generate(A, A + N * N, [&] { return dist(rng); });
    std::generate(B, B + N * N, [&] { return dist(rng); });
    std::generate(C, C + N * N, [&] { return 0.0f; });

    for (auto _ : s) {
        serial_mmul(A, B, C, N);
    }

    delete[] A;
    delete[] B;
    delete[] C;
}
BENCHMARK(serial_mmul_bench)
    ->Arg(2 * numThreads * 16)
    ->Arg(4 * numThreads * 16)
    ->Arg(6 * numThreads * 16)
    ->Unit(benchmark::kMillisecond);

// Parallel MMul benchmark
static void parallel_mmul_bench(benchmark::State &s) {
    std::size_t N = s.range(0);
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_real_distribution<float> dist(-10, 10);

    float *A = new float[N * N];
    float *B = new float[N * N];
    float *C = new float[N * N];

    std::generate(A, A + N * N, [&] { return dist(rng); });
    std::generate(B, B + N * N, [&] { return dist(rng); });
    std::generate(C, C + N * N, [&] { return 0.0f; });

    if (!global_pool) {
        global_pool = new ThreadPool(numThreads);
    }

    std::size_t n_rows = N / numThreads;

    for (auto _ : s) {
        for (std::size_t i = 0; i < numThreads; ++i) {
            auto start_row = i * n_rows;
            auto end_row = (i == numThreads - 1) ? N : (start_row + n_rows);
            global_pool->enqueue([=] { parallel_mmul(A, B, C, N, start_row, end_row); });
        }
        global_pool->wait_for_all_tasks();
    }

    delete[] A;
    delete[] B;
    delete[] C;
}
BENCHMARK(parallel_mmul_bench)
    ->Arg(2 * numThreads * 16)
    ->Arg(4 * numThreads * 16)
    ->Arg(6 * numThreads * 16)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

int main(int argc, char **argv) {
    std::vector<char *> benchmark_args;
    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]).find("--") == 0 || i == 0) {
            benchmark_args.push_back(argv[i]);
        } else {
            numThreads = std::min(static_cast<unsigned int>(std::stoi(argv[i])),
                                  std::thread::hardware_concurrency());
        }
    }

    int benchmark_argc = static_cast<int>(benchmark_args.size());
    char **benchmark_argv = benchmark_args.data();

    benchmark::Initialize(&benchmark_argc, benchmark_argv);
    if (benchmark::ReportUnrecognizedArguments(benchmark_argc, benchmark_argv))
        return 1;
    benchmark::RunSpecifiedBenchmarks();

    if (global_pool) {
        delete global_pool;
    }

    return 0;
}
