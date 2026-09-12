#include <iostream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"
#include "Types.hpp"

int main(){
    Order o1(1,Side::Sell,100,10);
    Order o2(2,Side::Buy,150,7);
    Order o3(10,Side::Sell,200,10);
    MatchingEngine e;
    e.submitOrder(o1);
    e.debug();
    e.submitOrder(o2);
    e.debug();
    e.submitOrder(o2);
    e.debug();
    e.submitOrder(o1);
    e.debug();
    e.submitOrder(o3);
    e.debug();
}