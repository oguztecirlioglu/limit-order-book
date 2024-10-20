#include "LOB.hpp"

#include "MarketEvent.hpp"
#include "Utility.hpp"

#include <iostream>
#include <vector>

LOB::LOB() {
    std::cout << "LOB constructed." << std::endl;
}

LOB::~LOB() {
    std::cout << m_bid_table.bucket_count() << " " << m_ask_table.bucket_count() << std::endl;

    for (auto &[p, l] : m_bid_table) {
        delete l;
    }
    m_bid_table.clear();
    m_bid.clear();

    for (auto &[p, l] : m_ask_table) {
        delete l;
    }
    m_ask_table.clear();
    m_ask.clear();
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
        auto buyLim = m_bid_table.find(newOrder->getPrice());
        if (buyLim != m_bid_table.end()) {
            lim = buyLim->second;
        } else {
            lim = new Limit(newOrder->getPrice());
            m_bid[newOrder->getPrice()] = lim;
            m_bid_table[newOrder->getPrice()] = lim;
        }
    } else if (orderType == ORDER_TYPE::SELL) {
        auto sellLim = m_ask_table.find(newOrder->getPrice());
        if (sellLim != m_ask_table.end()) {
            lim = sellLim->second;
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
 * Adjusts the volume of an order by a specified amount and updates the corresponding limit.
 *
 * @param orderId The ID of the order to be partially cancelled.
 * @param volume The volume to be removed from the order.
 * @param price The price of the order.
 * @param type The type of the order (BUY/SELL).
 * @return int ID of the cancelled order, whose shares have been reduced.
 *
 * @throws std::invalid_argument if the amount of volume to cancel exceeds the total volume of the order.
 * @throws std::invalid_argument if the order is not in either m_bid_table or m_ask_table
 */
int LOB::cancel(OrderId orderId, Volume volume, Price price, ORDER_TYPE type) {
    // Get order, change its volume by desired amount, update relevant limit to reflect decreased volume as well.
    auto buyLim = m_bid_table.find(price);
    auto sellLim = m_ask_table.find(price);

    if (buyLim == m_bid_table.end() and sellLim == m_ask_table.end())
        throw std::invalid_argument("orderId not found in bid or ask tables.");

    Limit *lim = type == ORDER_TYPE::BUY ? buyLim->second : sellLim->second;

    if (lim->getOrderVolume(orderId) < volume)
        throw std::invalid_argument("Amount of volume cancelled in partial cancellation should be less than the total volume of the order.");

    lim->reduceOrder(orderId, volume);

    return orderId;
}

/**
 * @brief Completely deletes order.
 *
 * @param orderId
 * @param volume
 * @param price
 * @param type
 * @return int ID of deleted order.
 */
int LOB::totalDelete(OrderId orderId, Volume volume, Price price, ORDER_TYPE type) {
    auto buyLim = m_bid_table.find(price);
    auto sellLim = m_ask_table.find(price);

    if (buyLim == m_bid_table.end() and sellLim == m_ask_table.end())
        throw std::invalid_argument("orderId not found in bid or ask tables.");

    Limit *lim = type == ORDER_TYPE::BUY ? buyLim->second : sellLim->second;
    return lim->remove(orderId);
}

/**
 * @brief Executes an existing order. Volume and direction needs to match that of the targeted existing order.
 *
 * @param existingORder
 * @return int ID of the existing order that we executed (took liquidity from)
 *
 * @throws std::invalid_argument if orderID is not found in either bid or ask tables.
 *
 */
int LOB::execute(OrderId orderId, Volume volume, Price price, ORDER_TYPE type) {
    auto buyLim = m_bid_table.find(price);
    auto sellLim = m_ask_table.find(price);
    Limit *lim;

    if (type == ORDER_TYPE::BUY and buyLim != m_bid_table.end())
        lim = m_bid_table[price];
    else if (type == ORDER_TYPE::SELL and sellLim != m_ask_table.end())
        lim = m_ask_table[price];
    else
        throw std::invalid_argument("Price not found in both ask and bid tables.");
    // PARTIAL REMOVALS CAN HAPPEN DURING EXECUTION,
    Volume originalOrderVolume = lim->getOrderVolume(orderId);
    if (originalOrderVolume < volume)
        throw std::invalid_argument("Trying to execute too much volume from this order.");
    else if (originalOrderVolume == volume)
        lim->remove(orderId);
    else
        lim->reduceOrder(orderId, volume);
    return orderId;
}

/**
 * @brief Returns total volume at a certain price limit
 *
 * @param limit
 * @return int
 *
 * @throws std::invalid_argument if limit (Price limit) does not exist.
 */
Volume LOB::getVolumeAtLimit(Price limit) {
    auto buyLim = m_bid_table.find(limit);
    auto sellLim = m_ask_table.find(limit);

    if (sellLim != m_ask_table.end())
        return m_ask_table[limit]->getTotalVolume();
    else if (buyLim != m_bid_table.end())
        return m_bid_table[limit]->getTotalVolume();

    throw std::invalid_argument("Limit not found.");
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
    std::vector<MarketEvent> events = Utility::loadStartOfDaySnapshot(fileDir);
    for (auto &event : events) {
        Order *order = Utility::marketEventToOrder(event);
        LOB::add(order);
    }
}