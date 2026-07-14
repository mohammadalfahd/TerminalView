#pragma once

#include <queue>
#include "../candle.h"

class sma{
    
    public:
    int period=20;
    double rolling_sum;
    queue<double> window;
    vector<double> sma_val;
    
    void initialise(std::vector<candle> &data);
    void update(candle &newcandle);
};