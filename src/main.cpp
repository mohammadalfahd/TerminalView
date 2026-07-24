#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>

#include "../include/candle.h"
#include "../include/viewport.h"
#include "../include/grid.h"
#include "../include/csv_loader.h"
#include "../include/input.h"
#include "../include/indicators.h"
#include "../include/renderer.h"
#include "../include/api_call.h"
#include "../include/json_parser.h"

using namespace std;

int main()
{

    cout << "Name of Comapny : ";
    string name;
    cin >> name;

    map<string, vector<candle>> Stock;
    vector<candle> datapoint;
    Viewport visible_region;
    Indicators toggle_indicators;
    sma sma20;
    ema ema20;
    macd macd;
    rsi rsi14;
    Renderer renderer;

    mutex mtx;
    thread livepolling;
    thread render;

    network_client httpclient;
    json_parser json;

    while (true)
    {
        cout << endl
             << "1 -> Manual Data Entry" << endl
             << "2 -> CSV Import " << endl
             << "3 -> RealTime Candlestick Chart " << endl
             << "Enter anything else to see graph " << endl;

        string choice;
        cout << "Enter choice : ";
        cin >> choice;

        if (choice == "1")
        {
            string timestamp;
            double open_price;
            double high_price;
            double low_price;
            double closing_price;
            double Adj_close;
            double volume;

            cout << endl
                 << "Enter Year : ";
            cin >> timestamp;
            cout << endl
                 << "Enter Open Price : ";
            cin >> open_price;
            cout << endl
                 << "Enter high price : ";
            cin >> high_price;
            cout << endl
                 << "Enter low price : ";
            cin >> low_price;
            cout << endl
                 << "Enter Closing price : ";
            cin >> closing_price;
            cout << endl
                 << "Enter Adjacent Close : ";
            cin >> Adj_close;
            cout << endl
                 << "Enter Volume : ";
            cin >> volume;

            datapoint.push_back({timestamp, open_price, high_price, low_price, closing_price, Adj_close, volume});

                sma20.initialise(datapoint);
                ema20.initialise(datapoint, sma20);
                macd.initialise(datapoint);
                rsi14.initialise(datapoint);
        }
        else if (choice == "2")
        {

            string file_address;
            string file_name;

            cout << "ENTER FILE NAME : ";
            cin >> file_name;

            file_address = "../CSV_files/" + file_name;

            datapoint = load_csv(file_address);

                sma20.initialise(datapoint);
                ema20.initialise(datapoint, sma20);
                macd.initialise(datapoint);
                rsi14.initialise(datapoint);
        }
        else if (choice == "3")
        {

            lock_guard<mutex> lock(mtx);

            cout << "Enter Symbol : ";
            cin >> httpclient.symbol;

            cout << "Enter Interval : ";
            cin >> httpclient.interval;

            httpclient.setup();

            bool data_fetch = httpclient.fetch_data();

            for (int i = 0; i < 1; i++)
            {
                if (datapoint.empty())
                {
                    if (data_fetch)
                    {
                        bool json_parse = json.parse_json(httpclient.response);

                        if (json_parse)
                        {
                            json.set_data(datapoint);
                            sma20.initialise(datapoint);
                            ema20.initialise(datapoint, sma20);
                            macd.initialise(datapoint);
                            rsi14.initialise(datapoint);
                        }
                    }
                    else
                    {
                        cerr << "DATA FETCH FAILED\n";
                    }
                }
                if (!datapoint.empty())
                {
                    livepolling = thread([&]()
                                         {
                        cout<<"Polling started\n";
                    httpclient.limit=1;
                    
                    while(true){
                        data_fetch=httpclient.fetch_data();
                        bool json_parse=json.parse_json(httpclient.response);
                       
                        if(json_parse && data_fetch ){
                            lock_guard<mutex> lock(mtx);
                            
                            
                            
                            if(!json.set_data(datapoint)){    
                                sma20.update(datapoint.back());
                                ema20.update(datapoint.back());
                                macd.update(datapoint.back());
                                rsi14.update(datapoint.back());
                            }
                            else{
                                sma20.refresh(datapoint);
                                ema20.refresh(datapoint.back());
                                rsi14.refresh(datapoint.back());
                            }
                            
                        }
                    
                        std::this_thread::sleep_for(std::chrono::seconds(1));


                    } });
                }
            }
        }

        else
        {
            cout << endl
                 << "Data Capture Terminated" << endl;
            break;
        };
    }
    GridConfig CONFIG(visible_region);

    Stock.emplace(name, datapoint);



    render = thread([&]()
                    {
        
        enable_raw_mode();
        
        while(true){

                {   
                lock_guard<mutex> lock(mtx);
                system("clear");
                cout<<"============= "<<name<<" ================"<<endl;
                renderer.render(datapoint,CONFIG,visible_region,toggle_indicators,sma20,ema20,macd,rsi14);
                }
                
                char key=get_key();
                
                if(key==' ')
                    continue;
                if(key=='a'){
                    visible_region.pan(visible_region,datapoint,-1);
                }
                if(key=='d'){
                    visible_region.pan(visible_region,datapoint,1);
                }
                if(key=='l'){
                    visible_region.select_candle(visible_region,1);
                }
                if(key=='j'){
                    visible_region.select_candle(visible_region,-1);
                }
                if(key=='s'){
                    toggle_indicators.sma=!toggle_indicators.sma;
                }
                if(key=='e'){
                    toggle_indicators.ema=!toggle_indicators.ema;
                }
                if(key=='v'){
                    toggle_indicators.volume=!toggle_indicators.volume;
                }
                if(key=='m'){
                    toggle_indicators.macd=!toggle_indicators.macd;
                }
                if(key=='r'){
                    toggle_indicators.rsi=!toggle_indicators.rsi;
                }
                if(key=='q'){
                    return 0;
                }
                this_thread::sleep_for(chrono::milliseconds(33));

        }
        
        disable_raw_mode(); });

    if (livepolling.joinable())
        livepolling.join();

    if (render.joinable())
        render.join();

    return 0;
}