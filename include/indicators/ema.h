#pragma once

#include "../candle.h"
#include "sma.h"


class ema{

    public:
    double period;
    double multiplier=double(2)/(period+1);

    ema(int p=20){period=p; multiplier=double(2)/(period+1);}
    vector<double> ema_val;
    void initialise(vector<candle> &data,sma &sma20);
    void initialise(vector<double> &data,sma &sma20);
    void update(candle &newcandle);
    void update(double &val);

};