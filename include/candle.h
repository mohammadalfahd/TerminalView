#ifndef CANDLE_H
#define CANDLE_H

#include <iostream>

class candle
{
public:
    std::string timestamp;

    double open;
    double high;
    double low;
    double close;

    double adjClose;

    double volume;

};

#endif