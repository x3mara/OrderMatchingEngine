# OrderMatchingEngine

## Introduction

### What is it?
A multi-threaded C++ order matching engine, the core component of a financial exchange, responsible for matching incoming buy/sell orders against a live order book using price-time priority.

### Incentive
Built as a systems-programming project to explore concurrent data structures, matching algorithms, and low-level C++ design under realistic constraints similar to what trading infrastructure and exchange systems actually run in production.

## What does it do?

- Maintains a live order book of resting buy and sell orders, sorted by price-time priority.
- Matches incoming limit orders against the book: a buy matches a resting sell when the buy price is greater than or equal to the sell price, and vice versa.
- Supports partial fills, an order can match against multiple resting orders across one or more price levels.
- Supports order cancellation via an O(1) lookup index, independent of order book size.
- Processes orders concurrently: multiple producer threads can submit orders simultaneously. This was done via a mutex + queue with a worker thread processing the queue one by one.
- Logs every executed trade to `logs/trades.csv`.

## Build instructions

Requires CMake 3.14+ and a C++20-compatible compiler.

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

Run from the project root (so `logs/` resolves correctly):
```bash
./build/Release/matching_engine     # Windows/Visual Studio generator
./build/matching_engine             # Linux/macOS
```

## Benchmark results

Measured end-to-end (order submission + full matching + logging) on a 12-thread machine:

| Producer threads | Orders per thread | Total orders | Time | Throughput |
|---|---|---|---|---|
| 10 | 2,500 | 25,000 | 515 to 560 ms | 44,643 to 48,544 orders/sec |
| 1 | 2,500 | 25,000 | 520 to 550 ms | 45,500 to 48,100 orders/sec |

Interestingly, throughput was nearly identical whether using 1 or 10 concurrent producer threads (~45 to 48K orders/sec either way), indicating the single dedicated matching thread, not order submission, is the system's throughput bottleneck. This is expected given the design: the queue decouples submission from matching, but overall throughput is capped by how fast one thread can process the order book sequentially.

## What's not included (possible future work)

- **Automated test suite**: the engine has been exercised extensively via the load-generator benchmark above, but a Catch2 unit test suite covering matching edge cases (exact fills, partial fills, cancellation, price-time priority) is still in progress.
- **TCP network layer**: currently the engine is exercised in-process via a load generator in `main.cpp`. A socket-based interface (accepting orders from real external clients) is a natural extension, but out of scope for the current version.
- **Market orders**: only limit orders are currently supported.
- **Pro-rata matching**: only price-time (FIFO) priority is implemented; the matching rule could be made pluggable via a `MatchingStrategy` interface to support alternative allocation schemes.

## Tech stack

- C++20
- CMake
- `std::thread`, `std::mutex`, `std::condition_variable` for concurrency, no external threading libraries

## License

MIT, see `LICENSE`.
