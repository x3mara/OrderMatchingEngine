#pragma once
#include "OrderBook.hpp"
#include "Trade.hpp"
#include "ThreadSafeQueue.hpp"
#include <vector>
#include <thread>

class MatchingEngine{
public:
    MatchingEngine();
    ~MatchingEngine();

    void submitOrder(const Order& incoming_order);

    // Debug functions <3
    size_t waitingCount() const;
    void debug() const;

private:
    OrderBook book_;
    ThreadSafeQueue<Order> queue_;
    std::thread workerThread_;

    void workerProcess();

    std::vector<Trade> matchOrder(const Order& incoming_order);
    std::vector<Trade> matchBuy(const Order& incoming_order);
    std::vector<Trade> matchSell(const Order& incoming_order);
};