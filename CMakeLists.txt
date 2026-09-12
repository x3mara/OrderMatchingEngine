cmake_minimum_required(VERSION 3.14)
project(OrderMatchingEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matching_engine
    src/main.cpp
    src/Order.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
)

target_include_directories(matching_engine PRIVATE src)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(matching_engine PRIVATE -Wall -Wextra -O2)
endif()

find_package(Threads REQUIRED)
target_link_libraries(matching_engine PRIVATE Threads::Threads)