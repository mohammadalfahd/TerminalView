#include "../include/grid.h"

int scale(GridConfig &config,int value,int highest,int lowest){

    int gap=highest-lowest;

    if(gap==0){
        return config.chart_height/2;
    }
    else{
    int scaled_val=(value-lowest)*(config.chart_height-1)/gap;

    return config.chart_height - 1 - scaled_val;
    } 
}


