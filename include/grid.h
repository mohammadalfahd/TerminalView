#ifndef GRID_H
#define GRID_H

#include "viewport.h"

class GridConfig{
    public: 
    int y_label_width=1;
    int x_label_height=3;
    int spacing=2;
    int candle_count;
    int chart_height=20;
    int chart_width;


    int total_width;
    int total_height;



    GridConfig(){}
    
    GridConfig(Viewport &visible_region){

        candle_count=visible_region.candle_count;
        chart_width=spacing*candle_count;
        total_width=chart_width+y_label_width+3;
        total_height=chart_height+x_label_height+1;

    }
};

int scale(GridConfig &config,int price,int highest_price,int lowest_price);
int vol_scale(GridConfig &config,int volume,int highest_volume,int lowest_volume);

#endif