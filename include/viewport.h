#ifndef VIEWPORT_H
#define VIEWPORT_H

struct Viewport{
    int first_visible_candle=0;
    int candle_count=20;

    int selected_candle=19;
};

#endif