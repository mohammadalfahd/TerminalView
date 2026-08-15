#include "network/api_call.h"

size_t network_client :: writecallback(void* content ,size_t size,size_t nmemb,void* userp){

    size_t totalbytes=size*nmemb;
    std::string* response=static_cast<std::string*>(userp);
    response->append(static_cast<char*>(content),totalbytes);

    return totalbytes;
}

void network_client :: setup(){

    if(!curl){
        std::cout<<"Failed Initialization\n";
        return;
    }
    
    headers=curl_slist_append(headers,"Accept: application/json");

    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writecallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&response);
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,follow_redirects);
    curl_easy_setopt(curl,CURLOPT_ACCEPT_ENCODING,accept_encoding.c_str());
    curl_easy_setopt(curl,CURLOPT_USERAGENT,user_agent.c_str());
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,timeout);
    curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headers);
    curl_easy_setopt(curl,CURLOPT_VERBOSE,verbose);
    curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errorbuffer);

}

bool network_client :: fetch_data(){
    std::lock_guard<std::mutex> curl_lock(curl_mtx);

    url="https://api.binance.com/api/v3/klines?symbol="+symbol+"&interval="+timeframe_array[timeframe_tracker]+"&limit="+std::to_string(limit);

    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());

    response.clear();
    
    result =curl_easy_perform(curl);

    if(result!=CURLE_OK){
        std::cerr<<"Request Failed \n";
        std::cerr<<"libcurl Error : "<<curl_easy_strerror(result)<<std::endl;
        std::cerr<<"Detailed Error : "<<errorbuffer<<std::endl;

        return false;
    }
    else{
        
        return true;
    }
    
}

void network_client :: switch_timeframe(int direction){
    if(direction==1 && timeframe_tracker<timeframe_array.size()-1)
        timeframe_tracker++;
    else if(direction==-1 && timeframe_tracker>0)
        timeframe_tracker--;

}

void network_client :: add_symbol(){
    
    std::string new_symbol;

    std::cout<<"Enter Symbol : ";
    std::cin>>new_symbol;

    symbol_array.push_back(new_symbol);
}

void network_client :: switch_symbol(int direction){
    if(direction==1 && symbol_tracker<symbol_array.size()-1)
        symbol_tracker++;
    else if(direction==-1 && symbol_tracker>0)
        symbol_tracker--;

    symbol=symbol_array[symbol_tracker];
}