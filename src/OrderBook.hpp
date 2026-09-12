#pragma once
#include "Types.hpp"
#include "Order.hpp"
#include <deque>
#include <map>
#include <optional>

class OrderBook{
public:
    OrderBook();

    void addOrder(const Order& incoming_order);
    std::optional<Order> cancelOrder(int orderId);

    void debug();
private:
    std::map<double,std::deque<Order>> sells_, buys_;
    std::map<int,Order> currentOrders_;

    void matchBuy(const Order& incoming_order);
    void matchSell(const Order& incoming_order);
};