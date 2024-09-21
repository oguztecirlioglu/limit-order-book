#include "Order.hpp"
#include "Common.hpp"
#include <iostream>

Order::Order(OrderId id, ORDER_TYPE type, Volume shares, Price price, std::chrono::system_clock::time_point time)
    : m_id(id), m_type(type), m_shares(shares), m_price(price), m_nanosecondsSinceMidnight(time) // Initializer list
{
    // Constructor body (empty in this case)
}

int Order::getId() {
    return this->m_id;
}

int Order::getShares() {
    return this->m_shares;
}

void Order::setShares(int newShares) {
    this->m_shares = newShares;
}

ORDER_TYPE Order::getType() {
    return this->m_type;
}
int Order::getPrice() {
    return this->m_price;
}
std::chrono::system_clock::time_point Order::getTime() {
    return this->m_nanosecondsSinceMidnight;
}