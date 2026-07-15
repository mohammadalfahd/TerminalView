#pragma once

#include <queue>
#include "../candle.h"

class sma{
    
    public:
    int period;
    double rolling_sum;
    queue<double> window;
    vector<double> sma_val;
    
    sma(int p=20){period=p;};
    void initialise(std::vector<candle> &data);
    void initialise(vector<double> &data);
    void update(candle &newcandle);
    void update(double &val);
};