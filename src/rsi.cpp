#include "../include/indicators/rsi.h"

vector<rsipoint> calculate_RSI( vector<candle>& data,int period){
    vector<rsipoint> rsi_data(data.size());
    rsipoint rsi_point;
    double tot_gain=0;
    double tot_loss=0;
    for(int i=1;i<=period;i++){
        double change=data[i].closing_price-data[i-1].closing_price;

        if(change<0){
            tot_loss+= -change;
        }
        else{
            tot_gain+=change;
        }
    }

    double avg_gain=tot_gain/period;
    double avg_loss=tot_loss/period;
    double rs=0;
    double rsi;
    if(avg_loss==0){
        rsi=100;
    }
    else{    
        rs=avg_gain/avg_loss;
        rsi=100-(100/(1+rs));
    }

    
    rsi_data[period].avg_gain=avg_gain;
    rsi_data[period].avg_loss=avg_loss;
    rsi_data[period].rsi=rsi;
    rsi_data[period].rs=rs;

    for(int i = period + 1; i < data.size(); i++){

        double change =data[i].closing_price -data[i-1].closing_price;

        double gain = 0;
        double loss = 0;

        if(change > 0){
            gain = change;
        }
        else{loss = -change;}

        avg_gain =((avg_gain * (period-1)) + gain) / period;

        avg_loss =((avg_loss * (period-1)) + loss) / period;

        if(avg_loss==0){
            rsi=100;
        }
        else{    
            rs=avg_gain/avg_loss;
            rsi=100-(100/(1+rs));
        }


        rsi_data[i].avg_gain=avg_gain;
        rsi_data[i].avg_loss=avg_loss;
        rsi_data[i].rsi=rsi;
        rsi_data[i].rs=rs;
    
    }

    return rsi_data;
}

void draw_rsi(GridConfig& config,vector<candle> &data, Viewport& Viewport){
    vector<rsipoint> rsi=calculate_RSI(data);
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

    const int y_label_count = 5;

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

    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());

    int visible = end - Viewport.first_visible_candle;

    for(int screen_index = 0; screen_index < visible; screen_index++){
        int data_index = Viewport.first_visible_candle + screen_index;

        double rsi_value=scale(config,rsi[data_index].rsi,100,0);

        bool selected=(screen_index==Viewport.selected_candle);

        int x = 1 + screen_index * config.spacing;

        if (rsi_value < 0 || rsi_value >= config.chart_height)
            continue;
        if(rsi[data_index].rsi>50){
            rsi_grid[rsi_value][x]="\033[32m●\033[0m";
        }
        else if(rsi[data_index].rsi<50){
            rsi_grid[rsi_value][x]="\033[31m●\033[0m";
        }
        else if(rsi[data_index].rsi=50){
            rsi_grid[rsi_value][x]="\033[38;2;255;165;0m●\033[0m";
        }
    
    }

    for(int i=0;i<rsi_grid.size();i++){

        for(int j=0;j<rsi_grid[0].size();j++){
            cout<<rsi_grid[i][j];
        }

        cout<<endl;
    }


}