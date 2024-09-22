#ifndef MARKET_EVENT_H
#define MARKET_EVENT

#include "Common.hpp"
#include <chrono>

class MarketEvent {
  private:
    const int m_marketEventID;
    const OrderId m_id;
    const ORDER_TYPE m_type;
    const Price m_price;
    const std::chrono::system_clock::time_point m_nanosecondsSinceMidnight;
    const Volume m_shares;

  public:
    MarketEvent(int marketEventID, OrderId id, ORDER_TYPE type, Volume m_shares, Price price, std::chrono::system_clock::time_point time);
    OrderId getOrderId();
    ORDER_TYPE getOrderType();
    std::chrono::system_clock::time_point getEventTime();
    Volume getShares();
    int getMarketEventID();
};

#endif