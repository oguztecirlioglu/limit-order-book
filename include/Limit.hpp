#ifndef LIMIT_HPP
#define LIMIT_HPP

#include "Order.hpp"

#include <boost/unordered/unordered_flat_map.hpp>

#include <unordered_map>
#include <vector>

class Limit {
  private:
    const Price m_price;
    Volume m_totalVolume;

    std::vector<Order *> orders;                                     // Linked List of orders
    boost::unordered::unordered_flat_map<OrderId, size_t> ordersMap; // Random access to orders to remove in O1, key is order ID.

    void setTotalVolume(Volume newVolume);

  public:
    Limit(Price priceLevel);
    ~Limit();
    Price getPrice();
    Volume getTotalVolume();
    Volume getOrderVolume(OrderId orderId);
    OrderId add(Order *newOrder);
    OrderId remove(OrderId orderId);
    OrderId reduceOrder(OrderId orderId, Volume volChange);
};

#endif