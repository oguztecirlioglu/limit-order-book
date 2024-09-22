#include "LOB.hpp"

#include <iostream>
#include <vector>

LOB::LOB() {
    std::cout << "LOB constructed." << std::endl;
}

/**
 * @brief Adds a new order to the end of the list of orders for that limit, if it's a new limit, order is placed at the head of the new limit.
 *
 * @param newOrder
 * @return int The ID of the newly added order.
 */
int LOB::add(Order *newOrder) {
    // If hashmap contains price-limit, get limit, add order to end of limit
    // Else, create new limit, add it ordered_map & hashmap, add order to new limit.
    ORDER_TYPE orderType = newOrder->getType();
    Limit *lim;

    if (orderType == ORDER_TYPE::BUY) {
        if (m_bid_table.contains(newOrder->getPrice())) {
            lim = m_bid_table[newOrder->getPrice()];
            lim->add(newOrder);
        } else {
            lim = new Limit(newOrder->getPrice());
            m_bid[newOrder->getPrice()] = lim;
            m_bid_table[newOrder->getPrice()] = lim;
        }
    } else if (orderType == ORDER_TYPE::SELL) {
        if (m_ask_table.contains(newOrder->getPrice())) {
            lim = m_ask_table[newOrder->getPrice()];
        } else {
            lim = new Limit(newOrder->getPrice());
            m_ask[newOrder->getPrice()] = lim;
            m_ask_table[newOrder->getPrice()] = lim;
        }
    }

    lim->add(newOrder);
    return newOrder->getId();
}

/**
 * @brief Partially cancels an existing order.
 *
 * @param existingOrder
 * @return int ID of the cancelled order, whose shares have been reduced, or -1 if too many shares were removed or order doesn't exist.
 */
int LOB::cancel(OrderId orderId, Volume volume, Price price, ORDER_TYPE type) {
    // Get order, change its volume by desired amount, update relavant limit to reflect decreased volume as well.
    Limit *lim = type == ORDER_TYPE::BUY ? m_bid_table[orderId] : m_ask_table[orderId];

    if (lim->getOrderVolume(orderId) <= volume)
        return -1; // return erroneous ID because partial deletion shouldn't take up the total or more of the orders volume.

    lim->reduceOrder(orderId, volume);

    lim->setTotalVolume(lim->getTotalVolume() - volume);

    return orderId;
}

/**
 * @brief Completely deletes order.
 *
 * @param orderId
 * @param volume
 * @param price
 * @param type
 * @return int ID of deleted order, or -1 if a mismatch between share count and/or the existence of the order itself occurs.
 */
int LOB::totalDelete(OrderId orderId, Volume volume, Price price, ORDER_TYPE type) {
    Limit *lim = type == ORDER_TYPE::BUY ? m_bid_table[orderId] : m_ask_table[orderId];
    return lim->remove(orderId);
}

/**
 * @brief Executes an existing order. Volume and direction needs to match that of the targeted existing order.
 *
 * @param existingORder
 * @return int ID of the existing order that we executed (took liquidity from), or -1 if it doesn't exist.
 */
int LOB::execute(OrderId orderId, Volume volume, Price price, ORDER_TYPE type) {
    Limit *lim;
    if (type == ORDER_TYPE::BUY and m_bid_table.contains(orderId))
        lim = m_bid_table[orderId];
    else if (type == ORDER_TYPE::SELL and m_ask_table.contains(orderId))
        lim = m_ask_table[orderId];
    else
        return -1;

    return lim->remove(orderId);
}

/**
 * @brief Returns total volume at a certain price limit, or -1 if that limit does not exist.
 *
 * @param limit
 * @return int
 */
Volume LOB::getVolumeAtLimit(Price limit) {
    if (m_ask_table.contains(limit))
        return m_ask_table[limit]->getTotalVolume();
    else if (m_bid_table.contains(limit))
        return m_bid_table[limit]->getTotalVolume();
    return -1;
}

/**
 * @brief Gets the best price, depending on which side of the book (bid or ask) you want.
 *
 * @param type
 * @return int
 */
Price LOB::getBestPrice(ORDER_TYPE type) {
    return ORDER_TYPE::BUY == type ? m_bid.begin()->first : m_ask.begin()->first;
}

void LOB::printBook(int depth) {
    // ask price 1, ask size 1, bid price 1, bid size 2 ... bid size n.
    //  eg depth == 2 -> 5859400,200,5853300,18,5859800,200,5853000,150
    std::ostringstream oss;

    auto bidItr = m_bid.begin();
    auto askItr = m_ask.begin();

    for (int i = 0; i < depth && bidItr != m_bid.end() && askItr != m_ask.end(); ++i, ++bidItr, ++askItr) {
        if (i > 0)
            oss << ',';
        oss << askItr->second->getPrice() << ',' << askItr->second->getTotalVolume() << ','
            << bidItr->second->getPrice() << ',' << bidItr->second->getTotalVolume();
    }

    oss << '\n';
    std::cout << oss.str();
}

void LOB::loadSnapshot(std::string fileDir) {
    
}