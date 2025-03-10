# C Matching Engine

A high-performance order book matching engine implemented in C, designed for financial trading systems.

## Overview

This project implements a complete order book matching engine with efficient data structures optimized for high-frequency trading applications. The matching engine processes buy and sell orders, maintains order books, and executes trades based on price-time priority.

## Features

- **Fast Order Matching**: Optimized algorithms for quick order matching
- **Price-Time Priority**: Orders are matched according to standard price-time priority rules
- **Efficient Data Structures**: Custom implementations of order heaps and maps for O(log n) operations
- **Trade History**: Maintains a record of executed trades
- **Price History**: Tracks price movements over time
- **Memory Efficient**: Careful memory management for high-performance applications

## Project Structure

```
.
├── src/                # Source code
│   ├── core/           # Core data structures and algorithms
│   │   ├── order.h/c   # Order representation
│   │   ├── orderbook.h/c # Order book implementation
│   │   ├── orderheap.h/c # Heap-based priority queue
│   │   └── ordermap.h/c  # Fast order lookup
│   ├── matching/       # Matching engine logic
│   └── utils/          # Utility functions
├── include/            # Public headers
├── tests/              # Test suite
├── examples/           # Example applications
├── bin/                # Compiled binaries
└── obj/                # Object files
```

## Getting Started

### Prerequisites

- GCC compiler
- Make

### Building

```bash
# Build the project
make

# Run the application
make run

# Run tests
make run_tests
```

### Usage Example

```c
#include "core/orderbook.h"

int main() {
    // Create an orderbook
    OrderBook *orderbook = create_orderbook();

    // Add buy and sell orders
    Order *buy = create_order(1, 100, 10, 1000, 'B');
    Order *sell = create_order(2, 105, 5, 1001, 'S');

    add_order(orderbook, buy);
    add_order(orderbook, sell);

    // Print the orderbook state
    print_orderbook(orderbook);

    // Clean up
    free_orderbook(orderbook);

    return 0;
}
```

## Performance

The matching engine is designed for high-performance trading applications with:

- O(log n) order insertion and deletion
- O(1) order lookup by ID
- Efficient memory management
- Minimal heap allocations during critical operations

## Roadmap
- [x] Implement orderbook that handles submission logic for limit orders
- [ ] Implement basic matching logic for limit orders
- [ ] Implement market orders & corresponding matching logic
- [ ] Add support for order cancellation
- [ ] Implement order modification
- [ ] Add persistence layer for order storage
- [ ] Create REST API for order submission
- [ ] Implement WebSocket for real-time updates
- [ ] Add authentication and authorization
- [ ] Create admin dashboard
- [ ] Implement historical data analysis
- [ ] Add support for multiple trading pairs

