#include "../include/indicators/sma.h"
#include <cmath>


void sma :: initialise(vector<double> &data){

    sma_val.clear();

    rolling_sum=0;
    for(int i=0;i<period-1;i++){
        sma_val.push_back(NAN);
        
    }
    for(int i=0;i<period;i++){
        rolling_sum += data[i];
        window.push(data[i]);
    }

    sma_val.push_back(rolling_sum/period);

    for (int i = period; i < data.size(); i++){
        rolling_sum += data[i];
        rolling_sum -= data[i - period];

        sma_val.push_back(rolling_sum / period);
    }
    
}

void sma :: initialise(vector<candle> &data){

    sma_val.clear();

    rolling_sum=0;
    for(int i=0;i<period-1;i++){
        sma_val.push_back(NAN);

    }
    for(int i=0;i<period;i++){
        rolling_sum += data[i].closing_price;
        window.push(data[i].closing_price);
    }

    sma_val.push_back(rolling_sum/period);

    for (int i = period; i < data.size(); i++){
        rolling_sum += data[i].closing_price;
        rolling_sum -= data[i - period].closing_price;

        sma_val.push_back(rolling_sum / period);
    }
    
}

void sma :: update(double &val){
    
    rolling_sum-=window.front();
    window.pop();
    
    rolling_sum+=val;
    window.push(val);

    sma_val.push_back(rolling_sum/period);

}

void sma :: update(candle &newcandle){
    
    rolling_sum-=window.front();
    window.pop();
    
    rolling_sum+=newcandle.closing_price;
    window.push(newcandle.closing_price);

    sma_val.push_back(rolling_sum/period);

}

void sma::refresh(vector<candle>& data){
    if(data.size() < period)
        return;

    double sum = 0;

    for(int i = data.size() - period; i < data.size(); i++)
    {
        sum += data[i].closing_price;
    }

    sma_val.back() = sum / period;
}

void sma :: refresh(double &val){
    
    rolling_sum-=window.front();
    window.pop();
    
    rolling_sum+=val;
    window.push(val);

    sma_val.back()=rolling_sum/period;

}