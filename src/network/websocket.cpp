#include "network/websocket.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

void websocketclient ::connect(std::string &symbol,const std::string &timeframe)
{   
    make_url(symbol,timeframe);
    websocket.setUrl(url);
    websocket.start();
}

void websocketclient :: reconnect(){
    int retry_delay = 1;

        if (!connected) {

            std::cout << "Connecting...\n";

            websocket.start();

            // Give the socket some time to establish connection
            std::this_thread::sleep_for(
                std::chrono::seconds(retry_delay)
            );

            if (connected) {
                retry_delay = 1;
            }
            else {
                retry_delay = std::min(retry_delay * 2, 30);
            }
        }

        else {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(500)
            );
        }
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
        //std::cout << "\nCandle Recieved\n"; -> Disabled For Finalisation
        response=msg->str;
        }
        break;

    case ix::WebSocketMessageType::Error:
        connected=false;
        reconnect_requested=true;
        std::cerr << "\nWebSocket Fucked\n";
        break;
    case ix::WebSocketMessageType::Open:
        connected=true;
        std::cout << "\nConnection Estalished \n";
        
        break;
    case ix::WebSocketMessageType::Close:
        connected=false;
        reconnect_requested=true;
        std::cout << "\nConnection Closed\n";
        break;
    default:
        break;
    }
}

