#include "Common.hpp"
#ifndef LOB_H
#define LOB_H

class LOB {
   private:
   public:
    int add();
    int cancel();
    int execute();
    int getVolumeAtLimit(float limit);
    float getBestPrice(ORDER_TYPE type);
    LOB();
};

#endif