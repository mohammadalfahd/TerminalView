#include "../include/candle.h"

int get_highest_price(vector<candle> &data,Viewport &Viewport){
    int highest_price=0;
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle;i<end;i++){
        if(data[i].high_price>highest_price){
            highest_price=data[i].high_price;
        }
    }

    return highest_price;
}

int get_lowest_price(vector<candle> &data,Viewport &Viewport){
    
    int lowest_price = data[0].low_price;
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle;i<end;i++){
        if(data[i].low_price<lowest_price){
            lowest_price=data[i].low_price;
        }
    }

    return lowest_price;
}