#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

template <typename T, size_t Size>
class LockFreeQueue {
    // Ring buffer storage
    std::vector<T> buffer;
    
    // Atomic indices
    // alignas(64) to prevent false sharing between head and tail
    alignas(64) std::atomic<size_t> head{0}; // Read index
    alignas(64) std::atomic<size_t> tail{0}; // Write index

public:
    LockFreeQueue() : buffer(Size) {}

    bool push(const T& item) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) % Size;

        if (next_tail == head.load(std::memory_order_acquire)) {
            return false; // Full
        }

        buffer[current_tail] = item;
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        size_t current_head = head.load(std::memory_order_relaxed);

        if (current_head == tail.load(std::memory_order_acquire)) {
            return false; // Empty
        }

        item = buffer[current_head];
        head.store((current_head + 1) % Size, std::memory_order_release);
        return true;
    }
};

void producer(LockFreeQueue<int, 1024>& q) {
    for (int i = 0; i < 1000000; ++i) {
        while (!q.push(i)) {
            // Busy wait (spin)
            std::this_thread::yield(); 
        }
    }
}

void consumer(LockFreeQueue<int, 1024>& q) {
    int val;
    int count = 0;
    while (count < 1000000) {
        if (q.pop(val)) {
            count++;
            if (count % 100000 == 0) std::cout << "Consumed: " << val << "\n";
        } else {
            // Busy wait
            std::this_thread::yield();
        }
    }
}

int main() {
    LockFreeQueue<int, 1024> queue;

    std::thread t1(producer, std::ref(queue));
    std::thread t2(consumer, std::ref(queue));

    t1.join();
    t2.join();

    std::cout << "Finished passing 1 million items lock-free!\n";
    return 0;
}
