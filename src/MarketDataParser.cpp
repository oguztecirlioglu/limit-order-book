#include "MarketDataParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

MarketDataParser::MarketDataParser(std::string filepath, LOB *lob) : m_limitOrderBook(lob) {
    // Load all events into this->m_marketEvents
    m_inFileStream.open(filepath);
    if (not m_inFileStream.is_open()) {
        throw std::runtime_error("Unable to open file: " + filepath);
    }
}

MarketDataParser::~MarketDataParser() {
    if (m_inFileStream.is_open()) {
        m_inFileStream.close();
    }
}

void MarketDataParser::processEvent(MarketEvent *event) {

    switch (event->getEventType()) {
        Order *newOrder;
    case EVENT_TYPE::SUBMIT:
        newOrder = new Order(event->getOrderId(), event->getOrderType(), event->getShares(), event->getPrice(), event->getEventTime());
        this->m_limitOrderBook->add(newOrder);
        break;
    case EVENT_TYPE::CANCEL:
        this->m_limitOrderBook->cancel(event->getOrderId(), event->getShares(), event->getPrice(), event->getOrderType());
        break;
    case EVENT_TYPE::DELETE:
        this->m_limitOrderBook->totalDelete(event->getOrderId(), event->getShares(), event->getPrice(), event->getOrderType());
        break;
    case EVENT_TYPE::EXECUTE_VISIBLE:
        this->m_limitOrderBook->execute(event->getOrderId(), event->getShares(), event->getPrice(), event->getOrderType());
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
    std::vector<MarketEvent> events;
    std::string line;

    while (std::getline(m_inFileStream, line)) {
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

    this->m_marketEvents = std::move(events);
    return this->m_marketEvents;
}

/**
 * @brief Retrieves the next MarketEvent from the file.
 *
 * This method reads the next line from the input file, parses it,
 * and creates a new MarketEvent based on the data.
 *
 * @return MarketEvent* A pointer to a newly created MarketEvent if successful,
 *                      or nullptr if the end of the file has been reached.
 *
 * @note Caller is responsible for deleting the returned MarketEvent
 *       to avoid memory leaks.
 */
MarketEvent *MarketDataParser::getNextEvent() {
    std::string line;

    if (std::getline(m_inFileStream, line)) {
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

        return new MarketEvent(eventType, orderId, type, shares, price, timestamp);
    }

    return nullptr;
}

void MarketDataParser::resetFileStream() {
    m_inFileStream.clear();
    m_inFileStream.seekg(0, std::ios::beg);
}