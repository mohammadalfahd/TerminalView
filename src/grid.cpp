#include "../include/grid.h"

int scale(GridConfig &config,int price,int highest_price,int lowest_price){

    int price_gap=highest_price-lowest_price;

    if(price_gap==0){
        return config.chart_height/2;
    }
    else{
    int scaled_price=(price-lowest_price)*(config.chart_height-1)/price_gap;

    return config.chart_height - 1 - scaled_price;
    } 
}