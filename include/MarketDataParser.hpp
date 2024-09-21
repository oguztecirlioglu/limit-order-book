#ifndef MARKET_DATA_PARSER_H
#define MARKET_DATA_PARSER_H

#include "MarketEvent.hpp"

#include <string>
#include <vector>

class MarketDataParser {
    // open file
    // get next event
  private:
    std::vector<MarketEvent> marketEvents;

  public:
    MarketDataParser(std::string filepath);
    void LoadAllEvents(); // Loads all event to memory, don't have to do FileSystem I/O, faster.
    MarketEvent getNextEvent();
};

#endif