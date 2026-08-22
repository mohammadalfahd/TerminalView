#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <atomic>
#include <chrono>

class websocketclient{
    public:
        ix::WebSocket websocket;

        std::string url;
        std::string response;

        std::atomic<bool> connected{false};
        std::atomic<bool> reconnect_requested{false};
        websocketclient(){
            
            websocket.setOnMessageCallback(
                [this](const ix::WebSocketMessagePtr &msg){
                    
                    onMessage(msg);  
                    
                });

            
            }
        
        ~websocketclient(){
            websocket.stop();
        }
        
        
        void connect(std::string &symbol,const std::string &timeframe);
        void make_url(std::string &symbol,const std::string &timeframe);
        void onMessage(const ix::WebSocketMessagePtr &msg);
        void reconnect();
};