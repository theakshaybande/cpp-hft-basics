#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// Simple Market Simulator
class Market {
    double price = 100.0;
    std::mt19937 gen{std::random_device{}()};
    std::normal_distribution<> d{0, 0.1}; // Mean 0, StdDev 0.1

public:
    double get_market_price() {
        // Random Walk
        price += d(gen);
        return price;
    }
};

// Simple Strategy
class MarketMaker {
    double cash = 0;
    int inventory = 0;
    double pnl = 0;

public:
    void on_market_data(double mid_price) {
        // Quote around mid price
        double spread = 0.05;
        double bid = mid_price - spread / 2;
        double ask = mid_price + spread / 2;

        // Simplified execution assumption:
        // If market moves through our quotes, we trade.
        // For this sim, let's assume we capture the spread 50% of the time
        // just to see PnL ticking.
        
        // In a real backtest, we'd check if (NextPrice < Bid) -> Buy, (NextPrice > Ask) -> Sell.
        
        // Let's simulate a trade happening
        // Buy at Bid
        inventory++;
        cash -= bid;
        
        // Sell at Ask
        inventory--;
        cash += ask;
        
        // Mark to Market PnL
        double value = cash + (inventory * mid_price);
        std::cout << "Price: " << mid_price << " | PnL: " << value << "\n";
    }
};

int main() {
    Market market;
    MarketMaker strategy;

    std::cout << "Starting Simulation...\n";
    for (int i = 0; i < 100; ++i) {
        double price = market.get_market_price();
        strategy.on_market_data(price);
    }
    
    std::cout << "Simulation Complete.\n";
    return 0;
}
