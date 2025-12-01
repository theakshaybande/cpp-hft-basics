# Chapter 10: Simple HFT Strategy Simulation

Finally, we put everything together in a simple simulation.

## The Strategy: Market Making
A market maker provides liquidity by quoting both a Buy (Bid) and Sell (Ask) price.
- **Profit**: The Spread (Ask - Bid).
- **Risk**: Inventory risk (holding too much of one asset when price moves against you).

## Simulation Loop
1.  **Market Data**: Receive a price update (simulated).
2.  **Strategy Logic**: Calculate fair price and quote width.
3.  **Order Entry**: Send orders to the exchange (Order Book).
4.  **PnL Tracking**: Calculate Profit and Loss.

## Interactive Exercise
We will simulate a "Random Walk" market and run a simple Market Making strategy against it.
