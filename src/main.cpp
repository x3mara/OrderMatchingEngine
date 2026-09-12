#include <iostream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Types.hpp"

int main(){
    Order o1(1,Side::Sell,100,10);
    Order o2(2,Side::Buy,150,3);
    Order o3(10,Side::Sell,200,10);
    OrderBook ob;
    ob.addOrder(o1);
    ob.debug();
    ob.addOrder(o2);
    ob.debug();
    ob.addOrder(o2);
    ob.debug();
    ob.addOrder(o2);
    ob.debug();
    ob.addOrder(o2);
    ob.debug();
    ob.addOrder(o3);
    ob.debug();
}