#include "MatchingEngine.hpp"
#include <filesystem>

MatchingEngine::MatchingEngine()
    :workerThread_(&MatchingEngine::workerProcess, this){}

MatchingEngine::~MatchingEngine(){
    queue_.stop();
    workerThread_.join();
}

void MatchingEngine::submitOrder(const Order& incoming_order){
    queue_.push(incoming_order);
}

void MatchingEngine::workerProcess(){
    try{
        while(true){
            Order order = queue_.pop();
            for(auto trade:matchOrder(order)){
                tradeLogger_.log(trade);
            }
        }
    }
    catch(const std::runtime_error& e){
        std::cout<<"WorkerThread Stopped: " << e.what() <<std::endl; 
    }
}

std::vector<Trade> MatchingEngine::matchOrder(const Order& incoming_order){
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

size_t MatchingEngine::waitingCount() const{
    return queue_.size();
}

void MatchingEngine::debug() const {
    book_.debug();
}