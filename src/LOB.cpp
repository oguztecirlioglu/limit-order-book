#include "LOB.hpp"

#include <iostream>

LOB::LOB() {
    std::cout << "LOB constructed." << std::endl;
}

/**
 * @brief
 *
 * @param newOrder
 * @return int The ID of the newly added order.
 */
int LOB::add(Order &newOrder) {
    auto [it, inserted] = this->m_bid.emplace(newOrder.getPrice(), Limit(newOrder.getPrice()));
    if (!inserted) {
        it->second.add(newOrder);
    }
    return newOrder.getId();
}

/**
 * @brief Fully or partially cancels an existing order. Volume and direction (Buy or sell) of existing order needs to match.
 *
 * @param existingOrder
 * @return int ID of the cancelled, previously existing order, or -1 if it doesn't exist.
 */
int LOB::cancel(int orderId, int volume, int price, ORDER_TYPE type) {
    // get order, change its volume by desired amount, if zero, delete order from limit.    
    if(type == ORDER_TYPE::BUY) {
        Limit lim = m_bid.find(price)->second;
        auto order = lim.getOrder(orderId);
        order.
    }
    return -1;
}

/**
 * @brief Executes an existing order. Volume and direction needs to match that of the targeted existing order.
 *
 * @param existingORder
 * @return int ID of the existing order that we executed (took liquidity from), or -1 if it doesn't exist.
 */
int LOB::execute(int orderId, int volume, int price, ORDER_TYPE type) {
    switch (type) {
    case ORDER_TYPE::BUY: {
        auto it = m_bid.find(price);
        if (it == m_bid.end())
            return -1;

        Limit lim = it->second;
        lim.
    }
    case ORDER_TYPE::SELL: {
        auto it = m_ask.find(price);
        if (it != m_ask.end()) {
            return it->second.remove(existingOrder.getId());
        }
    }
    }

    return -1;
}