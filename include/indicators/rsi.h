#pragma once

#include <vector>
#include "../grid.h"
#include "../candle.h"


constexpr double RSI_OVERBOUGHT = 70.0;
constexpr double RSI_NEUTRAL    = 50.0;
constexpr double RSI_OVERSOLD   = 30.0;

struct rsipoint{

    double avg_gain;
    double avg_loss;

    double rs;
    double rsi;
};

vector<rsipoint> calculate_RSI( vector<candle>& data,int period = 14);
void draw_rsi(GridConfig& config, vector<candle> &data, Viewport& viewport);

