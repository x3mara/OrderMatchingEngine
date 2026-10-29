#include <catch2/catch_test_macros.hpp>
#include "OrderBook.hpp"

TEST_CASE("adding an order"){
    OrderBook book;
    SECTION("order shouldn't exist on initialization"){
        REQUIRE_FALSE(book.getOrder(1).has_value());
    }
    book.addOrder(Order(1,Side::Buy,100.0,50));
    SECTION("order should be added without any errors or changes"){
        REQUIRE(book.getOrder(1).has_value());
        CHECK(book.getOrder(1)->quantity() == 50);
    }
}

TEST_CASE("cancelling an order"){
    OrderBook book;
    book.addOrder(Order(1,Side::Buy,100.0,50));
    SECTION("cancelling an existing order"){
        auto cancelled_order = book.cancelOrder(1);
        REQUIRE(cancelled_order.has_value());
        CHECK(cancelled_order->quantity() == 50);
    }
    SECTION("cancelling a cancelled order"){
        book.cancelOrder(1);
        auto cancelled_order = book.cancelOrder(1);
        REQUIRE_FALSE(cancelled_order.has_value());
    }
    SECTION("cancelling a non-existing order"){
        auto cancelled_order = book.cancelOrder(2);
        REQUIRE_FALSE(cancelled_order.has_value());
    }
}

TEST_CASE("cancelling then adding an order with same id"){
    OrderBook book;
    book.addOrder(Order(1,Side::Buy,100.0,50));
    book.cancelOrder(1);
    SECTION("order should be successfulling cancelled"){
        REQUIRE_FALSE(book.getOrder(1).has_value());
    }
    book.addOrder(Order(1,Side::Sell,140.0,20));
    SECTION("order should have the new correct data"){
        auto order = book.getOrder(1);
        REQUIRE(order.has_value());
        CHECK(order->side() == Side::Sell);
        CHECK(order->price() == 140.0);
        CHECK(order->quantity() == 20);
    }
}