#ifndef LIMIT_H
#define LIMIT_H

#include "Order.hpp"
#include <list>
#include <unordered_map>

class Limit {
  private:
    const Price m_price;
    Volume m_totalVolume;

    std::list<Order *> orders;                                           // Linked List of orders
    std::unordered_map<OrderId, std::list<Order *>::iterator> ordersMap; // Random access to orders to remove in O1, key is order ID.

  public:
    Limit(Price priceLevel);
    Price getPrice();
    Volume getTotalVolume();
    void setTotalVolume(Volume newVolume);
    Volume getOrderVolume(OrderId orderId);
    OrderId add(Order *newOrder);
    OrderId remove(OrderId orderId);
    OrderId reduceOrder(OrderId orderId, Volume volChange);
};

#endif