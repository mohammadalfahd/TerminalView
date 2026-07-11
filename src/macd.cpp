#include "../include/indicators/macd.h"
#include "../include/indicators.h"


MACDData calculate_macd(vector<candle> &data){
    MACDData macd_data;

    macd_data.macd.resize(data.size());
    macd_data.signal.resize(data.size());
    macd_data.histogram.resize(data.size());

    for(int i=0;i<data.size();i++){
        double ema12 = calculate_exponential_moving_average(data,i,12);
        double ema26 = calculate_exponential_moving_average(data,i,26);

        macd_data.macd[i]=ema12-ema26;
        macd_data.signal[i]=calculate_exponential_moving_average(macd_data.macd,i,9);
        macd_data.histogram[i]=macd_data.macd[i]-macd_data.signal[i];

    }

    return macd_data;

}
double get_max_macd(MACDData &macd_data,Viewport &Viewport){

    double highest = max(max(macd_data.macd[Viewport.first_visible_candle],macd_data.signal[Viewport.first_visible_candle]),macd_data.histogram[Viewport.first_visible_candle]);
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(macd_data.macd.size()));
    for(int i=Viewport.first_visible_candle+1;i<end;i++){
    
        highest = max(highest,max(max(macd_data.macd[i],macd_data.signal[i]),macd_data.histogram[i])); 
    }

    return highest;
}

double get_min_macd(MACDData &macd_data,Viewport &Viewport){

    double lowest = min(min(macd_data.macd[Viewport.first_visible_candle],macd_data.signal[Viewport.first_visible_candle]),macd_data.histogram[Viewport.first_visible_candle]);
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(macd_data.macd.size()));
    for(int i=Viewport.first_visible_candle+1;i<end;i++){
    
        lowest = min(lowest, min(min(macd_data.macd[i],macd_data.signal[i]),macd_data.histogram[i]));
    }

    return lowest;
}

void draw_macd(GridConfig &config,std::vector<candle>& data,Viewport &Viewport){
    MACDData macd_data=calculate_macd(data);
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


    double highest =get_max_macd(macd_data,Viewport);
    double lowest =get_min_macd(macd_data,Viewport);

    double maxAbs = max(abs(highest), abs(lowest));

    highest = maxAbs;
    lowest  = -maxAbs;

    const int y_label_count = 5;

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

        double histogram = macd_data.histogram[data_index];

        int hist_y = scale(config, histogram, highest, lowest);

        bool selected = (screen_index == Viewport.selected_candle);

        int x = 1 + screen_index * config.spacing;

        if (histogram >= 0)
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
        int macd_y   = scale(config, macd_data.macd[data_index], highest, lowest);
        int signal_y = scale(config, macd_data.signal[data_index], highest, lowest);
        
        int x = 1 + screen_index * config.spacing;

        macd_grid[macd_y][x]   = "\033[36m●\033[0m";   
        if(signal_y < 0 || signal_y >= config.total_height)
{
    cout << "signal_y = " << signal_y
         << " data_index = " << data_index
         << endl;
        exit(0);
}

macd_grid[signal_y][x] = "\033[35m●\033[0m";   
    }

    for(int i=0;i<macd_grid.size();i++){

        for(int j=0;j<macd_grid[0].size();j++){
            cout<<macd_grid[i][j];
        }

        cout<<endl;
    }

}

