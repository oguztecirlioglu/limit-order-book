#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "MarketEvent.hpp"
#include "Order.hpp"

#include <string>
#include <vector>

class Utility {
  public:
    static void createStartOfDaySnapshot(std::vector<Order> &missingOrders, const std::string &path);
    static std::vector<MarketEvent> loadStartOfDaySnapshot(const std::string &path);
    static std::vector<Order> findMissingOrders(const std::string &messageFilePath);

    static Order *marketEventToOrder(MarketEvent &event);
    static MarketEvent orderToMarketEvent(Order &order);
};

#endif