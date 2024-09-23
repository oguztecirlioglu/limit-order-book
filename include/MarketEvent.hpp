#ifndef MARKET_EVENT_H
#define MARKET_EVENT_H

#include "Common.hpp"
#include <chrono>

class MarketEvent {
  private:
    const EVENT_TYPE m_eventType;
    const OrderId m_id;
    const ORDER_TYPE m_orderType;
    const Price m_price;
    const std::chrono::system_clock::time_point m_nanosecondsSinceMidnight;
    const Volume m_shares;

  public:
    MarketEvent(EVENT_TYPE eventType, OrderId id, ORDER_TYPE orderYype, Volume m_shares, Price price, std::chrono::system_clock::time_point time);
    EVENT_TYPE getEventType();
    OrderId getOrderId();
    ORDER_TYPE getOrderType();
    std::chrono::system_clock::time_point getEventTime();
    Volume getShares();
    Price getPrice();
};

#endif