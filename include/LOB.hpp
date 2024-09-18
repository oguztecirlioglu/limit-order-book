#ifndef LOB_H
#define LOB_H

#include "Common.hpp"
#include "Limit.hpp"
#include "Order.hpp"

#include <map>
#include <string>

class LOB {
    /*
    LOB should have two sides: bid, and ask, with various limits in each side.
    There should also be printBook method, which accepts an Int as depth, to verify against Lobster data.

    */
  private:
    std::map<Price, Limit *, std::greater<Price>> m_bid; // O(1) access to top of book.
    std::unordered_map<Price, Limit *> m_bid_table;      // O(1) random access to limits keyed off of price.

    std::map<Price, Limit *, std::less<Price>> m_ask;
    std::unordered_map<Price, Limit *> m_ask_table;

  public:
    OrderId add(Order *newOrder);
    OrderId cancel(int orderId, int volume, int price, ORDER_TYPE type);      // Partial deletion
    OrderId totalDelete(int orderId, int volume, int price, ORDER_TYPE type); // Total deletion
    OrderId execute(int orderId, int volume, int price, ORDER_TYPE type);     // Execute from inside of book at best price, oldest order first.
    Volume getVolumeAtLimit(int limit);
    Price getBestPrice(ORDER_TYPE type);
    void printBook(int depth);
    void loadSnapshot(std::string fileDir);
    LOB();
};

#endif