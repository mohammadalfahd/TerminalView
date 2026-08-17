#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include <string>
#include <algorithm>
#include <cctype>

class websocketclient{
    public:
        ix::WebSocket websocket;

        std::string url;
        std::string response;

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

};