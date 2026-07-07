#pragma once

#include <vector>

#include "candle.h"
#include "viewport.h"
#include "grid.h"
#include "indicators.h"
using namespace std;

class Chart
{
private:

    std::vector<candle> data;

    Viewport viewport;

    Indicators indicators;

    GridConfig config;

public:

    Chart();

vector<candle> load_csv(string file_address);

    void select_candle(Viewport &viewport,int direction);
    
    void render(vector<candle> &data,GridConfig &config,Viewport &Viewport,Indicators &Indicators);

    void pan(Viewport &Viewport,vector<candle> &data,int direction);

};