#include "Limit.hpp"
#include <iostream>
#include <stdexcept>

Limit::Limit(int priceLevel) : m_price(priceLevel), m_totalVolume(0), orders(60) {
    // Constructor body (empty in this case)
}

Limit::~Limit() {
    for (auto order : orders) {
        delete order;
    }
    orders.clear();
}

Price Limit::getPrice() {
    return this->m_price;
}

Volume Limit::getTotalVolume() {
    return this->m_totalVolume;
}

/**
 * @brief Sets the total volume for this price limit / level.
 *
 * @param newVolume New total volume of this limit, must be greater than zero.
 * @throws std::invalid_argument if the volume is negative.
 */
void Limit::setTotalVolume(int newVolume) {
    // State Invariant 1: Total volume can't be less than zero.
    if (newVolume < 0) {
        throw std::invalid_argument("Received negative value for newVolume, should be 0 or more.");
    }
    this->m_totalVolume = newVolume;
    return;
}

Volume Limit::getOrderVolume(int orderId) {
    if (not this->orders.contains(orderId))
        throw std::invalid_argument("Cannot find volume, as order does not exist.");
    return this->orders.get(orderId)->getShares();
}

/**
 * @brief O(1) Operation.
 * Adds a new order to this limit, pushing it to the back of the list, and creating a
 * hash map entry of the orderId - iterator pair, so that the order can be removed in O(1).
 * Adds the volume of the newOrder to the totalVolume for this limit.
 *
 * @param newOrder
 */
OrderId Limit::add(Order *newOrder) {
    this->orders.push_back(newOrder);

    int existingVolume = this->getTotalVolume();
    this->setTotalVolume(existingVolume + newOrder->getShares());

    return newOrder->getId();
}

/**
 * @brief O(1) operation.
 * Removes an existing order from this limit.
 * Also removes the entry from the hashmap, and removes however many shares the order contained
 * from the total volume.
 *
 * @param orderToBeDeleted
 *
 * @throws std::invalid_argument when order does not exist in ordersMap.
 */
OrderId Limit::remove(int orderId) {
    if (not this->orders.contains(orderId))
        throw std::invalid_argument("Remove - Order does not exist in ordersMap");

    Order *order = this->orders.get(orderId);
    int existingVolume = this->getTotalVolume();
    int removedVolume = order->getShares();
    this->setTotalVolume(existingVolume - removedVolume); // Remove volume from total volume for this limit.

    this->orders.erase(orderId);
    return orderId;
}

/**
 * @brief
 *
 * @param orderId
 * @param volChange
 * @return int
 *
 * @throws std::invalid_argument when order does not exist in ordersMap.
 */
OrderId Limit::reduceOrder(int orderId, int volChange) {
    if (not orders.contains(orderId))
        throw std::invalid_argument("Reduce Order - Order does not exist in ordersMap");

    Order *order = this->orders.get(orderId);
    int orderVolume = order->getShares();

    if (orderVolume <= volChange)
        return this->remove(orderId);

    order->setShares(orderVolume - volChange);
    this->setTotalVolume(this->getTotalVolume() - volChange); // Remove volume from total volume for this limit.

    return orderId;
}
