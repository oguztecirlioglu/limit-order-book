#include "MarketEvent.hpp"

MarketEvent::MarketEvent(EVENT_TYPE eventType,
                         OrderId id,
                         ORDER_TYPE orderType,
                         Volume shares,
                         Price price,
                         std::chrono::system_clock::time_point time)
    : m_eventType(eventType), m_id(id), m_orderType(orderType), m_price(price), m_shares(shares), m_nanosecondsSinceMidnight(time) {
    // Don't think I need a body for this.
}

EVENT_TYPE MarketEvent::getEventType() {
    return m_eventType;
};

OrderId MarketEvent::getOrderId() {
    return m_id;
}

ORDER_TYPE MarketEvent::getOrderType() {
    return m_orderType;
}

std::chrono::system_clock::time_point MarketEvent::getEventTime() {
    return m_nanosecondsSinceMidnight;
}
Volume MarketEvent::getShares() {
    return m_shares;
}

Price MarketEvent::getPrice() {
    return m_price;
}