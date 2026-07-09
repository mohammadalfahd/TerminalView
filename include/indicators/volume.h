#pragma once

#include <vector>
#include "../grid.h"
#include "../candle.h"

void draw_volume(
    GridConfig &grid,
    std::vector<candle>& candles,
    Viewport &Viewport
);