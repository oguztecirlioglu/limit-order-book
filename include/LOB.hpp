#ifndef LOB_HPP
#define LOB_HPP

#include "Common.hpp"
#include "Limit.hpp"
#include "Order.hpp"

#include <boost/unordered/unordered_flat_map.hpp>
#include <map>
#include <string>

class LOB {
    /*
    LOB should have two sides: bid, and ask, with various limits in each side.
    There should also be printBook method, which accepts an Int as depth, to verify against Lobster data.

    */
  private:
    // std::map<Price, Limit *, std::greater<Price>> m_bid; // O(1) access to top of book.
    std::map<Price, Limit *, std::less<Price>> m_bid;                 // O(1) access to top of book.
    boost::unordered::unordered_flat_map<Price, Limit *> m_bid_table; // O(1) random access to limits keyed off of price.

    // std::map<Price, Limit *, std::less<Price>> m_ask;
    std::map<Price, Limit *, std::greater<Price>> m_ask;
    boost::unordered::unordered_flat_map<Price, Limit *> m_ask_table;

  public:
    OrderId add(Order *newOrder);
    OrderId cancel(OrderId orderId, Volume volume, Price price, ORDER_TYPE type);      // Partial deletion
    OrderId totalDelete(OrderId orderId, Volume volume, Price price, ORDER_TYPE type); // Total deletion
    OrderId execute(OrderId orderId, Volume volume, Price price, ORDER_TYPE type);     // Execute from inside of book at best price, oldest order first.
    Volume getVolumeAtLimit(Price limit);
    Price getBestPrice(ORDER_TYPE type);
    void printBook(int depth);
    void loadSnapshot(std::string fileDir);
    LOB();
    ~LOB();
};

#endif