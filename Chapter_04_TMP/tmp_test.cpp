#include <iostream>
#include <chrono>

// 1. Dynamic Polymorphism (Virtual Functions) - The "Slow" Way
class LoggerBase {
public:
    virtual void log(const char* msg) = 0;
    virtual ~LoggerBase() {}
};

class ConsoleLogger : public LoggerBase {
public:
    void log(const char* msg) override {
        // In real life, this might write to a file or network
        // std::cout << "[Dynamic] " << msg << "\n";
        volatile int x = 0; (void)x; // Simulate work
    }
};

// 2. Static Polymorphism (CRTP) - The "Fast" Way
template <typename Derived>
class FastLoggerBase {
public:
    void log(const char* msg) {
        // Compile-time dispatch to the derived class implementation
        static_cast<Derived*>(this)->log_impl(msg);
    }
};

class FastConsoleLogger : public FastLoggerBase<FastConsoleLogger> {
public:
    void log_impl(const char* msg) {
        // std::cout << "[Static] " << msg << "\n";
        volatile int x = 0; (void)x; // Simulate work
    }
};

// Function taking a CRTP logger
template <typename Logger>
void do_logging(Logger& logger) {
    for (int i = 0; i < 1000000; ++i) {
        logger.log("test");
    }
}

// Function taking a virtual logger
void do_logging_dynamic(LoggerBase& logger) {
    for (int i = 0; i < 1000000; ++i) {
        logger.log("test");
    }
}

int main() {
    using namespace std::chrono;

    // Benchmark Dynamic
    ConsoleLogger dyn_logger;
    auto start = high_resolution_clock::now();
    do_logging_dynamic(dyn_logger);
    auto end = high_resolution_clock::now();
    std::cout << "Dynamic Polymorphism: " << duration<double, std::milli>(end - start).count() << " ms\n";

    // Benchmark Static
    FastConsoleLogger static_logger;
    start = high_resolution_clock::now();
    do_logging(static_logger);
    end = high_resolution_clock::now();
    std::cout << "Static Polymorphism:  " << duration<double, std::milli>(end - start).count() << " ms\n";

    return 0;
}
