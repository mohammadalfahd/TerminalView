#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <array>
#include <mutex>
#include <vector>

class network_client{
    public:
    
    CURL* curl;
    CURLcode result;

    std::string response;

    std::vector<std::string> symbol_array{"BTCUSDT","ETHUSDT","BNBUSDT"};
    int symbol_tracker=0;
    std::string symbol=symbol_array[symbol_tracker];
    const std::array<std::string,8> timeframe_array={"1s","1m","3m","5m","15m","30m","1h","1d"};
    int timeframe_tracker=3;

    size_t limit=200;
    time_t start_time;
    time_t end_time;


    std::string url;

    std::mutex curl_mtx;
    const long timeout=10L;
    const long follow_redirects=1L;
    const std::string user_agent="TerminalView";
    const std::string accept_encoding="";
    struct curl_slist* headers=nullptr;

    //temp for debug only
    long verbose=0L;
    char errorbuffer[CURL_ERROR_SIZE]={0};

    network_client(){
        curl=curl_easy_init();

        if(!curl){
            std::cout<<"Failed Initialization\n";
            return ;
        }
    }

    
    void switch_timeframe(int diretion);
    void add_symbol();
    void switch_symbol(int direction);
    static size_t writecallback(void* content,size_t size,size_t nmemb,void* userp);
    void setup();
    bool fetch_data();
    
    ~network_client(){
        if(curl){
            curl_easy_cleanup(curl);
        }
        if(headers){
            curl_slist_free_all(headers);
        }
    }

};

