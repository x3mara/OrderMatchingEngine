#include "OrderBook.hpp"
#include <iostream>

OrderBook::OrderBook(){}

void OrderBook::addOrder(const Order& incoming_order){
    currentOrders_[incoming_order.id()] = incoming_order;
    switch (incoming_order.side())
    {
    case Side::Buy:
        buys_[incoming_order.price()].push_back(incoming_order);
        break;
    
    case Side::Sell:
        sells_[incoming_order.price()].push_back(incoming_order);
        break;
    }
}

std::optional<Order> OrderBook::cancelOrder(int orderId){
    if(currentOrders_.find(orderId) == currentOrders_.end()){
        return std::nullopt;
    }
    Order order = currentOrders_[orderId];
    std::deque<Order> &dq = (order.side() == Side::Buy ?
        buys_[order.price()] : sells_[order.price()]);
    
    auto it = std::find_if(dq.begin(), dq.end(),
        [&](const Order& o){return o.id() == orderId;});
    
    if(it == dq.end()) return std::nullopt;
    order = *it;
    dq.erase(it);
    currentOrders_.erase(orderId);

    if(order.quantity() == 0) return std::nullopt;
    return order;
}

std::deque<Order>* OrderBook::bestSell(double requestPrice){
    auto currentUpper = sells_.upper_bound(requestPrice);
    while(currentUpper != sells_.begin() &&
    (prev(currentUpper)->second).empty()){
        sells_.erase(prev(currentUpper));
    }
    if(currentUpper == sells_.begin()) return nullptr;
    return &prev(currentUpper)->second;
}
std::deque<Order>* OrderBook::bestBuy(double requestPrice){
    auto current = buys_.lower_bound(requestPrice);
    while(current != buys_.end() &&
    (current->second).empty()){
        current = next(current);
        buys_.erase(prev(current));
    }
    if(current == buys_.end()) return nullptr;
    return &current->second;
}

void OrderBook::debug() const{
    std::cout<<"=== Order Book ===\n";
    std::cout<<"Current Sells: \n";
    for(auto &[p,q]:sells_){
        std::cout<<p<<": ";
        for(auto i:q) std::cout<<i<<' ';
        std::cout<<'\n';
    }
    std::cout<<"Current Buys: \n";
    for(auto &[p,q]:buys_){
        std::cout<<p<<": ";
        for(auto i:q) std::cout<<i<<' ';
        std::cout<<'\n';
    }
    std::cout<<"==================\n";
    std::cout.flush();
}