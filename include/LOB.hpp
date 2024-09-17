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
    std::map<int, Limit, std::greater<int>> m_bid;
    std::map<int, Limit, std::less<int>> m_ask;

  public:
    int add(Order &newOrder);
    int cancel(int orderId, int volume, int price, ORDER_TYPE type); // Can be partial or total cancellation
    int execute(int orderId, int volume, int price, ORDER_TYPE type);
    int getVolumeAtLimit(int limit);
    int getBestPrice(ORDER_TYPE type);
    void printBook(int depth);
    void loadSnapshot(std::string fileDir);
    LOB();
};

#endif