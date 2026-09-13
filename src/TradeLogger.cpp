#include "TradeLogger.hpp"
#include "TimeUtils.hpp"

TradeLogger::TradeLogger():file_(getLogPath()){
    file_ << "SellOrderId,BuyOrderId,Price,Quantity,Timestamp\n";
}

void TradeLogger::log(const Trade &trade){
    file_ << trade.sellId << "," << trade.buyId << ","
        << trade.price << "," << trade.quantity << ","
        << formatTimestamp(trade.timestamp) <<'\n';
}

std::filesystem::path TradeLogger::getLogPath() {
    std::filesystem::path sourceDir = std::filesystem::path(__FILE__).parent_path();
    return sourceDir / ".." / "logs" / "trades.csv";
}