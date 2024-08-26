#ifndef ORDER_H
#define ORDER_H
#include "Common.hpp"

class Order {
   private:
    int id;
    ORDER_TYPE type;
    int shares;
    float price;
    int time;

   public:
    Order();
};

#endif