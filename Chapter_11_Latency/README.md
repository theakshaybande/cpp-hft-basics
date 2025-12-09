# Chapter 11: Latency Budget Sandbox

A tiny, microstructure-aware latency budget demo. It instruments a fake tick path (parse -> risk -> encode -> transmit) with `rdtscp`, prints p50/p90/p99, and shows how small code changes shift the budget.

## Build
- MSVC: `cl /std:c++20 latency_budget.cpp`
- clang: `clang++ -O3 -std=c++20 latency_budget.cpp -o latency_budget`
- g++: `g++ -O3 -std=c++20 latency_budget.cpp -o latency_budget`

## Run
- Default assumes a 3.2 GHz TSC: `./latency_budget`
- Override CPU frequency (cycles per ns): `./latency_budget 3.6`

## What to tweak
- Change `Stage` weights to see how heavier parsing or risk checks shift p99.
- Swap the `busy_work` body to reflect your real hot code.
- Add more stages (e.g., kill-switch poll, venue throttling) to extend the budget.
