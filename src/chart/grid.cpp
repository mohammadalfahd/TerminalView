#include "chart/grid.h"

#include <cmath>

int scale(GridConfig &config, double value, double highest, double lowest)
{
    double gap = highest - lowest;

    if (gap == 0)
        return config.chart_height / 2;

    double normalized = (value - lowest) / gap;
    int row = static_cast<int>(std::round((1.0 - normalized) * (config.chart_height - 1)));

    if (row < 0) row = 0;
    if (row >= config.chart_height) row = config.chart_height - 1;

    return row;
}

void GridConfig :: update_grid_config(Viewport &visible_region){
    chart_height=visible_region.viewport_height-5-x_label_height;
    candle_count=visible_region.candle_count;
    chart_width=spacing*candle_count;
    total_width=chart_width+y_label_width+3;
    total_height=visible_region.viewport_height;
}
