#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

// Serialize TSC reads so stage boundaries are stable.
static inline std::uint64_t rdtsc_serialized() noexcept {
#if defined(_MSC_VER)
  unsigned int aux;
  return __rdtscp(&aux);
#else
  unsigned int aux;
  return __rdtscp(&aux);
#endif
}

struct LatencyStats {
  std::string_view label;
  std::vector<std::uint64_t> samples;

  void add(std::uint64_t cycles) { samples.push_back(cycles); }

  void summarize(double tsc_ghz) const {
    if (samples.empty()) {
      std::cout << label << ": no samples\n";
      return;
    }
    std::vector<std::uint64_t> sorted = samples;
    std::sort(sorted.begin(), sorted.end());

    auto pick = [&sorted](double q) {
      const std::size_t idx = static_cast<std::size_t>(q * (sorted.size() - 1));
      return sorted[idx];
    };

    const auto p50 = pick(0.50);
    const auto p90 = pick(0.90);
    const auto p99 = pick(0.99);

    auto cycles_to_ns = [tsc_ghz](std::uint64_t cycles) {
      return static_cast<double>(cycles) / tsc_ghz; // cycles per ns => ns
    };

    std::cout << label << ": p50=" << cycles_to_ns(p50) << " ns, "
              << "p90=" << cycles_to_ns(p90) << " ns, "
              << "p99=" << cycles_to_ns(p99) << " ns\n";
  }
};

class Probe {
 public:
  Probe(LatencyStats &stats) : stats_(stats), start_(rdtsc_serialized()) {}
  ~Probe() { stats_.add(rdtsc_serialized() - start_); }

 private:
  LatencyStats &stats_;
  std::uint64_t start_;
};

// Cheap deterministic work so the optimizer cannot erase the path.
std::uint64_t busy_work(int weight, std::uint64_t seed) {
  std::uint64_t x = seed | 1;
  for (int i = 0; i < weight; ++i) {
    x ^= (x << 7) + 0x9e3779b97f4a7c15ULL;
    x = (x >> 3) | (x << 61);
  }
  return x;
}

struct Stage {
  std::string_view name;
  int weight;
};

int main(int argc, char **argv) {
  const double tsc_ghz = (argc > 1) ? std::stod(argv[1]) : 3.2; // cycles per ns
  constexpr int kIters = 5000; // keep short; enough for stable percentiles

  std::vector<Stage> stages = {
      {"parse_md", 6},
      {"risk_checks", 9},
      {"encode_order", 5},
      {"tx_gateway", 4},
  };

  std::vector<LatencyStats> stats;
  stats.reserve(stages.size() + 1);
  for (auto &stage : stages) {
    stats.push_back({stage.name, {}});
  }
  stats.push_back({"total_path", {}});

  std::uint64_t seed = 1;
  for (int i = 0; i < kIters; ++i) {
    Probe total(stats.back());
    for (std::size_t s = 0; s < stages.size(); ++s) {
      Probe p(stats[s]);
      seed = busy_work(stages[s].weight + (i % 3), seed);
    }
  }

  std::cout << "Latency budget (cycles converted with TSC=" << tsc_ghz << " GHz):\n";
  for (auto &s : stats) {
    s.summarize(tsc_ghz);
  }

  return static_cast<int>(seed & 0xFF); // keep seed alive
}
