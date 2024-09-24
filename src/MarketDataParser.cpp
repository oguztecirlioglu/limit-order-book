#include "MarketDataParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

MarketDataParser::MarketDataParser(std::string filepath, LOB &lob) : limitOrderBook(lob) {
    // Load all events into this->marketEvents
    std::ifstream inFile(filepath);
    if (!inFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filepath);
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

        events.emplace_back(eventType, orderId, type, shares, price, timestamp);
    }

    this->marketEvents = events; // Causing problems
}

void MarketDataParser::processEvent(MarketEvent *event) {
    Order *newOrder = new Order(event->getOrderId(), event->getOrderType(), event->getShares(), event->getPrice(), event->getEventTime());

    switch (event->getEventType()) {
    case EVENT_TYPE::SUBMIT:
        this->limitOrderBook.add(newOrder);
        break;
    case EVENT_TYPE::CANCEL:
        this->limitOrderBook.cancel(event->getOrderId(), event->getShares(), event->getPrice(), event->getOrderType());
        break;
    case EVENT_TYPE::DELETE:
        this->limitOrderBook.totalDelete(event->getOrderId(), event->getShares(), event->getPrice(), event->getOrderType());
        break;
    case EVENT_TYPE::EXECUTE_VISIBLE:
        this->limitOrderBook.execute(event->getOrderId(), event->getShares(), event->getPrice(), event->getOrderType());
        break;
    case EVENT_TYPE::EXECUTE_HIDDEN:
        // Do nothing, as we are executing hidden stuff.
        break;
    case EVENT_TYPE::CROSS_TRADE:
        throw std::invalid_argument("Did not implement a CROSS_TRADE event :(");
        break;
    case EVENT_TYPE::HALT:
        throw std::invalid_argument("Did not implement a HALT event :(");
        break;
    }
}

std::vector<MarketEvent> MarketDataParser::getAllEvents() {
    return this->marketEvents;
}