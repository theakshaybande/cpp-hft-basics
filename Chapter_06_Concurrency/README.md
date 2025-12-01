# Chapter 6: Concurrency & Multithreading

In HFT, we often use dedicated threads for specific tasks (e.g., one thread for market data, one for strategy, one for order entry) to avoid context switching.

## Key Concepts

### 1. Thread Affinity (Pinning)
- Binding a thread to a specific CPU core.
- *Benefit*: Prevents the OS from moving the thread to another core, preserving cache warmth.

### 2. False Sharing
- When two threads modify variables that reside on the same cache line.
- *Impact*: Causes "ping-ponging" of the cache line between cores, destroying performance.
- *Fix*: Align data to cache line boundaries (usually 64 bytes).

### 3. Spinlocks vs. Mutexes
- **Mutex**: Puts the thread to sleep if the lock is taken (system call). Good for long waits.
- **Spinlock**: Keeps the thread busy-waiting. Good for very short waits (low latency), but burns CPU.

## Interactive Exercise
We will write a program that demonstrates how to set thread affinity (Windows specific) and measures the cost of context switching (conceptually).
