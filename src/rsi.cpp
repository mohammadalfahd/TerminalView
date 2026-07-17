#include "../include/indicators/rsi.h"
#include <cmath>
void rsi::initialise(vector<candle> &data) {

    if (data.size() <= period) return; 

    double change = 0;
    double tot_gain = 0;
    double tot_loss = 0;
    
    rsi_val.clear();


    for (int i = 0; i < period; i++) {
        rsi_val.push_back(NAN);
    }

    for (int i = 1; i <= period; i++) {
        change = data[i].closing_price - data[i - 1].closing_price;

        if (change > 0) {
            tot_gain += change;
        } else if (change < 0) {
            tot_loss += (-change);
        }
    }

    avg_gain = tot_gain / period;
    avg_loss = tot_loss / period;

    double rs = (avg_loss == 0) ? 0 : (avg_gain / avg_loss);
    rsi_val.push_back(100 - (100 / (rs + 1)));


    for (int i = period + 1; i < data.size(); i++) {
        change = data[i].closing_price - data[i - 1].closing_price;

        double current_gain;
        if (change > 0) {
            current_gain = change;
        } else {
            current_gain = 0.0;
        }
        double current_loss;
        if (change < 0) {
            current_loss = -change;
        } else {
            current_loss = 0.0;
        }

        avg_gain = ((avg_gain * (period - 1)) + current_gain) / period;
        avg_loss = ((avg_loss * (period - 1)) + current_loss) / period;

        if (avg_loss == 0) {
            rs = 0;
        } else {
            rs = avg_gain / avg_loss;
        }
        
        rsi_val.push_back(100 - (100 / (rs + 1)));
    }
}

void rsi::update(candle &new_candle) {

    double change = new_candle.closing_price - prev_candle.closing_price;


    double current_gain = (change > 0) ? change : 0.0;
    double current_loss = (change < 0) ? (-change) : 0.0;


    avg_gain = ((avg_gain * (period - 1)) + current_gain) / period;
    avg_loss = ((avg_loss * (period - 1)) + current_loss) / period;


    double rs = 0.0;
    double current_rsi = 0.0;

    if (avg_loss == 0) {
        if (avg_gain == 0) {
            current_rsi = 50.0; 
        } else {
            current_rsi = 100.0; 
        }
    } else {
        rs = avg_gain / avg_loss;
        current_rsi = 100.0 - (100.0 / (1.0 + rs));
    }


    rsi_val.push_back(current_rsi);
    prev_candle = new_candle; 
}

void rsi :: draw_rsi(GridConfig& config,vector<candle> &data, Viewport& Viewport){
    vector<vector<string>> rsi_grid(config.total_height,vector<string>(config.total_width," "));

    for(int i=0;i<config.total_height;i++){

        rsi_grid[i][0]="\033[36m|\033[0m";
        rsi_grid[i][config.total_width-1]="\033[36m|\033[0m"; 
    }
    for(int i=0;i<config.chart_height;i++){

        rsi_grid[i][config.chart_width+1]="\033[36m|\033[0m";    
    }

    int bottom=config.total_height-1;

    for(int i=0;i<config.total_width;i++){

        rsi_grid[bottom][i]="\033[36m=\033[0m";
        rsi_grid[config.chart_height+1][i]="\033[36m=\033[0m";
        rsi_grid[0][i]="\033[36m=\033[0m";
    }

    rsi_grid[bottom][0]="\033[36m+\033[0m";

    int highest_rsi = 100;
    int lowest_rsi  = 0;

    const int y_label_count = 19;

    for (int i = 0; i < y_label_count; i++)
    {
        int row = i * (config.chart_height - 1) / (y_label_count - 1);

        int rsi_label =
            highest_rsi -
            row * (highest_rsi - lowest_rsi) / (config.chart_height - 1);

        rsi_grid[row+1][config.chart_width + 2] = to_string(rsi_label);
    }
    
    int x_label_count = 3;

    for (int i = 0; i < x_label_count; i++)
    {   
        int screen_index=i*(Viewport.candle_count-1)/(x_label_count-1);
        int candle_index =Viewport.first_visible_candle+screen_index;

        int x = 1 + screen_index ;

        rsi_grid[config.total_height-2][x] =data[candle_index].timestamp;
            
    }

    int end = min((int)rsi_val.size(), Viewport.first_visible_candle + Viewport.candle_count);

    int visible = end - Viewport.first_visible_candle;

    for(int screen_index = 0; screen_index < visible; screen_index++){
        int data_index = Viewport.first_visible_candle + screen_index;
       
        if (data_index >= rsi_val.size()) continue;

        double rsi_value=scale(config,rsi_val[data_index],100,0);

        bool selected=(screen_index==Viewport.selected_candle);

        int x = 1 + screen_index * config.spacing;

        if (rsi_value < 0 || rsi_value >= config.chart_height)
            continue;
        if(rsi_val[data_index]>50){
            rsi_grid[rsi_value][x]="\033[32m●\033[0m";
        }
        else if(rsi_val[data_index]<50){
            rsi_grid[rsi_value][x]="\033[31m●\033[0m";
        }
        else if(rsi_val[data_index]==50){
            rsi_grid[rsi_value][x]="\033[38;2;255;165;0m●\033[0m";
        }
        if(screen_index==Viewport.selected_candle){
            rsi_grid[rsi_value][x]="\033[33m●\033[0m";
        }
    }

    for(int i=0;i<rsi_grid.size();i++){

        for(int j=0;j<rsi_grid[0].size();j++){
            cout<<rsi_grid[i][j];
        }

        cout<<endl;
    }


}