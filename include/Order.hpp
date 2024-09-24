#ifndef ORDER_HPP
#define ORDER_HPP

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
    const OrderId getId();
    const ORDER_TYPE getType();
    const Volume getShares();
    void setShares(Volume newShares);
    const Price getPrice();
    std::chrono::system_clock::time_point getTime();
};

std::ostream &operator<<(std::ostream &out, Order &order);

#endif