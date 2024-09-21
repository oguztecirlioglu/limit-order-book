#include <chrono>
#include <iostream>

#include "Common.hpp"
#include "LOB.hpp"
#include "Order.hpp"

int main() {
    std::cout << "hello world!" << std::endl;
    auto lob = LOB();
    auto sampleOrder = Order(OrderId{123},
                             ORDER_TYPE::BUY,
                             Volume{123},
                             Price{512},
                             std::chrono::system_clock::now());
    lob.add(&sampleOrder);
    std::cout << "Best Price is: " << lob.getBestPrice(ORDER_TYPE::BUY) << std::endl;
    return 0;
}