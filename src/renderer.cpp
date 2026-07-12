#include "../include/renderer.h"

vector<vector<string>> Renderer :: draw_grid(GridConfig &config){

    vector<vector<string>> grid(config.total_height,vector<string>(config.total_width," "));
    
    return grid;
}

void Renderer :: draw_axes(vector<vector<string>> &grid,GridConfig &config){

    for(int i=0;i<config.total_height;i++){

        grid[i][0]="\033[36m|\033[0m";
        grid[i][config.total_width-1]="\033[36m|\033[0m"; 
    }
    for(int i=0;i<config.chart_height;i++){

        grid[i][config.chart_width+1]="\033[36m|\033[0m";    
    }

    int bottom=config.total_height-1;

    for(int i=0;i<config.total_width;i++){

        grid[bottom][i]="\033[36m=\033[0m";
        grid[config.chart_height+1][i]="\033[36m=\033[0m";
        grid[0][i]="\033[36m=\033[0m";
    }

    grid[bottom][0]="\033[36m+\033[0m";
}

void Renderer :: y_draw_label(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport){
    int highest = get_highest_price(data,Viewport);
    int lowest  = get_lowest_price(data,Viewport);

    const int label_count = 5;

    for (int i = 0; i < label_count; i++)
    {
        int row = i * (config.chart_height - 1) / (label_count - 1);

        int price =
            highest -
            row * (highest - lowest) / (config.chart_height - 1);

        grid[row+1][config.chart_width + 2] = to_string(price);
    }
}

void Renderer :: x_draw_label(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport){

    int label_count = 3;

    for (int i = 0; i < label_count; i++)
    {   
        int screen_index=i*(Viewport.candle_count-1)/(label_count-1);
        int candle_index =Viewport.first_visible_candle+screen_index;

        int x = 1 + screen_index ;

        grid[config.total_height-2][x] =data[candle_index].timestamp;
            
    }
}

void Renderer :: draw_candle(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport){

    const int candle_spacing=config.spacing;
    int highest_price=get_highest_price(data,Viewport);
    int lowest_price=get_lowest_price(data,Viewport);

    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());

    int visible = end - Viewport.first_visible_candle;

    for(int screen_index = 0; screen_index < visible; screen_index++){
    int data_index = Viewport.first_visible_candle + screen_index;
        
        int high_y=scale(config,data[data_index].high_price,highest_price,lowest_price);
        int low_y=scale(config,data[data_index].low_price,highest_price,lowest_price);
        int open_y=scale(config,data[data_index].open_price,highest_price,lowest_price);
        int close_y=scale(config,data[data_index].closing_price,highest_price,lowest_price);

        bool selected=(screen_index==Viewport.selected_candle);

        int x = 1 + screen_index * config.spacing;

        int body_top=max(open_y,close_y);
        int body_bottom=min(open_y,close_y);
        
        
        for(int j=low_y;j>=body_bottom;j--){

            grid[j][x]="|";
        }
        for(int j=body_top+1;j>=high_y;j--){

            grid[j][x]="|";
        }
        for(int j=body_bottom;j<=body_top;j++){

            if(open_y>close_y){
                grid[j][x]="\033[32m█\033[0m";
            }
            else {grid[j][x]="\033[31m█\033[0m";}
            if(selected){
                grid[j][x]="\033[33m█\033[0m";
            }
        }
    }
}

void Renderer :: draw_moving_average(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport,int period=20){
    int highest_price = get_highest_price(data, Viewport);
    int lowest_price = get_lowest_price(data, Viewport);

    bool first_point = true;
    

    int prev_x;
    int prev_y;
    

    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());
    int visible = end - Viewport.first_visible_candle;

    for (int screen_index = 0; screen_index < visible; screen_index++)
    {   
        int data_index=screen_index+Viewport.first_visible_candle;
        double sma=calculate_moving_average(data,data_index,period);
        int y = scale(config, sma, highest_price, lowest_price);
        int x = 1 + screen_index * config.spacing;
        if (y < 0 || y >= config.chart_height)
            continue;
        if (first_point)
        {   
            grid[y][x] = "\033[95m*\033[0m";

            prev_x = x;
            prev_y = y;
            first_point = false;
        }
        else
        {
            grid[y][x] = "\033[95m*\033[0m";

            prev_x = x;
            prev_y = y;
        }
    }
}

void Renderer :: draw_exponential_moving_average(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport,int period=20){
    int highest_price = get_highest_price(data, Viewport);
    int lowest_price = get_lowest_price(data, Viewport);

    bool first_point = true;
    

    int prev_x;
    int prev_y;
    

    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());
    int visible = end - Viewport.first_visible_candle;

    for (int screen_index = 0; screen_index < visible; screen_index++)
    {   
        int data_index=screen_index+Viewport.first_visible_candle;
        double ema=calculate_exponential_moving_average(data,data_index,period);
        int y = scale(config, ema, highest_price, lowest_price);
        int x = 1 + screen_index * config.spacing;
        if (data_index < period - 1){
            continue;
        }
        if (y < 0 || y >= config.chart_height)
            continue;
        if (first_point)
        {   
            grid[y][x] = "\033[38;5;208m*\033[0m";

            prev_x = x;
            prev_y = y;
            first_point = false;
        }
        else
        {
            grid[y][x] = "\033[38;5;208m*\033[0m";

            prev_x = x;
            prev_y = y;
        }
    }
}

void Renderer :: render(vector<candle> &data,GridConfig &config,Viewport &Viewport,Indicators &Indicators){
    auto grid=draw_grid(config);

    draw_axes(grid,config);
    y_draw_label(grid,data,config,Viewport);
    x_draw_label(grid,data,config,Viewport);

    draw_candle(grid,data,config,Viewport);
    
    if(Indicators.sma){
        draw_moving_average(grid,data,config,Viewport,20);
    }
    if(Indicators.ema){
        draw_exponential_moving_average(grid,data,config,Viewport,20);
    }
    
    print_grid(grid);
    
    if(Indicators.volume){
        draw_volume(config,data,Viewport);
    }

    if(Indicators.macd){
        draw_macd(config,data,Viewport);
    }
    
    if(Indicators.rsi){
        draw_rsi(config,data,Viewport);
    }
    
    status_bar(Viewport,data);

}

void Renderer :: print_grid(vector<vector<string>> &grid){

    for(int i=0;i<grid.size();i++){
        
        for(int j=0;j<grid[i].size();j++){
            cout<<grid[i][j];
        }

        cout<<endl;
    }
}


void Renderer :: status_bar(Viewport &Viewport,vector<candle> &data){
    int data_index=Viewport.first_visible_candle+Viewport.selected_candle;
    double range = data[data_index].high_price - data[data_index].low_price;
    double change =data[data_index].closing_price -data[data_index].open_price;
    double percent =change * 100.0 /data[data_index].open_price;

    double rsi=calculate_RSI(data)[data_index].rsi;

    cout<<"\033[36m*-------------------------------------------------------------*"<<endl<<endl;
    cout<<"📅 Date : "<<data[data_index].timestamp<<endl;
    cout<<"\033[38;5;214m 🕯️ Candle Type : ";
    if(data[data_index].open_price<data[data_index].closing_price){
        cout<<"\033[32mBullish"<<endl;
    }
    else if(data[data_index].open_price>data[data_index].closing_price){
        cout<<"\033[91mBearish"<<endl;
    }
    else{cout<<"\033[0;33mDoji"<<endl;}
    cout<<"\033[32m🟢 O : "<<data[data_index].open_price<<"   🔺 H : "<<data[data_index].high_price<<endl;
    cout<<"\033[91m🔻 L : "<<data[data_index].low_price<<"    🔴 C : "<<data[data_index].closing_price<<endl<<endl;
    cout<<"\033[38;5;214m📈 Change : "<<change<<"           📊 Change % : "<<percent<<endl;
    cout<<"\033[38;5;214m📏 Range : "<<range<<endl<<endl;
    cout<<"\033[38;5;214m📦 Volume : "<<data[data_index].volume<<endl;
    cout<<"\033[35m📈 SMA20 : "<<  calculate_moving_average(data,data_index,20) <<"           \033[38;5;202m📈 EMA20 : "<<  calculate_exponential_moving_average(data,data_index,20) <<endl;
    cout<<"\033[38;5;214m📈 RSI20 : "<<rsi <<endl;
    cout<<"\033[36m*-------------------------------------------------------------*\033[0m"<<endl;

}