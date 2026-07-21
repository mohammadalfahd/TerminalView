#include "../include/api_call.h"

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
    
    url="https://api.binance.com/api/v3/klines?symbol="+symbol+"&interval="+interval+"&limit="+std::to_string(limit);

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

int network_client :: get_interval_seconds(){
    int value = std::stoi(interval.substr(0, interval.size() - 1));
    char unit = interval.back();

    switch(unit)
    {
        case 'm': return value * 60;
        case 'h': return value * 60 * 60;
        case 'd': return value * 60 * 60 * 24;
        case 'w': return value * 60 * 60 * 24 * 7;
        default: return 60;
    }
}