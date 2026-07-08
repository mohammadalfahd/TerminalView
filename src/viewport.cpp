#include "../include/viewport.h"
#include "../include/candle.h"

void Viewport :: pan(Viewport &Viewport,vector<candle> &data,int direction){
    int last_start = max(0, (int(data.size()) - Viewport.candle_count));

    if(direction==-1){
        if(Viewport.first_visible_candle!=0){
            Viewport.first_visible_candle--;
        }
    }
    else if (direction==1){
        if (Viewport.first_visible_candle + Viewport.candle_count < data.size())
        {
            Viewport.first_visible_candle++;
        }
    }
}

void Viewport :: select_candle(Viewport &viewport,int direction){
    if(direction==1 && viewport.selected_candle<19){
        viewport.selected_candle++;
    }
    else if(direction==-1 && viewport.selected_candle>0){
        viewport.selected_candle--;
    }
    
}