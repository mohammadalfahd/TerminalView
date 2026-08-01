#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

#include "application/app_state.h"
#include "chart/renderer.h"

using namespace std;

int main()
{

    cout << "Name of Comapny : ";
    string name;
    cin >> name;

    map<string, vector<candle>> Stock;
    app_state appstate;
    std::thread livepolling;
    std::thread render;
    Renderer renderer;

    while (appstate.running)
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

            appstate.datapoint.push_back({timestamp, open_price, high_price, low_price, closing_price, Adj_close, volume});
            appstate.viewport.first_visible_candle=max(0, static_cast<int>(appstate.datapoint.size()) - appstate.viewport.candle_count);

                appstate.sma20.initialise(appstate.datapoint);
                appstate.ema20.initialise(appstate.datapoint, appstate.sma20);
                appstate.macd_line.initialise(appstate.datapoint);
                appstate.rsi14.initialise(appstate.datapoint);
        }
        else if (choice == "2")
        {

            string file_address;
            string file_name;

            cout << "ENTER FILE NAME : ";
            cin >> file_name;

            file_address = "../CSV_files/" + file_name;

            appstate.datapoint = load_csv(file_address);
            appstate.viewport.first_visible_candle=max(0, static_cast<int>(appstate.datapoint.size()) - appstate.viewport.candle_count);
            appstate.viewport.selected_candle =min(appstate.viewport.selected_candle,static_cast<int>(appstate.datapoint.size()) - appstate.viewport.first_visible_candle - 1);
                
                appstate.sma20.initialise(appstate.datapoint);
                appstate.ema20.initialise(appstate.datapoint, appstate.sma20);
                appstate.macd_line.initialise(appstate.datapoint);
                appstate.rsi14.initialise(appstate.datapoint);
        }
        else if (choice == "3")
        {

            lock_guard<mutex> lock(appstate.mtx);

            cout << "Enter Symbol : ";
            cin >> appstate.httpclient.symbol;

            appstate.httpclient.limit=300;
            appstate.httpclient.setup();

            bool data_fetch = appstate.httpclient.fetch_data();

            for (int i = 0; i < 1; i++)
            {
                if (appstate.datapoint.empty())
                {
                    if (data_fetch)
                    {
                        bool json_parse = appstate.json.parse_json(appstate.httpclient.response);

                        if (json_parse)
                        {
                            appstate.json.set_data(appstate.datapoint);
                            
                            appstate.viewport.first_visible_candle=max(0, static_cast<int>(appstate.datapoint.size()) - appstate.viewport.candle_count);
                            appstate.viewport.selected_candle =min(appstate.viewport.selected_candle,static_cast<int>(appstate.datapoint.size()) - appstate.viewport.first_visible_candle - 1);
                            appstate.sma20.initialise(appstate.datapoint);
                            appstate.ema20.initialise(appstate.datapoint, appstate.sma20);
                            appstate.macd_line.initialise(appstate.datapoint);
                            appstate.rsi14.initialise(appstate.datapoint);

                            appstate.polling=true;
                        }
                    }
                    else
                    {
                        cerr << "DATA FETCH FAILED\n";
                    }
                }
                if (!appstate.datapoint.empty() && appstate.polling)
                {   
                    livepolling = thread([&]()
                                         {
                    cout<<"\nappstate.Polling started\n";
                    appstate.httpclient.limit=1;
                    
                    while(appstate.running){

                        if (!appstate.polling) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(50));
                            continue;
                        }
                        bool data_fetch=appstate.httpclient.fetch_data();
                        bool json_parse=appstate.json.parse_json(appstate.httpclient.response);
                       
                        if(json_parse && data_fetch ){
                            lock_guard<mutex> lock(appstate.mtx);
                            
                            
                            
                            if(!appstate.json.set_data(appstate.datapoint)){  
                                appstate.viewport.first_visible_candle=max(0, static_cast<int>(appstate.datapoint.size()) - appstate.viewport.candle_count);  
                                appstate.sma20.update(appstate.datapoint.back());
                                appstate.ema20.update(appstate.datapoint.back());
                                appstate.macd_line.update(appstate.datapoint.back());
                                appstate.rsi14.update(appstate.datapoint.back());
                            }
                            else{
                                appstate.sma20.refresh(appstate.datapoint);
                                appstate.ema20.refresh(appstate.datapoint.back());
                                appstate.rsi14.refresh(appstate.datapoint.back());
                                appstate.macd_line.refresh(appstate.datapoint.back());
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
    GridConfig CONFIG(appstate.viewport);
    CONFIG.update_grid_config(appstate.viewport);

    Stock.emplace(name, appstate.datapoint);



    render = thread([&]()
                    {
        
        enable_raw_mode();
        
        while(appstate.running){

                {   
                lock_guard<mutex> lock(appstate.mtx);
                appstate.viewport.update_layout();
                appstate.viewport.selected_candle =min(appstate.viewport.selected_candle,static_cast<int>(appstate.datapoint.size()) - appstate.viewport.first_visible_candle - 1);
                CONFIG.update_grid_config(appstate.viewport);
                system("clear");
                if(appstate.datapoint.empty()){cout<<"Fetching Data"<<endl; continue;}
                cout<<"============= "<<name<<" ================"<<endl;
                renderer.render(appstate.datapoint,CONFIG,appstate.viewport,appstate.toggles,appstate.sma20,appstate.ema20,appstate.macd_line,appstate.rsi14);
                }
                
                char key=get_key();
                
                if(key==' ')
                    continue;
                if(key=='a'){
                    appstate.viewport.pan(appstate.datapoint,-1);
                }
                if(key=='d'){
                    appstate.viewport.pan(appstate.datapoint,1);
                }
                if(key=='l'){
                    appstate.viewport.select_candle(1);
                }
                if(key=='j'){
                    appstate.viewport.select_candle(-1);
                }
                if(key=='s'){
                    appstate.toggles.sma=!appstate.toggles.sma;
                }
                if(key=='e'){
                    appstate.toggles.ema=!appstate.toggles.ema;
                }
                if(key=='v'){
                    appstate.toggles.volume=!appstate.toggles.volume;
                }
                if(key=='m'){
                    appstate.toggles.macd=!appstate.toggles.macd;
                }
                if(key=='r'){
                    appstate.toggles.rsi=!appstate.toggles.rsi;
                }
                if(key=='i'){
                    std::lock_guard<std::mutex> lock(appstate.mtx);
                    appstate.polling =false;
                    appstate.datapoint.clear();

                    appstate.httpclient.switch_timeframe(1);

                    appstate.httpclient.limit = 200;
                    appstate.httpclient.fetch_data();
                    appstate.json.parse_json(appstate.httpclient.response);
                    appstate.json.set_data(appstate.datapoint);

                    appstate.viewport.first_visible_candle =std::max(0, (int)appstate.datapoint.size() - appstate.viewport.candle_count);
                    appstate.viewport.selected_candle =std::max(0, appstate.viewport.candle_count - 1);

                    appstate.sma20.initialise(appstate.datapoint);
                    appstate.ema20.initialise(appstate.datapoint, appstate.sma20);
                    appstate.macd_line.initialise(appstate.datapoint);
                    appstate.rsi14.initialise(appstate.datapoint);

                    appstate.polling=true;
                    appstate.httpclient.limit = 1;

                }
                if(key=='k'){
                    std::lock_guard<std::mutex> lock(appstate.mtx);
                    appstate.polling =false;
                    appstate.datapoint.clear();

                    appstate.httpclient.switch_timeframe(-1);

                    appstate.httpclient.limit = 200;
                    appstate.httpclient.fetch_data();
                    appstate.json.parse_json(appstate.httpclient.response);
                    appstate.json.set_data(appstate.datapoint);

                    appstate.viewport.first_visible_candle =
                        std::max(0, (int)appstate.datapoint.size() - appstate.viewport.candle_count);
                    appstate.viewport.selected_candle =
                        std::max(0, appstate.viewport.candle_count - 1);

                    appstate.sma20.initialise(appstate.datapoint);
                    appstate.ema20.initialise(appstate.datapoint, appstate.sma20);
                    appstate.macd_line.initialise(appstate.datapoint);
                    appstate.rsi14.initialise(appstate.datapoint);

                    appstate.polling=true;
                    appstate.httpclient.limit = 1;
                }
                if(key=='q'){
                    appstate.running=false;
                    break;
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