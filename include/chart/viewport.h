#pragma once

#include "../terminal/terminal.h"
#include <vector>
using namespace std;

class candle;
struct Viewport{

    public:
    int first_visible_candle=0;
    
    int viewport_height=get_terminal_dimension().first;
    int viewport_width=get_terminal_dimension().second;

    int candle_count;

    int selected_candle;


    void pan(vector<candle> &data,int direction);
    void select_candle(int direction);
    void update_layout();
};

