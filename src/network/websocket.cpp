#include "network/websocket.h"
#include <iostream>
#include <nlohmann/json.hpp>

void websocketclient ::connect(std::string &symbol,const std::string &timeframe)
{   
    make_url(symbol,timeframe);
    websocket.setUrl(url);
    websocket.start();
}

void websocketclient ::make_url(std::string &symbol,const std::string &timeframe)
{
    std::string Symbol = symbol;
    std::string Timeframe = timeframe;

    std::transform(Symbol.begin(), Symbol.end(), Symbol.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::transform(Timeframe.begin(), Timeframe.end(), Timeframe.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    url = "wss://stream.binance.com:9443/ws/" + Symbol + "@kline_" + Timeframe;
}

void websocketclient ::onMessage(const ix::WebSocketMessagePtr &msg)
{

    switch (msg->type)
    {
    case ix::WebSocketMessageType::Message:
        {
        std::cout << "\nCandle Recieved\n";
        response=msg->str;
        }
        break;

    case ix::WebSocketMessageType::Error:
        std::cerr << "\nWebSocket Fucked\n";
        break;
    case ix::WebSocketMessageType::Open:
        std::cout << "\nConnection Estalished \n";
        
        break;
    case ix::WebSocketMessageType::Close:
        std::cout << "\nConnection Closed\n";
        break;
    default:
        break;
    }
}

