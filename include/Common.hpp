#ifndef COMMON_H
#define COMMON_H

#include <unordered_map>

enum class ORDER_TYPE { BUY = 1,
                        SELL = -1 };

enum class EVENT_TYPE { SUBMIT = 1,
                        CANCEL = 2,
                        DELETE = 3,
                        EXECUTE_VISIBLE = 4,
                        EXECUTE_HIDDEN = 5,
                        CROSS_TRADE = 6,
                        HALT = 7 };

using OrderId = int;
using Price = int;
using Volume = int;

const std::unordered_map<int, EVENT_TYPE> eventTypeMap = {
    {1, EVENT_TYPE::SUBMIT},
    {2, EVENT_TYPE::CANCEL},
    {3, EVENT_TYPE::DELETE},
    {4, EVENT_TYPE::EXECUTE_VISIBLE},
    {5, EVENT_TYPE::EXECUTE_HIDDEN},
    {6, EVENT_TYPE::CROSS_TRADE},
    {7, EVENT_TYPE::HALT}};

#endif