# Chapter 3: Modern C++ for Low Latency (C++17/20)

Modern C++ standards (C++17, C++20) introduce features that help write safer, cleaner, and faster code.

## Key Features for HFT

### 1. `constexpr` and `consteval`
- **`constexpr`**: Suggests that a function or variable can be evaluated at compile time.
- **`consteval` (C++20)**: Forces evaluation at compile time.
- *Benefit*: Moves computation from runtime to compile time, reducing latency.

### 2. `std::string_view` (C++17)
- A lightweight, non-owning reference to a string.
- *Benefit*: Avoids expensive string copies and allocations when passing strings around.

### 3. `std::optional` (C++17)
- Represents a value that may or may not be present.
- *Benefit*: Safer than using magic values (e.g., -1 for error) without the overhead of exceptions.

### 4. `std::variant` (C++17)
- A type-safe union.
- *Benefit*: Can hold one of several types. Useful for message parsing where a message could be one of several types (Order, Cancel, Trade).

## Interactive Exercise
We will write a program that uses `constexpr` to calculate factorials at compile time and uses `std::string_view` to parse a "market message" without allocations.
