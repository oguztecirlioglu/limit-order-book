#include "MarketEvent.hpp"

MarketEvent::MarketEvent(OrderId id,
                         ORDER_TYPE type,
                         Volume shares,
                         Price price,
                         std::chrono::system_clock::time_point time)
    : m_id(id), m_type(type), m_price(price), m_shares(shares), m_nanosecondsSinceMidnight(time) {
    // Don't think I need a body for this.
}

OrderId MarketEvent::getOrderId() {
    return m_id;
}

ORDER_TYPE MarketEvent::getOrderType() {
    return m_type;
}
std::chrono::system_clock::time_point MarketEvent::getEventTime() {
    return m_nanosecondsSinceMidnight;
}
Volume MarketEvent::getShares() {
    return m_shares;
}