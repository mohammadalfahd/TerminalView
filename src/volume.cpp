#include "../include/indicators/volume.h"

void draw_volume(GridConfig &config,std::vector<candle>& data,Viewport &Viewport){
    
    vector<vector<string>> vol_grid(config.total_height,vector<string>(config.total_width+1," "));

    for(int i=0;i<config.total_height;i++){

        vol_grid[i][0]="\033[36m|\033[0m";
        vol_grid[i][config.total_width-1]="\033[36m|\033[0m"; 
    }
    for(int i=0;i<config.chart_height;i++){

        vol_grid[i][config.chart_width+1]="\033[36m|\033[0m";    
    }

    int bottom=config.total_height-1;

    for(int i=0;i<config.total_width;i++){

        vol_grid[bottom][i]="\033[36m=\033[0m";
        vol_grid[config.chart_height+1][i]="\033[36m=\033[0m";
        vol_grid[0][i]="\033[36m=\033[0m";
    }

    vol_grid[bottom][0]="\033[36m+\033[0m";
    
    const int candle_spacing=config.spacing;
    int highest_volume=get_highest_volume(data,Viewport);
    int lowest_volume=get_lowest_volume(data,Viewport);


    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());

    int visible = end - Viewport.first_visible_candle;

    for(int screen_index = 0; screen_index < visible; screen_index++){
    int data_index = Viewport.first_visible_candle + screen_index;

        int vol_y=scale(config,data[data_index].volume,highest_volume,lowest_volume);


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
    const int y_label_count = 5;

    for (int i = 0; i < y_label_count; i++)
    {
        int row = i * (config.chart_height - 1) / (y_label_count - 1);

        int volume =highest_volume -row * (highest_volume - lowest_volume) / (config.chart_height - 1);

        vol_grid[row+1][config.chart_width + 2] = to_string(volume);
    }

    int x_label_count = 3;

    for (int i = 0; i < x_label_count; i++)
    {   
        int screen_index=i*(Viewport.candle_count-1)/(x_label_count-1);
        int candle_index =Viewport.first_visible_candle+screen_index;

        int x = 1 + screen_index ;

        vol_grid[config.total_height-2][x] =data[candle_index].timestamp;
            
    }

        for(int i=0;i<vol_grid.size();i++){
        
            for(int j=0;j<vol_grid[0].size();j++){
            cout<<vol_grid[i][j];
            }

        cout<<endl;
    }

}