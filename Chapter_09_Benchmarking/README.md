# Chapter 9: Benchmarking & Profiling

"Premature optimization is the root of all evil." - Donald Knuth.
However, in HFT, we must optimize. But we must do it based on data, not guesses.

## Key Concepts

### 1. Micro-benchmarking
- Measuring the performance of small, isolated pieces of code (e.g., a single function).
- Tools: Google Benchmark, Celero, or custom RDTSC timers.

### 2. Hot Paths
- The code paths that are executed most frequently and where latency matters most (e.g., the market data callback).
- Optimize these first.

### 3. Branch Prediction
- CPUs try to predict which way an `if` statement will go.
- If they guess wrong, the pipeline is flushed (expensive).
- `likely()` / `unlikely()` macros can help hint the compiler.

## Interactive Exercise
We will benchmark our Order Book's `add_order` function to see how many orders per second it can handle.
