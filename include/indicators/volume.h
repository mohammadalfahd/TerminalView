#pragma once

#include <vector>
#include "../chart/grid.h"
#include "../data/candle.h"

void draw_volume(
    GridConfig &grid,
    std::vector<candle>& candles,
    Viewport &Viewport
);