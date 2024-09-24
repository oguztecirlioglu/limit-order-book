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

    // Utility::createStartOfDaySnapshot(missingOrders, "../sampleData/AAPL_2012-06-21_34200000_57600000_START_OF_DAY_SNAPSHOT.csv");
    lob.loadSnapshot("../sampleData/AAPL_2012-06-21_34200000_57600000_START_OF_DAY_SNAPSHOT.csv");

    MarketDataParser mdp = MarketDataParser("../sampleData/AAPL_2012-06-21_34200000_57600000_message_5.csv", lob);

    std::vector<MarketEvent> allEvents = mdp.getAllEvents();
    for (auto e : allEvents) {
        mdp.processEvent(&e);
    }
    std::cout << "Processed: " << allEvents.size() << " events. " << std::endl;
    return 0;
}