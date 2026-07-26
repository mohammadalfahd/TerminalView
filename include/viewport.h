#pragma once

#include "terminal.h"
#include <vector>
using namespace std;

class candle;
struct Viewport{

    public:
    int first_visible_candle=0;
    
    int viewport_height=get_terminal_dimension().first;
    int viewport_width=get_terminal_dimension().second;

    int candle_count=45;

    int selected_candle=44;


    void pan(Viewport &Viewport,vector<candle> &data,int direction);
    void select_candle(Viewport &viewport,int direction);
};

