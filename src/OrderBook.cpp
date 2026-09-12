#include "OrderBook.hpp"
#include <iostream>

OrderBook::OrderBook(){}

void OrderBook::addOrder(const Order& incoming_order){
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

void OrderBook::matchBuy(const Order& incoming_order){
    Order order = incoming_order;
    auto currentUpper = sells_.upper_bound(order.quantity());
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
    auto current = buys_.lower_bound(order.quantity());
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
    std::cout.flush();
}