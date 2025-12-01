#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <string>

enum class Side { Buy, Sell };

struct Order {
    int id;
    int price;
    int quantity;
    Side side;
};

class OrderBook {
    // Price -> List of Orders (Time priority)
    // Use std::greater for Bids (highest price first)
    std::map<int, std::list<Order>, std::greater<int>> bids;
    // Use std::less for Asks (lowest price first)
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
                
                std::cout << "TRADE: Buy " << id << " matches Sell " << sell_order.id 
                          << " @ " << it->first << " Qty: " << trade_qty << "\n";

                quantity -= trade_qty;
                sell_order.quantity -= trade_qty;

                if (sell_order.quantity == 0) {
                    orders.pop_front();
                }
            }
            if (orders.empty()) {
                it = asks.erase(it);
            } else {
                ++it;
            }
        }

        // If quantity remains, add to book
        if (quantity > 0) {
            bids[price].push_back({id, price, quantity, Side::Buy});
            std::cout << "Added Buy Order " << id << " @ " << price << " Qty: " << quantity << "\n";
        }
    }

    void match_sell(int id, int price, int quantity) {
        auto it = bids.begin();
        while (it != bids.end() && it->first >= price && quantity > 0) {
            auto& orders = it->second;
            while (!orders.empty() && quantity > 0) {
                Order& buy_order = orders.front();
                int trade_qty = std::min(quantity, buy_order.quantity);
                
                std::cout << "TRADE: Sell " << id << " matches Buy " << buy_order.id 
                          << " @ " << it->first << " Qty: " << trade_qty << "\n";

                quantity -= trade_qty;
                buy_order.quantity -= trade_qty;

                if (buy_order.quantity == 0) {
                    orders.pop_front();
                }
            }
            if (orders.empty()) {
                it = bids.erase(it);
            } else {
                ++it;
            }
        }

        // If quantity remains, add to book
        if (quantity > 0) {
            asks[price].push_back({id, price, quantity, Side::Sell});
            std::cout << "Added Sell Order " << id << " @ " << price << " Qty: " << quantity << "\n";
        }
    }

    void print_book() {
        std::cout << "\n--- Order Book ---\n";
        std::cout << "ASKS:\n";
        for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
            int total_qty = 0;
            for (const auto& o : it->second) total_qty += o.quantity;
            std::cout << "  " << it->first << " : " << total_qty << "\n";
        }
        std::cout << "BIDS:\n";
        for (const auto& pair : bids) {
            int total_qty = 0;
            for (const auto& o : pair.second) total_qty += o.quantity;
            std::cout << "  " << pair.first << " : " << total_qty << "\n";
        }
        std::cout << "------------------\n\n";
    }
};

int main() {
    OrderBook book;

    // Add some resting orders
    book.add_order(1, Side::Sell, 101, 100);
    book.add_order(2, Side::Sell, 102, 50);
    book.add_order(3, Side::Buy, 99, 100);
    book.add_order(4, Side::Buy, 98, 200);

    book.print_book();

    // Incoming aggressive Buy order
    std::cout << "Incoming Buy Order (Aggressive)...\n";
    book.add_order(5, Side::Buy, 101, 120); // Should match 100 @ 101, rest 20 @ 101 (wait, no, rest on book if not FOK)
    
    // Note: In this simple logic, if it matches partially, the rest is added to book at limit price.
    // Since 101 matches the sell at 101.
    
    book.print_book();

    return 0;
}
