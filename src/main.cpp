#include <chrono>
#include <iostream>

#include "Common.hpp"
#include "LOB.hpp"
#include "MarketDataParser.hpp"
#include "Order.hpp"
#include "Utility.hpp"

int main() {
    std::cout << "hello world!" << std::endl;
    auto lob = LOB();

    std::vector<Order> missingOrders = Utility::findMissingOrders("../sampleData/AAPL_2012-06-21_34200000_57600000_message_5.csv");

    Utility::createStartOfDaySnapshot(missingOrders, "../sampleData/AAPL_2012-06-21_34200000_57600000_START_OF_DAY_SNAPSHOT.csv");
    lob.loadSnapshot("../sampleData/AAPL_2012-06-21_34200000_57600000_START_OF_DAY_SNAPSHOT.csv");
    return 0;
}