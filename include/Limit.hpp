#ifndef LIMIT_H
#define LIMIT_H

#include "Order.hpp"
#include <list>
#include <unordered_map>

class Limit {
  private:
    const int m_price;
    int m_totalVolume;

    std::list<Order> orders;                                       // Linked List of orders
    std::unordered_map<int, std::list<Order>::iterator> ordersMap; // Random access to orders to remove in O1, key is order ID.

  public:
    Limit(int priceLevel);
    int getPrice();
    int getTotalVolume();
    void setTotalVolume(int newVolume);
    int add(Order &newOrder);
    Order getOrder(int orderId);
    int remove(int orderId);
    int editOrder(int orderId, int volChange);
};

#endif