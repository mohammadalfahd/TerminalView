#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "data/candle.h"
using namespace std;

vector<candle> load_csv(string &file_address);

#endif