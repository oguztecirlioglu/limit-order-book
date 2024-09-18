#ifndef ORDER_H
#define ORDER_H

#include <stdint.h>

#include "Common.hpp"

class Order {
  private:
    const OrderId m_id;
    const ORDER_TYPE m_type;
    const Price m_price;
    const int64_t m_nanosecondsSinceMidnight;
    int m_shares;

  public:
    Order(OrderId id, ORDER_TYPE type, Volume shares, Price price, int64_t time);
    OrderId getId();
    ORDER_TYPE getType();
    Volume getShares();
    void setShares(Volume newShares);
    Price getPrice();
    int64_t getTime();
};

#endif