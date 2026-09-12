#include "MatchingEngine.hpp"

std::vector<Trade> MatchingEngine::submitOrder(const Order& incoming_order){
    switch (incoming_order.side())
    {
    case Side::Buy:
        return matchBuy(incoming_order);
    
    case Side::Sell:
        return matchSell(incoming_order);
    }
    return std::vector<Trade>();
}

std::vector<Trade> MatchingEngine::matchBuy(const Order& incoming_order){
    Order order = incoming_order;
    std::deque<Order>* dq = book_.bestSell(order.price());
    std::vector<Trade> trades;
    while(order.quantity() && dq){
        while(!dq->empty()){
            if(dq->front().quantity() <= order.quantity()){
                trades.push_back(
                    Trade(dq->front().id(),order.id(),
                        dq->front().price(), dq->front().quantity())
                );
                order.decreaseQuantity(dq->front().quantity());
                dq->pop_front();
            }
            else{
                trades.push_back(
                    Trade(dq->front().id(),order.id(),
                        dq->front().price(), order.quantity())
                );
                dq->front().decreaseQuantity(order.quantity());
                order.decreaseQuantity(order.quantity());
                break;
            }
        }
        dq = book_.bestSell(order.price());
    }
    if(order.quantity()){
        book_.addOrder(order);
    }
    return trades;
}
std::vector<Trade> MatchingEngine::matchSell(const Order& incoming_order){
    Order order = incoming_order;
    std::deque<Order>* dq = book_.bestBuy(order.price());
    std::vector<Trade> trades;
    while(order.quantity() && dq){
        while(!dq->empty()){
            if(dq->front().quantity() <= order.quantity()){
                trades.push_back(
                    Trade(order.id(),dq->front().id(),
                        dq->front().price(), dq->front().quantity())
                );
                order.decreaseQuantity(dq->front().quantity());
                dq->pop_front();
            }
            else{
                trades.push_back(
                    Trade(order.id(),dq->front().id(),
                        dq->front().price(), order.quantity())
                );
                dq->front().decreaseQuantity(order.quantity());
                order.decreaseQuantity(order.quantity());
                break;
            }
        }
        dq = book_.bestBuy(order.price());
    }
    if(order.quantity()){
        book_.addOrder(order);
    }
    return trades;
}

void MatchingEngine::debug() const {
    book_.debug();
}