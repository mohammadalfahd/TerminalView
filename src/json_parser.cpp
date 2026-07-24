#include "../include/json_parser.h"

bool json_parser :: parse_json(std::string &response){
    json_data=nlohmann::json::parse(response);

    if(json_data.empty()){
        std::cerr<<"Parsing Failed \n";
        return false;
    }
    else{
        //std::cout<<"Parsing successfull\n"; ->Disabled for Finalisation
        return true;}
}

bool json_parser :: set_data(vector<candle> &data){

    bool dup_flag=false;
    
    for(nlohmann::json candle_json:json_data){
        if(data.empty()){
            data.push_back(extract_candle(candle_json));
            
        }
        else if(data.back().timestamp==std::to_string(candle_json[0].get<long long>())){
            data.back()=extract_candle(candle_json);
            dup_flag=true;
        }
        else{
            data.push_back(extract_candle(candle_json));
            
        }

    }
    return dup_flag;
} 


candle json_parser :: extract_candle(nlohmann::json &candle_json){
    candle data_point;

    data_point.timestamp = std::to_string(candle_json[0].get<long long>());
    data_point.open_price=std::stod(candle_json[1].get<std::string>());    
    data_point.high_price=std::stod(candle_json[2].get<std::string>());
    data_point.low_price=std::stod(candle_json[3].get<std::string>());
    data_point.closing_price=std::stod(candle_json[4].get<std::string>());
    data_point.adj_close=data_point.closing_price;
    data_point.volume=std::stod(candle_json[5].get<std::string>());
    
    return data_point;
}

