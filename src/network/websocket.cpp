#include "websocket.h"
#include <iostream>
void websocketclient ::connect()
{
    websocket.start();
}

void websocketclient ::make_url(std::string &symbol, std::string &timeframe)
{
    std::string symbol = symbol;
    std::string timeframe = timeframe;

    std::transform(symbol.begin(), symbol.end(), symbol.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::transform(timeframe.begin(), timeframe.end(), timeframe.begin(), [](unsigned char c)
                   { return std::tolower(c); });
                   
    url = "wss://stream.binance.com:9443/ws/" + symbol + "@kline_" + timeframe;
}

void websocketclient ::onMessage(const ix::WebSocketMessagePtr &msg)
{

    switch (msg->type)
    {
    case ix::WebSocketMessageType::Message:
        std::cout << "\nCandle Recieved\n";
        /*Code For other stuff*/
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
