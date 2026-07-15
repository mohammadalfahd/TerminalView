#pragma once

#include <vector>
#include <iostream>
#include "candle.h"
#include "viewport.h"
#include "grid.h"
#include "indicators.h"


using namespace std;

class Renderer
{
public:


    vector<vector<string>> draw_grid(GridConfig &config);

    void draw_axes(vector<vector<string>> &grid,GridConfig &config);
    void x_draw_label(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport);

    void y_draw_label(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport);
    void draw_candle(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport);

    void draw_moving_average( vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport,sma &sma20);

    void draw_exponential_moving_average(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport,ema &ema20);

    void render(std::vector<candle>& data,GridConfig& config,Viewport& viewport,Indicators& indicators,sma &sma20,ema &ema20,macd &macd);
    
    void print_grid(vector<vector<string>> &grid);

    void status_bar(Viewport &Viewport,vector<candle> &data);

};