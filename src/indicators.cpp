#include "../include/indicators.h"

double calculate_moving_average(vector<candle> &data,int index,int period ){

    
    double sum=0;
    double sma=0;
    for(int i=index;i>index-period;i--){
        if(i<0){
            continue;
        }
        sum+=data[i].closing_price;
    }
    sma =sum/period;

    return sma;
}

double calculate_exponential_moving_average(vector<candle> &data,int index,int period ){

    double prev_EMA=calculate_moving_average(data,period-1,period);
    double multiplier=double(2)/(period+1);


    if (index < period - 1){
            return 0;
            }
    for(int i=period;i<index;i++){
        
        prev_EMA=(data[i].closing_price*multiplier)+(prev_EMA*(1-multiplier));
    }

    double current_ema=(data[index].closing_price*multiplier)+(prev_EMA*(1-multiplier));

    return current_ema;
}