#pragma once
#include "OrderBook.hpp"
#include "Trade.hpp"
#include <vector>

class MatchingEngine{
public:
    std::vector<Trade> submitOrder(const Order& incoming_order);

    void debug() const;
private:
    OrderBook book_;

    std::vector<Trade> matchBuy(const Order& incoming_order);
    std::vector<Trade> matchSell(const Order& incoming_order);
};