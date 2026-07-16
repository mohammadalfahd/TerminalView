#pragma once

#include <vector>
#include "../grid.h"
#include "../candle.h"



class rsi{
    public:
    int period=14;

    double avg_gain=0;
    double avg_loss=0;
    
    vector<double> rsi_val;
    
    candle prev_candle;

    double RSI_OVERBOUGHT = 70.0;
    double RSI_NEUTRAL    = 50.0;
    double RSI_OVERSOLD   = 30.0;

    void initialise(vector<candle> &data);
    void update(candle &newcandle);

    void draw_rsi(GridConfig& config, vector<candle> &data, Viewport& viewport);
};



