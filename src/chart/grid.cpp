#include "chart/grid.h"

int scale(GridConfig &config,double value,double highest,double lowest){

    double gap=highest-lowest;

    if(gap==0){
        return config.chart_height/2;
    }
    else{
    double scaled_val=(value-lowest)*(config.chart_height-1)/gap;

    return config.chart_height - 1 - scaled_val;
    } 
}

void GridConfig :: update_grid_config(Viewport &visible_region){
    chart_height=visible_region.viewport_height-5-x_label_height;
    candle_count=visible_region.candle_count;
    chart_width=spacing*candle_count;
    total_width=chart_width+y_label_width+3;
    total_height=visible_region.viewport_height;
}
