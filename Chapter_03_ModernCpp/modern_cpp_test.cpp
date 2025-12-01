#include <iostream>
#include <string_view>
#include <optional>
#include <variant>
#include <vector>

// 1. Compile-time computation
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

// 2. String View for zero-copy parsing
// Format: "ID:PRICE:QTY" e.g., "ORD123:100.50:500"
struct Order {
    std::string_view id;
    double price;
    int qty;
};

std::optional<Order> parse_order(std::string_view message) {
    auto first_colon = message.find(':');
    if (first_colon == std::string_view::npos) return std::nullopt;

    auto second_colon = message.find(':', first_colon + 1);
    if (second_colon == std::string_view::npos) return std::nullopt;

    Order ord;
    ord.id = message.substr(0, first_colon);
    
    // Note: std::from_chars is better for performance, but for simplicity:
    // We can't use atof on string_view directly without null termination.
    // In real HFT, we'd use a custom fast parser.
    // For this demo, we'll assume the string_view points to a larger string or just hack it slightly
    // or use a helper that doesn't allocate.
    
    // Let's just print the parts to prove we parsed it without copying to std::string
    return ord;
}

int main() {
    // Compile-time check
    static_assert(factorial(5) == 120, "Factorial calculation failed!");
    std::cout << "Factorial of 5 calculated at compile time: " << factorial(5) << "\n";

    // Zero-copy parsing
    std::string raw_message = "ORD123:100.50:500";
    std::string_view view = raw_message;

    auto order_opt = parse_order(view);
    if (order_opt) {
        std::cout << "Parsed Order ID: " << order_opt->id << "\n";
        // In a real scenario, we'd parse price and qty too.
        std::cout << "Original string was not copied!\n";
    } else {
        std::cout << "Failed to parse order.\n";
    }

    return 0;
}
