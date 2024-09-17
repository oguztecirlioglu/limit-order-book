#include "Limit.hpp"
#include <stdexcept>

Limit::Limit(int priceLevel) : m_price(priceLevel), m_totalVolume(0) {
    // Constructor body (empty in this case)
}

int Limit::getPrice() {
    return this->m_price;
}

int Limit::getTotalVolume() {
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

/**
 * @brief O(1) Operation.
 * Adds a new order to this limit, pushing it to the back of the list, and creating a
 * hash map entry of the orderId - iterator pair, so that the order can be removed in O(1).
 * Adds the volume of the newOrder to the totalVolume for this limit.
 *
 * @param newOrder
 */
int Limit::add(Order &newOrder) {
    orders.push_back(newOrder);
    this->ordersMap[newOrder.getId()] = orders.end();

    int existingVolume = this->getTotalVolume();
    this->setTotalVolume(existingVolume + newOrder.getShares());

    return newOrder.getId();
}

/**
 * @brief O(1) operation.
 * Removes an existing order from this limit, otherwise throws an exception.
 * Also removes the entry from the hashmap, and removes however many shares the order contained
 * from the total volume.
 *
 * @param orderToBeDeleted
 */
int Limit::remove(int id) {
    auto it = this->ordersMap.find(id);
    if (it == this->ordersMap.end())
        return -1;

    this->orders.erase(it->second);
    this->ordersMap.erase(id);

    int existingVolume = this->getTotalVolume();
    int removedVolume = it->second->getShares();
    this->setTotalVolume(existingVolume - removedVolume);

    return id;
}

int Limit::editOrder(int id, int volChange) {
    auto it = this->ordersMap.find(id);
    if (it == this->ordersMap.end())
        return -1;

    it->
}