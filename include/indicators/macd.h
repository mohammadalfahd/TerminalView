#pragma once


#include <vector>
#include "../grid.h"
#include "../candle.h"

struct MACDData
{
    vector<double> macd;
    vector<double> signal;
    vector<double> histogram;

    
};

double get_max_macd(MACDData &macd_data,Viewport &Viewport);

double get_max_macd(MACDData &macd_data,Viewport &Viewport);

MACDData calculate_macd(vector<candle> &data);
void draw_macd(GridConfig &grid,std::vector<candle>& candles,Viewport &Viewport);