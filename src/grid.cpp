#include "../include/grid.h"

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


