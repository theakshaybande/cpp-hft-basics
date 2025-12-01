# Chapter 5: Network Programming (TCP/UDP/Multicast)

In HFT, market data is typically disseminated via **UDP Multicast**, while order entry is done via **TCP**.

## Key Concepts

### 1. TCP vs. UDP
- **TCP (Transmission Control Protocol)**: Reliable, ordered, connection-oriented. Slower due to handshakes and ACKs. Used for Order Entry.
- **UDP (User Datagram Protocol)**: Unreliable, connectionless, fast. No ACKs. Used for Market Data.

### 2. Multicast
- A method to send a single packet to multiple receivers. Efficient for market data feeds.

### 3. Kernel Bypass (Theory)
- Standard networking goes through the OS kernel (slow).
- **Kernel Bypass** (e.g., Solarflare OpenOnload, DPDK) allows the application to read directly from the NIC, skipping the kernel.

## Interactive Exercise
We will write a simple UDP receiver that listens for "market data" packets.
*Note: Since Windows networking requires Winsock setup, the code includes necessary boilerplate.*
