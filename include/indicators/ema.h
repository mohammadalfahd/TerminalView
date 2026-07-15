#pragma once

#include "../candle.h"
#include "sma.h"


class ema{

    public:
    double period=20;
    double multiplier=double(2)/(period+1);
    vector<double> ema_val;

    void initialise(vector<candle> &data,sma &sma20);
    void update(candle &newcandle);

};