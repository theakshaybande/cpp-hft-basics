# Chapter 7: Lock-free Data Structures (Ring Buffers)

In HFT, we cannot afford to wait for locks (`std::mutex`). Instead, we use **lock-free** data structures, primarily the **Single Producer Single Consumer (SPSC) Ring Buffer**.

## Key Concepts

### 1. Atomics
- Variables that can be modified safely by multiple threads without locks.
- `std::atomic<T>`.

### 2. Memory Ordering
- Tells the compiler and CPU how memory operations can be reordered.
- **`std::memory_order_acquire`**: Ensures subsequent reads happen after this load.
- **`std::memory_order_release`**: Ensures prior writes happen before this store.
- *Default is `memory_order_seq_cst` (Sequential Consistency), which is safe but slower.*

### 3. Ring Buffer (Circular Buffer)
- A fixed-size array where we wrap around indices.
- Efficient because it avoids dynamic allocation.
- SPSC Ring Buffer requires no locks, just atomic head and tail indices.

## Interactive Exercise
We will implement a lock-free SPSC queue and use it to pass messages between a producer thread and a consumer thread.
