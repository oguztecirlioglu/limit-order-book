#include "Limit.hpp"
#include <iostream>
#include <stdexcept>

Limit::Limit(int priceLevel) : m_price(priceLevel), m_totalVolume(0) {
    // Constructor body (empty in this case)
}

Limit::~Limit() {
    for (auto order : orders) {
        delete order;
    }
    ordersMap.clear();
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
    if (not this->ordersMap.contains(orderId))
        throw std::invalid_argument("Cannot find volume, as order does not exist.");
    return (*this->ordersMap[orderId])->getShares();
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
    orders.push_back(newOrder);
    this->ordersMap[newOrder->getId()] = --orders.end();

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
    if (not this->ordersMap.contains(orderId))
        throw std::invalid_argument("Order does not exist in ordersMap");
    std::list<Order *>::iterator it = this->ordersMap[orderId];
    int existingVolume = this->getTotalVolume();
    int removedVolume = (*it)->getShares();

    delete *it;
    this->orders.erase(it);
    this->ordersMap.erase(orderId);

    this->setTotalVolume(existingVolume - removedVolume); // Remove volume from total volume for this limit.

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
    if (not ordersMap.contains(orderId))
        throw std::invalid_argument("Order does not exist in ordersMap");

    int orderVolume = (*this->ordersMap[orderId])->getShares();

    if (orderVolume <= volChange)
        return this->remove(orderId);

    (*this->ordersMap[orderId])->setShares(orderVolume - volChange);
    this->setTotalVolume(this->getTotalVolume() - volChange); // Remove volume from total volume for this limit.

    return orderId;
}