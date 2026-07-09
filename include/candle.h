#pragma once

#include <iostream>
#include <vector>
#include "viewport.h"
using namespace std;

class candle
{
public:
    
    string timestamp;
    double open_price;
    double high_price;
    double low_price;
    double closing_price;
    double adj_close;
    double volume;
    
    
    
};

int get_highest_price(vector<candle> &data,Viewport &Viewport);
int get_lowest_price(vector<candle> &data,Viewport &Viewport);

int get_highest_volume(vector<candle> &data,Viewport &Viewport);
int get_lowest_volume(vector<candle> &data,Viewport &Viewport);
