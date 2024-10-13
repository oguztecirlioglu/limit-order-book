#ifndef LIMIT_HPP
#define LIMIT_HPP

#include "FastVec.hpp"
#include "LinkedHashMap.hpp"
#include "Order.hpp"
#include <boost/unordered/unordered_flat_map.hpp>
#include <vector>

class Limit {
  private:
    const Price m_price;
    Volume m_totalVolume;

    LinkedHashMap<Order *> orders;
    // FastVec<Order *> orders;

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