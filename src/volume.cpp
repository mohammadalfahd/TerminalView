#include "../include/indicators/volume.h"

void draw_volume(GridConfig &config,std::vector<candle>& data,Viewport &Viewport){
    
    vector<vector<string>> vol_grid(config.total_height,vector<string>(config.total_width+1," "));

    for(int i=0;i<config.chart_height;i++){
        vol_grid[i][0]="\033[36m|\033[0m";
    }

    for(int i=0;i<config.chart_width;i++){
        vol_grid[config.chart_height][i]="\033[36m=\033[0m";
    }
    vol_grid[config.chart_height][config.chart_width]="+";
    
    const int candle_spacing=config.spacing;
    int highest_volume=get_highest_volume(data,Viewport);
    int lowest_volume=get_lowest_volume(data,Viewport);


    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());

    int visible = end - Viewport.first_visible_candle;

    for(int screen_index = 0; screen_index < visible; screen_index++){
    int data_index = Viewport.first_visible_candle + screen_index;

        int vol_y=vol_scale(config,data[data_index].volume,highest_volume,lowest_volume);


        bool selected=(screen_index==Viewport.selected_candle);

        int x = 1 + screen_index * config.spacing;


        for(int y = vol_y; y < config.chart_height ; y++){

            if(data[data_index].open_price<data[data_index].closing_price){
                vol_grid[y][x]="\033[32m█\033[0m";
            }
            else {vol_grid[y][x]="\033[31m█\033[0m";}
            if(selected){
                vol_grid[y][x]="\033[33m█\033[0m";
            }
        }
    }
    
        for(int i=0;i<vol_grid.size();i++){
        
            for(int j=0;j<vol_grid[0].size();j++){
            cout<<vol_grid[i][j];
            }

        cout<<endl;
    }

}