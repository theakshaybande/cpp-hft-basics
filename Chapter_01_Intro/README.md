# Chapter 1: Introduction to HFT & C++ Performance

## What is High Frequency Trading (HFT)?
High Frequency Trading (HFT) involves using sophisticated algorithms to trade securities at extremely high speeds. In this domain, **latency** (the time it takes to react to an event) is the critical metric.

## Key Concepts

### 1. Latency vs. Throughput
- **Latency**: The time delay between a stimulus and a response. In HFT, we care about "tick-to-trade" latency—the time from receiving a market data packet to sending an order.
- **Throughput**: The number of units of information a system can process in a given amount of time.
- *HFT focuses on minimizing tail latency (99th and 99.9th percentile).*

### 2. Determinism
To achieve consistent low latency, code must be deterministic. This means avoiding:
- Dynamic memory allocation (malloc/new) on the hot path.
- System calls (I/O, context switches).
- Locking mechanisms that can cause blocking.

### 3. Zero-Cost Abstractions
C++ allows us to write high-level code that compiles down to efficient machine code with no runtime overhead. We should use these features (e.g., templates, inlining) to write clean but fast code.

## Interactive Exercise
In this chapter, we will measure the latency of a simple operation using the CPU's time stamp counter (RDTSC). This gives us a high-resolution measurement of how many CPU cycles a block of code takes.

### Code: `latency_test.cpp`
The accompanying code demonstrates how to use the `rdtsc` instruction to measure the cost of a function call and a simple addition.
