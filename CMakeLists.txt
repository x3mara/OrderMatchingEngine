cmake_minimum_required(VERSION 3.14)
project(OrderMatchingEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matching_engine
    src/main.cpp
    src/Order.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/TradeLogger.cpp
)

target_include_directories(matching_engine PRIVATE src)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(matching_engine PRIVATE -Wall -Wextra -O2)
endif()

find_package(Threads REQUIRED)
target_link_libraries(matching_engine PRIVATE Threads::Threads)

include(FetchContent)
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0)
FetchContent_MakeAvailable(Catch2)

add_executable(tests
    tests/MatchingEngineTests.cpp
    tests/OrderBookTests.cpp
    src/Order.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/TradeLogger.cpp
)

target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE Threads::Threads)