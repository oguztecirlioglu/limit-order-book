#ifndef COMMON_H
#define COMMON_H

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

#endif