#pragma once

#include <nlohmann/json.hpp>
#include "candle.h"

class json_parser{
    public:

    nlohmann::json json_data;

    bool parse_json(std::string &response);

    bool set_data(vector<candle> &data);

    candle extract_candle(nlohmann::json &candle_json);

};

