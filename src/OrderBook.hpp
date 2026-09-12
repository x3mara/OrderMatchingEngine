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

    std::deque<Order>* bestBuy(double requestPrice);
    std::deque<Order>* bestSell(double requestPrice);

    void debug() const;

private:
    std::map<double,std::deque<Order>> sells_, buys_;
    std::map<int,Order> currentOrders_;
};