#ifndef ORDER_H
#define ORDER_H

#include <stdint.h>

#include "Common.hpp"

class Order {
  private:
    const int m_id;
    const ORDER_TYPE m_type;
    const int m_price;
    const int64_t m_nanosecondsSinceMidnight;
    int m_shares;

  public:
    Order(int id, ORDER_TYPE type, int shares, int price, int64_t time);
    int getId();
    ORDER_TYPE getType();
    int getShares();
    void setShares(int newShares);
    int getPrice();
    int64_t getTime();
};

#endif