#pragma once

#include <vector>
using namespace std;

class candle;
struct Viewport{

    public:
    int first_visible_candle=0;
    int candle_count=20;

    int selected_candle=19;

    void pan(Viewport &Viewport,vector<candle> &data,int direction);
    void select_candle(Viewport &viewport,int direction);
};

