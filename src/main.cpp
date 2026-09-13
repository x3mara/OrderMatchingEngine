#include <iostream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"
#include "Types.hpp"
#include <thread>
#include <chrono>
#include <random>

Order generateRandomOrder(int id){
    static thread_local std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> sideRng(0, 1);
    static std::uniform_int_distribution<int> priceRng(9000, 10100);
    static std::uniform_int_distribution<int> quantityRng(1, 100);

    Side side = sideRng(rng) ? Side::Buy : Side::Sell;
    double price = (double)(priceRng(rng))/100;
    int quantity = quantityRng(rng);
    return Order(id,side,price,quantity);
}

int main(){

    std::cout<<"=============================\n";
    std::cout<<"       Program Started\n";
    std::cout<<"=============================\n";

    std::cout<<"Thread Hardware Concurrency: "<<
        std::thread::hardware_concurrency()<<'\n';

    constexpr int PRODUCER_CNT = 10;
    constexpr int ORDERS_PER_PRODUCER = 2500;

    auto start = std::chrono::steady_clock::now();
    {
        MatchingEngine engine;
        std::vector<std::thread> producers;
        producers.reserve(PRODUCER_CNT);
        for(int t = 0; t < PRODUCER_CNT; t++){
            producers.emplace_back([&engine, t](){
                for(int i = 0; i < ORDERS_PER_PRODUCER; i++){
                    engine.submitOrder(
                        generateRandomOrder(t * ORDERS_PER_PRODUCER + i)
                    );
                }
            });
        }
        for(auto &t: producers) t.join();
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "With " << PRODUCER_CNT << " threads each producing "
        << ORDERS_PER_PRODUCER << " orders simulatenously, the program took "
        << elapsed << " ms.\n";
}