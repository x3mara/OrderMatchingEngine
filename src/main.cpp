#include <iostream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Types.hpp"

int main(){
    Order o1(1,Side::Sell,100,10);
    Order o2(2,Side::Buy,150,3);
    std::cout<<o1<<' '<<o2<<'\n';
}