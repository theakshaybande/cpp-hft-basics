#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdint>

#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

// Function to read the Time Stamp Counter
inline uint64_t rdtsc() {
#ifdef _WIN32
    return __rdtsc();
#else
    return __rdtsc();
#endif
}

// A simple function to measure
void do_work() {
    volatile int x = 0;
    for (int i = 0; i < 100; ++i) {
        x += i;
    }
}

int main() {
    const int ITERATIONS = 10000;
    std::vector<uint64_t> latencies;
    latencies.reserve(ITERATIONS);

    // Warmup phase
    for (int i = 0; i < 1000; ++i) {
        do_work();
    }

    // Measurement phase
    for (int i = 0; i < ITERATIONS; ++i) {
        uint64_t start = rdtsc();
        do_work();
        uint64_t end = rdtsc();
        latencies.push_back(end - start);
    }

    // Analysis
    uint64_t sum = std::accumulate(latencies.begin(), latencies.end(), 0ULL);
    double average = static_cast<double>(sum) / ITERATIONS;

    std::sort(latencies.begin(), latencies.end());
    uint64_t min_lat = latencies.front();
    uint64_t max_lat = latencies.back();
    uint64_t median = latencies[ITERATIONS / 2];
    uint64_t p99 = latencies[static_cast<int>(ITERATIONS * 0.99)];

    std::cout << "--- Latency Statistics (Cycles) ---\n";
    std::cout << "Average: " << average << "\n";
    std::cout << "Min:     " << min_lat << "\n";
    std::cout << "Max:     " << max_lat << "\n";
    std::cout << "Median:  " << median << "\n";
    std::cout << "99th %:  " << p99 << "\n";

    return 0;
}
