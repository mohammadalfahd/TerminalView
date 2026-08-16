#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include <string>
#include <algorithm>
#include <cctype>

class websocketclient{
    public:
        ix::WebSocket websocket;

        std::string url;

        websocketclient(){}
        websocketclient(std::string &symbol,std::string &timeframe){
            
            make_url(symbol,timeframe);
            websocket.setUrl(url);
            websocket.setOnMessageCallback(
                [this](const ix::WebSocketMessagePtr &msg){
                    
                    onMessage(msg);  
                    
                });

            
            }
        
        ~websocketclient(){
            websocket.stop();
        }
        
        void connect();
        void make_url(std::string &symbol,std::string &timeframe);
        void onMessage(const ix::WebSocketMessagePtr &msg);

};