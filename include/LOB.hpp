#ifndef LOB_HPP
#define LOB_HPP

#include "Common.hpp"
#include "Limit.hpp"
#include "Order.hpp"

#include <boost/container/flat_map.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <map>
#include <sparsehash/dense_hash_map>
#include <string>

class LOB {
    /*
    LOB should have two sides: bid, and ask, with various limits in each side.
    There should also be printBook method, which accepts an Int as depth, to verify against Lobster data.

    */
  private:
    boost::container::flat_map<Price, Limit *, std::less<Price>> m_bid; // O(1) access to top of book.
    google::dense_hash_map<Price, Limit *> m_bid_table;                 // O(1) random access to limits keyed off of price.

    boost::container::flat_map<Price, Limit *, std::greater<Price>> m_ask;
    google::dense_hash_map<Price, Limit *> m_ask_table;

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