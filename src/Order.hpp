#pragma once
#include "Types.hpp"
#include <iostream>

class Order{
public:
    Order(int id, Side side, double price, int quantity);

    int id() const;
    Side side() const;
    double price() const;
    int quantity() const;

    void decreaseQuantity(int amount);

    friend std::ostream& operator<<(std::ostream& os, const Order& order);

private:
    int id_, quantity_, initialQuantity_;
    Side side_;
    double price_;
};