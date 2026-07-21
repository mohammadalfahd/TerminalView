#pragma once

#include <nlohmann/json.hpp>
#include "candle.h"

class json_parser{
    public:

    nlohmann::json json_data;

    bool parse_json(std::string &response);

    std::vector<candle> set_data();
    candle extract_candle(nlohmann::json &candle_json);

};

