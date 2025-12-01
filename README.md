# C++ HFT Basics

This repo is my personal C++ lab for high-frequency trading and low-latency systems.

The goal is simple: build up the core engineering skills needed for HFT / market making:
fast code, predictable latency, memory discipline, concurrency, lock-free structures, and order book mechanics.

## Structure

Each chapter focuses on one building block:

- **Chapter_01_Intro** – Basic C++ setup and simple latency tests.
- **Chapter_02_Memory** – Stack vs heap, allocations, cache-friendly patterns.
- **Chapter_03_ModernCpp** – RAII, move semantics, smart pointers, templates.
- **Chapter_04_TMP** – Template metaprogramming and compile-time tricks.
- **Chapter_05_Network** – UDP basics, message handling (for feed-style traffic).
- **Chapter_06_Concurrency** – Threads, mutexes, contention.
- **Chapter_07_LockFree** – Lock-free queues and patterns relevant to HFT engines.
- **Chapter_08_OrderBook** – Order book data structures and basic matching logic.
- **Chapter_09_Benchmarking** – Microbenchmarks and timing harnesses.
- **Chapter_10_Simulation** – Simple strategy / market simulation glueing it all together.

Each folder contains:

- `README.md` – short explanation of the concept.
- `*.cpp` – focused example / experiment.

## How to build

I currently compile from the command line. Example with `g++`:

```bash
g++ -O3 -std=c++20 Chapter_01_Intro/latency_test.cpp -o build/latency_test
./build/latency_test
