#ifndef INDICATORS_H
#define INDICATORS_H

#include <vector>
#include <iostream>

#include "candle.h"

using namespace std;
class Indicators
{   public:
    bool sma = false;
    bool ema = false;
    bool volume = false;
    bool macd = false;
    bool rsi = false;

    double calculate_moving_average(vector<candle> &data,int index,int period = 20);
    double calculate_exponential_moving_average(vector<candle> &data,int index,int period = 20);

};

#endif