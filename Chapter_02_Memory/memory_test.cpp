#include <iostream>
#include <vector>
#include <list>
#include <numeric>
#include <chrono>
#include <random>
#include <algorithm>

// Simple timer class
class Timer {
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

int main() {
    const int NUM_ELEMENTS = 1000000; // 1 Million elements

    std::cout << "Creating containers with " << NUM_ELEMENTS << " integers...\n";

    // Vector
    std::vector<int> vec(NUM_ELEMENTS);
    std::iota(vec.begin(), vec.end(), 0); // Fill with 0, 1, 2...

    // List
    std::list<int> lst(NUM_ELEMENTS);
    std::iota(lst.begin(), lst.end(), 0);

    // Shuffle list to simulate fragmented memory (worst case for list)
    // Note: std::list doesn't have random access, so we can't shuffle easily.
    // But even without shuffling, the linked nature implies pointer chasing.
    
    // Benchmark Vector
    long long sum_vec = 0;
    {
        Timer t;
        for (int val : vec) {
            sum_vec += val;
        }
        std::cout << "Vector traversal: " << t.elapsed_ms() << " ms\n";
    }

    // Benchmark List
    long long sum_lst = 0;
    {
        Timer t;
        for (int val : lst) {
            sum_lst += val;
        }
        std::cout << "List traversal:   " << t.elapsed_ms() << " ms\n";
    }

    // Prevent optimization
    if (sum_vec != sum_lst) std::cout << "Error!\n";

    return 0;
}
