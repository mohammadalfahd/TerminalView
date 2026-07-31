#include "../include/candle.h"

double get_highest_price(vector<candle> &data,Viewport &Viewport){
    double highest_price=0;
    double end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle;i<end;i++){
        if(data[i].high_price>highest_price){
            highest_price=data[i].high_price;
        }
    }

    return highest_price;
}

double get_lowest_price(vector<candle> &data,Viewport &Viewport){
    
    double lowest_price = data[0].low_price;
    double end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle;i<end;i++){
        if(data[i].low_price<lowest_price){
            lowest_price=data[i].low_price;
        }
    }

    return lowest_price;
}

double get_highest_volume(vector<candle> &data,Viewport &Viewport){

    double highest_volume=data[Viewport.first_visible_candle].volume;
    double end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle+1;i<end;i++){
        if(data[i].volume>highest_volume){
            highest_volume=data[i].volume;
        }
    }

    return highest_volume;


}

double get_lowest_volume(vector<candle> &data,Viewport &Viewport){
    
    double lowest_volume = data[Viewport.first_visible_candle].volume;
    double end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle+1;i<end;i++){
        if(data[i].volume<lowest_volume){
            lowest_volume=data[i].volume;
        }
    }
    return lowest_volume;
}