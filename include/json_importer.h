#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "candle.h"

vector<candle> load_json(string &fileaddress){
    vector<candle> data;

    ifstream file(fileaddress);

    nlohmann::json json;

    file>>json;

    
}