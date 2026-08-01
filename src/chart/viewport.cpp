#include "chart/viewport.h"
#include "data/candle.h"


void Viewport :: pan(vector<candle> &data,int direction){
    int last_start = max(0, (int(data.size()) - candle_count));

    if(direction==-1){
        if(first_visible_candle>0){
            first_visible_candle--;
        }
    }
    else if (direction==1){
        if (first_visible_candle< last_start)
        {
            first_visible_candle++;
        }
    }
}

void Viewport :: select_candle(int direction){
    if(direction == 1 && selected_candle < candle_count - 1)
        selected_candle++;
    
    else if(direction == -1 && selected_candle > 0)
        selected_candle--;
    
}

void Viewport :: update_layout(){

    auto [rows,cols]=get_terminal_dimension();

    viewport_height=rows;
    viewport_width=cols;

    int y_label_width=1;
    int x_label_height=3;
    int spacing =2;
    
    candle_count = std::max(1, (viewport_width - y_label_width - 10) / spacing);

    selected_candle = std::min(selected_candle, candle_count - 1);

}