#include <catch2/catch_test_macros.hpp>
#include "MatchingEngine.hpp"

TEST_CASE("matching test 1"){
    MatchingEngine engine;
    engine.submitOrder(Order(1,Side::Sell,100.0,50));
    engine.submitOrder(Order(2,Side::Sell,120.0,50));
    SECTION("first buy request"){
        engine.submitOrder(Order(3,Side::Buy,110.0,10));
        while(engine.waitingCount());
        for(int i=0;i<10000;i++); // temporary fix for now
        std::optional<Order> order1 = engine.getBook().getOrder(1);
        REQUIRE(order1.has_value());
        REQUIRE(order1->quantity() == 40);
    }
}