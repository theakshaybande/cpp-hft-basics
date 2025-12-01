# Chapter 8: Building an Order Book

The **Limit Order Book (LOB)** is the core data structure of an exchange or a trading strategy. It maintains all open buy (bid) and sell (ask) orders.

## Key Concepts

### 1. Price-Time Priority
- Orders are matched based on:
    1.  **Price**: Better prices (higher bid, lower ask) execute first.
    2.  **Time**: If prices are equal, the order that arrived first executes first.

### 2. Data Structures
- **Map**: `std::map<Price, List<Order>>`. Keeps prices sorted.
- **Vector**: `std::vector<PriceLevel>`. Faster iteration if price range is known/limited.

### 3. Matching Engine
- When a new order arrives (e.g., a Buy), it checks the opposite side (Sells) for a match.
- If `BuyPrice >= BestAskPrice`, a trade occurs.

## Interactive Exercise
We will implement a simplified Order Book that supports Adding orders and Executing trades.
