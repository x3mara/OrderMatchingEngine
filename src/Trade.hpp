#pragma once
#include <chrono>

struct Trade{
    Trade(int sellId, int buyId, double price, int quantity)
        :sellId(sellId),buyId(buyId),price(price),quantity(quantity){
            timestamp = std::chrono::system_clock::now();
        }
    int sellId, buyId;
    double price; int quantity;
    std::chrono::system_clock::time_point timestamp;
};