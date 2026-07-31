#ifndef INDICATORS_H
#define INDICATORS_H

#include <vector>
#include <iostream>

#include "../data/candle.h"
#include "volume.h"
#include "macd.h"
#include "rsi.h"
#include "sma.h"
#include "ema.h"

using namespace std;
class Indicators
{   public:
    bool sma = false;
    bool ema = false;
    bool volume = false;
    bool macd = false;
    bool rsi = false;

    
};


#endif