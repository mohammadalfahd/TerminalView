#include "../include/indicators/ema.h"
#include <cmath>
void ema :: initialise(vector<double> &data,sma &sma20){

    double prev_ema=sma20.sma_val[sma20.period-1];

    for(int i=0;i<period-1;i++){
        ema_val.push_back(NAN);
    }

    for(int i=period-1;i<data.size();i++){
        double ema=prev_ema+multiplier*(data[i]-prev_ema);
        ema_val.push_back(ema);
        prev_ema=ema;
    }

}
void ema :: initialise(vector<candle> &data,sma &sma20){

    double prev_ema=sma20.sma_val[sma20.period-1];

    for(int i=0;i<period-1;i++){
        ema_val.push_back(NAN);
    }

    for(int i=period-1;i<data.size();i++){
        double ema=prev_ema+multiplier*(data[i].closing_price-prev_ema);
        ema_val.push_back(ema);
        prev_ema=ema;
    }

}

void ema :: update(candle &newcandle){
    double prev_ema=ema_val.back();

    double ema=prev_ema+multiplier*(newcandle.closing_price-prev_ema); 

    ema_val.push_back(ema);
}

void ema :: update(double &val){
    double prev_ema=ema_val.back();


    double ema=prev_ema+multiplier*(val-prev_ema); 

    ema_val.push_back(ema);
}

void ema :: refresh(candle &newcandle){
    double prev_ema=ema_val[ema_val.size()-2];

    double ema=prev_ema+multiplier*(newcandle.closing_price-prev_ema); 

    ema_val.back()=ema;
}

void ema :: refresh(double &val){
    double prev_ema=ema_val[ema_val.size()-2];

    double ema=prev_ema+multiplier*(val-prev_ema); 

    ema_val.back()=ema;
}