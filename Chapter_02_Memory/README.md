# Chapter 2: Memory Management & Cache Friendliness

## The Memory Hierarchy
Modern CPUs are incredibly fast, but accessing main memory (RAM) is slow. To bridge this gap, CPUs use caches (L1, L2, L3).
- **L1 Cache**: ~1-4 cycles latency.
- **L2 Cache**: ~10-20 cycles.
- **L3 Cache**: ~50-100 cycles.
- **Main Memory**: ~200-300+ cycles.

## Cache Friendliness
To write fast code, we must respect the cache.
1.  **Spatial Locality**: Access data that is close together in memory.
2.  **Temporal Locality**: Reuse data that was recently accessed.

### Data Structures
- **`std::vector`**: Contiguous memory. Excellent spatial locality.
- **`std::list`**: Nodes scattered in memory. Poor spatial locality (cache misses).

## Stack vs. Heap
- **Stack**: Fast allocation/deallocation (just moving a pointer). Hot in cache.
- **Heap**: Slower allocation (`malloc`/`new`). Can cause fragmentation.
- *HFT Rule: Avoid heap allocations on the hot path.*

## Interactive Exercise
We will compare the performance of iterating over a `std::vector` versus a `std::list` to demonstrate the impact of cache locality.
