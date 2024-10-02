#include "Utility.hpp"
#include "Common.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

void Utility::createStartOfDaySnapshot(std::vector<Order> &missingOrders, const std::string &path) {
    std::ofstream outFile(path);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + path);
    }

    for (auto &order : missingOrders) {
        outFile << order.getTime() << ","
                << "1" << ","
                << order.getId() << ","
                << order.getShares() << ","
                << order.getPrice() << ","
                << (order.getType() == ORDER_TYPE::BUY ? "1" : "-1")
                << std::endl;
    }

    outFile.close();
}

std::vector<MarketEvent> Utility::loadStartOfDaySnapshot(const std::string &path) {
    std::ifstream inFile(path);
    if (!inFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + path);
    }

    std::vector<MarketEvent> events;
    std::string line;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string token;

        std::getline(iss, token, ',');
        std::chrono::nanoseconds ms(std::stoull(token));
        std::chrono::system_clock::time_point timestamp(ms);

        std::getline(iss, token, ',');
        int eventTypeInt = std::stoi(token);
        EVENT_TYPE eventType = eventTypeMap.at(eventTypeInt);

        std::getline(iss, token, ',');
        OrderId orderId = std::stoi(token);

        std::getline(iss, token, ',');
        Volume shares = std::stoi(token);

        std::getline(iss, token, ',');
        Price price = std::stoi(token);

        std::getline(iss, token, ',');
        ORDER_TYPE type = (std::stoi(token) == 1 ? ORDER_TYPE::BUY : ORDER_TYPE::SELL);

        events.emplace_back(MarketEvent(EVENT_TYPE::SUBMIT, orderId, type, shares, price, timestamp));
    }

    inFile.close();
    return events;
}

std::vector<Order> Utility::findMissingOrders(const std::string &messageFilePath) {
    std::ifstream file(messageFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + messageFilePath);
    }

    std::unordered_map<OrderId, Order> knownOrders;
    std::unordered_map<OrderId, Order> missingOrders;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        // Parse the CSV line
        std::getline(iss, token, ',');
        std::chrono::nanoseconds ms(std::stoull(token));
        std::chrono::system_clock::time_point timestamp(ms);

        std::getline(iss, token, ',');
        int eventType = std::stoi(token);

        std::getline(iss, token, ',');
        OrderId orderId = std::stoull(token);

        std::getline(iss, token, ',');
        int shares = std::stoi(token);

        std::getline(iss, token, ',');
        Price price = std::stod(token);

        std::getline(iss, token, ',');
        ORDER_TYPE direction = token == "1" ? ORDER_TYPE::BUY : ORDER_TYPE::SELL;

        switch (eventType) {
        case 1: // Add
            knownOrders.emplace(orderId, Order(orderId, direction, shares, price, timestamp));
            break;
        case 2: // Cancel (partial deletion)
        case 3: // Delete
        case 4: // Execute Visible
            if (not knownOrders.contains(orderId) and
                not missingOrders.contains(orderId)) {
                // This is a missing order

                constexpr long long secondsSinceMidnight = 8 * 3600; // 28800 seconds
                constexpr long long nanosecondsSinceMidnight = secondsSinceMidnight * 1'000'000'000LL;
                std::chrono::system_clock::time_point tradeTime =
                    std::chrono::system_clock::time_point(std::chrono::nanoseconds(nanosecondsSinceMidnight));

                Order missingOrder(orderId, direction, shares, price, tradeTime);
                missingOrders.emplace(orderId, missingOrder);
            } else if (missingOrders.contains(orderId)) { // Cancelling more shares from an already missing order.
                auto order = missingOrders.find(orderId);
                Volume origShareCount = order->second.getShares();
                order->second.setShares(origShareCount + shares);
            }
            break;
        case 5: // Execute Hidden
            // Ignore hidden executions as they don't impact visible orders
            break;
        }
    }

    file.close();

    std::vector<Order> result;
    for (const auto &[orderId, order] : missingOrders) {
        result.push_back(order);
    }

    return result;
}

Order *Utility::marketEventToOrder(MarketEvent &event) {
    return new Order(event.getOrderId(), event.getOrderType(), event.getShares(), event.getPrice(), event.getEventTime());
}