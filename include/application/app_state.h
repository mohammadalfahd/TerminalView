#pragma once

#include <iostream>
#include <vector>

#include "../terminal/terminal.h"
#include "../chart/viewport.h"
#include "../chart/grid.h"
#include "../data/candle.h"
#include "../data/csv_loader.h"
#include "../network/api_call.h"
#include "../network/json_parser.h"
#include "../indicators/indicators.h"

class app_state{

    std::vector<candle> datapoint;

    Viewport viewport;
    Indicators toggles;

    sma sma20;
    ema ema20;
    macd macd;
    rsi rsi14;

    network_client httpclient;
    json_parser json;

    std::mutex mtx;

    std::atomic<bool> running(true);
    std::atomic<bool> polling(false);

}