#include "OrderBook.hpp"
#include <iostream>

OrderBook::OrderBook(){}

void OrderBook::addOrder(const Order& incoming_order){
    currentOrders_[incoming_order.id()] = incoming_order;
    switch (incoming_order.side())
    {
    case Side::Buy:
        matchBuy(incoming_order);
        break;
    
    case Side::Sell:
        matchSell(incoming_order);
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

void OrderBook::matchBuy(const Order& incoming_order){
    Order order = incoming_order;
    auto currentUpper = sells_.upper_bound(order.price());
    while(order.quantity() && currentUpper != sells_.begin()){
        auto &[price,qu] = *prev(currentUpper);
        while(qu.size()){
            if(qu.front().quantity() <= order.quantity()){
                order.decreaseQuantity(qu.front().quantity());
                qu.pop_front();
            }
            else{
                qu.front().decreaseQuantity(order.quantity());
                order.decreaseQuantity(order.quantity());
                break;
            }
        }
        currentUpper = prev(currentUpper);
    }
    if(order.quantity()){
        buys_[order.price()].push_back(order);
    }
}
void OrderBook::matchSell(const Order& incoming_order){
    Order order = incoming_order;
    auto current = buys_.lower_bound(order.price());
    while(order.quantity() && current != buys_.end()){
        auto &[price,qu] = *current;
        while(qu.size()){
            if(qu.front().quantity() <= order.quantity()){
                order.decreaseQuantity(qu.front().quantity());
                qu.pop_front();
            }
            else{
                qu.front().decreaseQuantity(order.quantity());
                order.decreaseQuantity(order.quantity());
                break;
            }
        }
        current = next(current);
    }
    if(order.quantity()){
        sells_[order.price()].push_back(order);
    }
}

void OrderBook::debug(){
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