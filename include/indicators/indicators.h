#ifndef INDICATORS_H
#define INDICATORS_H

#include <vector>
#include <iostream>

#include "candle.h"
#include "indicators/volume.h"
#include "indicators/macd.h"
#include "indicators/rsi.h"
#include "indicators/sma.h"
#include "indicators/ema.h"

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