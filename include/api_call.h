#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>

class network_client{
    public:
    
    CURL* curl;
    CURLcode result;

    std::string response;

    std::string symbol="BTCUSDT";
    std::string interval="1h";
    size_t limit=200;
    time_t start_time;
    time_t end_time;

    std::string url;

    long timeout=5L;
    long follow_redirects=1L;
    std::string user_agent="TerminalView";
    std::string accept_encoding="";
    struct curl_slist* headers=nullptr;

    //temp for debug only
    long verbose=1L;
    char errorbuffer[CURL_ERROR_SIZE]={0};

    network_client(){
        curl=curl_easy_init();

        if(!curl){
            std::cout<<"Failed Initialization\n";
            return ;
        }
    }

    ~network_client(){
        if(curl){
            curl_easy_cleanup(curl);
        }
        if(headers){
            curl_slist_free_all(headers);
        }
    }

    void make_url(){

        url="https://api.binance.com/api/v3/klines?symbol="+symbol+"&interval="+interval+"&limit="+std::to_string(limit);

    }
    static size_t writecallback(void* content,size_t size,size_t nmemb,void* userp);
    void setup();
    bool fetch_data();
};

