# Chapter 11: Latency Budgeting and Microstructure-Aware Execution

High-frequency strategies live or die by how they spend latency. This chapter shows how to build a latency budget, trace it, and adapt execution to exchange microstructure without falling into over-optimization traps.

## 1. Building a Latency Budget
- **Scope the path:** NIC -> kernel bypass -> userland strategy -> risk checks -> order serializer -> exchange gateway.
- **Set targets:** e.g., <4µs tick-to-signal, <3µs signal-to-wire, <20µs round-trip on colocated venues.
- **Instrument everything:** use TSC snapshots around every stage; avoid coarse timers. Export histograms (p50/p99/p99.9) not averages.

```cpp
inline uint64_t rdtsc() noexcept {
  unsigned int aux;
  return __rdtscp(&aux); // serializing read for stable ordering
}
```

## 2. Kernel Bypass and Deterministic Userland
- Use DPDK/AF_XDP/Onload; pin Rx/Tx queues to dedicated isolated cores.
- Disable frequency scaling (governor=performance), C-states, SMT where noisy.
- Pre-fault and lock memory (mlockall); avoid page faults during the session.
- Replace dynamic allocations on hot paths with object pools or flat arrays.

## 3. Data Path Design
- **Cache-friendly books:** flat arrays for price levels; SoA for tight loops.
- **Branch minimization:** predictable branches or branchless min/max for top-of-book updates.
- **Serialization:** prebuild binary order templates; memcpy price/qty/side; avoid std::string.

```cpp
struct OrderTemplate {
  uint8_t buf[64]; // padded to cache line
  size_t len;
};

inline void encode_replace(OrderTemplate &tpl, int64_t px, int32_t qty) {
  static_assert(sizeof(tpl.buf) >= 64, "template too small");
  // overwrite only the mutable slots; rest is prewired session/session msg
  std::memcpy(tpl.buf + 16, &px, sizeof(px));
  std::memcpy(tpl.buf + 24, &qty, sizeof(qty));
}
```

## 4. Microstructure-Aware Execution
- Track queue position by modeling displayed depth and your resting size; avoid joining behind large iceberg candidates.
- Adapt order types to venue behavior: midpoint pegs vs. hidden vs. post-only; honor latency to cancel/replace.
- Measure adverse selection: markouts at 10/50/250µs; cut aggressiveness when markouts worsen.
- Avoid overreacting to fleeting quotes; require stability windows (e.g., 50–100µs) before sweeping.

## 5. Risk and Safeguards Without Latency Spikes
- Inline pre-trade limits: notional caps, fat-finger checks, self-match prevention.
- Keep risk state in lock-free, single-writer structures; avoid contended mutexes.
- Build a kill-switch with atomic flags; poll in the main loop, not via signals.

## 6. Time Sync and Drift Control
- Run PTP on dedicated NICs; monitor offset and path delay; fail fast on outliers.
- Cross-check TSC drift against PTP; recalibrate tick-to-time mapping regularly.
- Stamp every market event and order with both TSC and exchange timestamps where available.

## 7. Testing and Replay
- Record pcap + PTP timestamps; replay through the full stack with synthetic jitter.
- Assert latency SLOs in CI using perf counters; fail builds if p99.9 regresses.
- Fault-inject: drop/duplicate/out-of-order packets; ensure the strategy degrades safely.

## 8. Common Pitfalls
- Chasing single-digit nanoseconds while ignoring cancel/replace round-trips.
- Overfitting to one venue’s microstructure quirks; keep portability.
- Ignoring thermal throttling; watch CPU temps and NIC queues under burst load.

**Takeaway:** Treat latency as a budget you actively manage. Measure, attribute, and iterate. Microstructure-aware execution pays only when backed by instrumentation and disciplined safeguards.
