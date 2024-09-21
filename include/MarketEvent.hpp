#ifndef MARKET_EVENT_H
#define MARKET_EVENT

#include "Common.hpp"
#include <chrono>

class MarketEvent {
  private:
    const OrderId m_id;
    const ORDER_TYPE m_type;
    const Price m_price;
    const std::chrono::system_clock::time_point m_nanosecondsSinceMidnight;
    const Volume m_shares;

  public:
    MarketEvent(OrderId id, ORDER_TYPE type, Volume m_shares, Price price, std::chrono::system_clock::time_point time);
    OrderId getOrderId();
    ORDER_TYPE getOrderType();
    std::chrono::system_clock::time_point getEventTime();
    Volume getShares();
};

#endif