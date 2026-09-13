#pragma once
#include "Trade.hpp"
#include <fstream>
#include <filesystem>

class TradeLogger{
public:
    TradeLogger();

    void log(const Trade &trade);
    
private:
    std::ofstream file_;
    
    std::filesystem::path getLogPath();
};