#include "Order.hpp"
#include <cassert>

Order::Order(int id, Side side, double price, int quantity)
        :id_(id), side_(side), price_(price), quantity_(quantity),
            initialQuantity_(quantity){
            assert(quantity > 0);
        };
        
int Order::id() const{
    return id_;
}
Side Order::side() const{
    return side_;
}
double Order::price() const{
    return price_;
}
int Order::quantity() const{
    return quantity_;
}

void Order::decreaseQuantity(int amount){
    assert(amount <= quantity_);
    quantity_ -= amount;
}

std::ostream& operator<<(std::ostream& os, const Order& order){
    os << "{" << order.id_ << ", " <<
        (order.side_ == Side::Buy ? "Buy" : "Sell") << ", " <<
        order.price_ << " x " << order.quantity_ << "}"; 
    return os;
}