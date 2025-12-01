#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <chrono>
#include <random>

// --- Simplified Order Book (from Chapter 8) ---
enum class Side { Buy, Sell };

struct Order {
    int id;
    int price;
    int quantity;
    Side side;
};

class OrderBook {
    std::map<int, std::list<Order>, std::greater<int>> bids;
    std::map<int, std::list<Order>, std::less<int>> asks;

public:
    void add_order(int id, Side side, int price, int quantity) {
        if (side == Side::Buy) {
            match_buy(id, price, quantity);
        } else {
            match_sell(id, price, quantity);
        }
    }

    void match_buy(int id, int price, int quantity) {
        auto it = asks.begin();
        while (it != asks.end() && it->first <= price && quantity > 0) {
            auto& orders = it->second;
            while (!orders.empty() && quantity > 0) {
                Order& sell_order = orders.front();
                int trade_qty = std::min(quantity, sell_order.quantity);
                quantity -= trade_qty;
                sell_order.quantity -= trade_qty;
                if (sell_order.quantity == 0) orders.pop_front();
            }
            if (orders.empty()) it = asks.erase(it);
            else ++it;
        }
        if (quantity > 0) bids[price].push_back({id, price, quantity, Side::Buy});
    }

    void match_sell(int id, int price, int quantity) {
        auto it = bids.begin();
        while (it != bids.end() && it->first >= price && quantity > 0) {
            auto& orders = it->second;
            while (!orders.empty() && quantity > 0) {
                Order& buy_order = orders.front();
                int trade_qty = std::min(quantity, buy_order.quantity);
                quantity -= trade_qty;
                buy_order.quantity -= trade_qty;
                if (buy_order.quantity == 0) orders.pop_front();
            }
            if (orders.empty()) it = bids.erase(it);
            else ++it;
        }
        if (quantity > 0) asks[price].push_back({id, price, quantity, Side::Sell});
    }
};
// ----------------------------------------------

int main() {
    OrderBook book;
    const int NUM_ORDERS = 100000;
    
    std::cout << "Benchmarking " << NUM_ORDERS << " orders...\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; ++i) {
        // Alternate Buy/Sell to create matching activity
        Side side = (i % 2 == 0) ? Side::Buy : Side::Sell;
        int price = 100 + (i % 5); // Prices 100-104
        book.add_order(i, side, price, 10);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();

    std::cout << "Time: " << duration << " seconds\n";
    std::cout << "Throughput: " << (NUM_ORDERS / duration) << " orders/sec\n";

    return 0;
}
