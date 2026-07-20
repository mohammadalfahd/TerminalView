#include "../include/indicators/macd.h"
#include <cmath>

double macd :: get_max_macd(Viewport &Viewport){

    double highest = max(max(macd_val[Viewport.first_visible_candle],signal[Viewport.first_visible_candle]),histogram[Viewport.first_visible_candle]);
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(macd_val.size()));
    for(int i=Viewport.first_visible_candle+1;i<end;i++){
        
        highest = max(highest,max(max(macd_val[i],signal[i]),histogram[i])); 
    }

    return highest;
}

double macd :: get_min_macd(Viewport &Viewport){

    double lowest = min(min(macd_val[Viewport.first_visible_candle],signal[Viewport.first_visible_candle]),histogram[Viewport.first_visible_candle]);
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(macd_val.size()));
    for(int i=Viewport.first_visible_candle+1;i<end;i++){
    
        lowest = min(lowest, min(min(macd_val[i],signal[i]),histogram[i]));
    }

    return lowest;
}
void macd :: initialise(vector<candle> &data){

    sma sma12(12);
    sma sma26(26);
    sma sma9(9);


    sma12.initialise(data);
    sma26.initialise(data);
    ema12.initialise(data,sma12);
    ema26.initialise(data,sma26);

    for(int i=0;i<ema26.period-1;i++){
        macd_val.push_back(NAN);
    }
    vector<double> valid_macd;
    for(int i=ema26.period-1;i<data.size();i++){
        macd_val.push_back(ema12.ema_val[i]-ema26.ema_val[i]);
        valid_macd.push_back(ema12.ema_val[i]-ema26.ema_val[i]);
    }

    sma9.initialise(valid_macd);
    ema9.initialise(valid_macd,sma9);

    signal.insert(signal.begin(),ema26.period-1+ema9.period-1,NAN);
    for(int i=0;i<ema9.period-1;i++){
        signal.push_back(NAN);
    }
    for(int i=ema9.period-1;i<data.size();i++){
        signal.push_back(ema9.ema_val[i]);
    }

    for(int i=0;i<ema26.period-1;i++){
        histogram.push_back(NAN);
    }
    for(int i=ema26.period-1;i<data.size();i++){
        histogram.push_back(macd_val[i]-signal[i]);
    }

    //CHECKS

}

void macd :: update(candle &newcandle){

    ema12.update(newcandle);
    ema26.update(newcandle);
    
    macd_val.push_back(ema12.ema_val.back()-ema26.ema_val.back());

    ema9.update(macd_val.back());
    signal.push_back(ema9.ema_val.back());

    histogram.push_back(macd_val.back()-signal.back());

}

void macd :: draw_macd(GridConfig &config,std::vector<candle>& data,Viewport &Viewport){

    vector<vector<string>> macd_grid(config.total_height,vector<string>(config.total_width," "));

    for(int i=0;i<config.total_height;i++){

        macd_grid[i][0]="\033[36m|\033[0m";
        macd_grid[i][config.total_width-1]="\033[36m|\033[0m"; 
    }
    for(int i=0;i<config.chart_height;i++){

        macd_grid[i][config.chart_width+1]="\033[36m|\033[0m";    
    }

    int zero_line=config.chart_height/2;

    for(int i=0;i<config.total_width;i++){

        macd_grid[zero_line][i]="\033[36m=\033[0m";
        macd_grid[config.chart_height+1][i]="\033[36m=\033[0m";
        macd_grid[0][i]="\033[36m=\033[0m";
    }

    macd_grid[zero_line][0]="\033[36m+\033[0m";

    int x_label_count = 3;

    for (int i = 0; i < x_label_count; i++)
    {   
        int screen_index=i*(Viewport.candle_count-1)/(x_label_count-1);
        int candle_index =Viewport.first_visible_candle+screen_index;

        int x = 1 + screen_index ;

        macd_grid[config.total_height-2][x] =data[candle_index].timestamp;
            
    }


    double highest =get_max_macd(Viewport);
    double lowest =get_min_macd(Viewport);

    double maxAbs = max(abs(highest), abs(lowest));

    highest = maxAbs;
    lowest  = -maxAbs;

    const int y_label_count = 19;

    for (int i = 0; i < y_label_count; i++){

        int row = i * ((config.chart_height) - 1) / (y_label_count - 1);

        double macd_label =highest -row * (highest - lowest) / ((config.chart_height) - 1);

        macd_grid[row][config.chart_width + 2] = to_string(macd_label);
        
    }

    int zero_row = scale(config, 0.0, highest, lowest);
    int visible_count = min(Viewport.candle_count,int(data.size()) - Viewport.first_visible_candle);

    for (int screen_index = 0;screen_index < visible_count;screen_index++)
    {
        int data_index = Viewport.first_visible_candle + screen_index;

        double hist_point = histogram[data_index];

        int hist_y = scale(config, hist_point, highest, lowest);

        bool selected = (screen_index == Viewport.selected_candle);

        int x = 1 + screen_index * config.spacing;
        if(hist_y < 0 || hist_y >= config.total_height ){
            continue;
        }
        if (hist_point >= 0)
        {

            for (int y = hist_y; y <= zero_row; y++)
            {
                macd_grid[y][x] = selected ?
                    "\033[33m█\033[0m" :
                    "\033[32m█\033[0m";
            }
        }
        else
        {

            for (int y = zero_row; y <= hist_y; y++)
            {
                macd_grid[y][x] = selected ?
                    "\033[33m█\033[0m" :
                    "\033[31m█\033[0m";
            }
        }
    }

    for (int screen_index = 0;screen_index < visible_count;screen_index++)
    {   int data_index = Viewport.first_visible_candle + screen_index;
        if(isnan(signal[data_index])){continue;}
        int macd_y   = scale(config,macd_val[data_index], highest, lowest);
        int signal_y = scale(config,signal[data_index], highest, lowest);
        
        int x = 1 + screen_index * config.spacing;

        if(signal_y < 0 || signal_y >= config.total_height ||macd_y < 0 || macd_y >= config.total_height ){
            continue;
        }
        
        macd_grid[macd_y][x]   = "\033[36m●\033[0m";   
        macd_grid[signal_y][x] = "\033[35m●\033[0m";   
    }

    for(int i=0;i<macd_grid.size();i++){

        for(int j=0;j<macd_grid[0].size();j++){
            cout<<macd_grid[i][j];
        }

        cout<<endl;
    }

}

