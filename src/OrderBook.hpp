#pragma once
#include "Types.hpp"
#include "Order.hpp"
#include <deque>
#include <map>

class OrderBook{
public:
    OrderBook();

    void addOrder(const Order& incoming_order);
    void cancelOrder(int orderId);

    void debug();
private:
    std::map<double,std::deque<Order>> sells_, buys_;

    void matchBuy(const Order& incoming_order);
    void matchSell(const Order& incoming_order);
};