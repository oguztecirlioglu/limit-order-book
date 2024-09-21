#include <chrono>
#include <iostream>

#include "Common.hpp"
#include "LOB.hpp"
#include "Order.hpp"

int main() {
    std::cout << "hello world!" << std::endl;
    auto lob = LOB();
    auto sampleBuy = Order(OrderId{123},
                           ORDER_TYPE::BUY,
                           Volume{123},
                           Price{512},
                           std::chrono::system_clock::now());

    auto sampleSell = Order(OrderId{123},
                            ORDER_TYPE::SELL,
                            Volume{123},
                            Price{512},
                            std::chrono::system_clock::now());

    lob.add(&sampleBuy);
    lob.add(&sampleSell);
    lob.printBook(5);
    return 0;
}