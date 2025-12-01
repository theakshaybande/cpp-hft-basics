#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

// Function to pin thread to a core
void pin_thread(int core_id) {
#ifdef _WIN32
    HANDLE thread_handle = GetCurrentThread();
    DWORD_PTR mask = (1ULL << core_id);
    SetThreadAffinityMask(thread_handle, mask);
#else
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
#endif
}

void worker(int id) {
    pin_thread(id % std::thread::hardware_concurrency());
    
    // Simulate work
    volatile int x = 0;
    for (int i = 0; i < 10000000; ++i) {
        x += i;
    }
    
    std::cout << "Thread " << id << " finished on core " << id << "\n";
}

// False Sharing Demo
struct FalseSharing {
    volatile int a;
    // char padding[60]; // Uncomment to fix false sharing (assuming 64 byte cache line)
    volatile int b;
};

struct NoFalseSharing {
    alignas(64) volatile int a;
    alignas(64) volatile int b;
};

int main() {
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << "\n";

    // 1. Thread Affinity Demo
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    t1.join();
    t2.join();

    // 2. False Sharing Benchmark
    // (Simplified: In a real test, we'd have threads hammering these variables)
    std::cout << "Size of FalseSharing struct: " << sizeof(FalseSharing) << "\n";
    std::cout << "Size of NoFalseSharing struct: " << sizeof(NoFalseSharing) << "\n";

    return 0;
}
