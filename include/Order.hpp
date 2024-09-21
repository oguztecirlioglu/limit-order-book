#ifndef ORDER_H
#define ORDER_H

#include <chrono>
#include <stdint.h>

#include "Common.hpp"

class Order {
  private:
    const OrderId m_id;
    const ORDER_TYPE m_type;
    const Price m_price;
    const std::chrono::system_clock::time_point m_nanosecondsSinceMidnight;
    Volume m_shares;

  public:
    Order(OrderId id, ORDER_TYPE type, Volume shares, Price price, std::chrono::system_clock::time_point time);
    OrderId getId();
    ORDER_TYPE getType();
    Volume getShares();
    void setShares(Volume newShares);
    Price getPrice();
    std::chrono::system_clock::time_point getTime();
};

#endif