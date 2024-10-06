#include <chrono>
#include <iostream>

#include "Common.hpp"
#include "LOB.hpp"
#include "MarketDataParser.hpp"
#include "Order.hpp"
#include "Utility.hpp"

int main() {
    auto lob = LOB();

    std::vector<Order> missingOrders = Utility::findMissingOrders("../sampleData/AAPL_2012-06-21_34200000_57600000_message_5.csv");
    Utility::createStartOfDaySnapshot(missingOrders, "../sampleData/AAPL_2012-06-21_34200000_57600000_START_OF_DAY_SNAPSHOT.csv");
    lob.loadSnapshot("../sampleData/AAPL_2012-06-21_34200000_57600000_START_OF_DAY_SNAPSHOT.csv");

    MarketDataParser mdp = MarketDataParser("../sampleData/AAPL_2012-06-21_34200000_57600000_message_5.csv", &lob);

    std::vector<MarketEvent> allEvents = mdp.getAllEvents();

    auto start = std::chrono::high_resolution_clock::now();
    for (MarketEvent e : allEvents) {
        // std::cout << e.getOrderId() << " " << e.getEventType() << std::endl;
        mdp.processEvent(&e);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Processed: " << allEvents.size() << " events. " << std::endl;
    std::cout << "Computation time: " << duration.count() << " microseconds" << std::endl;
    std::cout << "Time per message " << duration.count() / (float)allEvents.size() << " microseconds " << std::endl;

    return 0;
}