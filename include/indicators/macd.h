#pragma once


#include <vector>
#include "../grid.h"
#include "../candle.h"
#include "ema.h"

class macd{
    public:

    ema ema12;
    ema ema26;
    ema ema9;
    macd() : ema12(12),ema26(26),ema9(9) {};
    
    vector<double> macd_val;
    vector<double> signal;
    vector<double> histogram;

    void initialise(vector<candle> &data);
    void update(candle &newcandle);

    double get_max_macd(Viewport &Viewport);
    double get_min_macd(Viewport &Viewport);
    
    void draw_macd(GridConfig &grid,std::vector<candle>& candles,Viewport &Viewport);
};



