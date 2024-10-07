#ifndef MARKET_DATA_PARSER_HPP
#define MARKET_DATA_PARSER_HPP

#include "LOB.hpp"
#include "MarketEvent.hpp"

#include <fstream>
#include <string>
#include <vector>

class MarketDataParser {
    // Open file
    // Loads all event to memory, don't have to do FileSystem I/O, faster.
    // Get Next event
    // Send order to LOB.
  private:
    std::vector<MarketEvent> m_marketEvents;
    LOB *m_limitOrderBook;
    std::ifstream m_inFileStream;

  public:
    MarketDataParser(std::string filepath, LOB *lob);
    ~MarketDataParser();
    void processEvent(MarketEvent *event);
    std::vector<MarketEvent> getAllEvents();
    MarketEvent *getNextEvent();
    void resetFileStream();
};

#endif