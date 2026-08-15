#include "application/application.h"

void application:: initialize_indicators(){
    state.sma20.initialise(state.datapoint);
    state.ema20.initialise(state.datapoint,state.sma20);
    state.rsi14.initialise(state.datapoint);
    state.macd_line.initialise(state.datapoint);
}

void application:: update_indicators(){
    state.sma20.update(state.datapoint.back());
    state.ema20.update(state.datapoint.back());
    state.macd_line.update(state.datapoint.back());
    state.rsi14.update(state.datapoint.back());
}

void application:: refresh_indicators(){
    state.sma20.refresh(state.datapoint);
    state.ema20.refresh(state.datapoint.back());
    state.rsi14.refresh(state.datapoint.back());
    state.macd_line.refresh(state.datapoint.back());
}

void application:: load_timeframe(int direction){
    state.polling =false;
    state.datapoint.clear();
    state.httpclient.switch_timeframe(direction);
    state.httpclient.limit = 200;
    state.httpclient.fetch_data();
    state.json.parse_json(state.httpclient.response);
    state.json.set_data(state.datapoint);
    state.viewport.update_layout();
    state.viewport.first_visible_candle =std::max(0, (int)state.datapoint.size() - state.viewport.candle_count);
    state.viewport.selected_candle =std::max(0, state.viewport.candle_count - 1);
    initialize_indicators();
    state.polling=true;
    state.httpclient.limit = 1;
}
void application :: load_symbol(int direction){
    state.polling =false;
    state.datapoint.clear();
    state.httpclient.switch_symbol(direction);
    state.httpclient.limit = 200;
    state.httpclient.fetch_data();
    state.json.parse_json(state.httpclient.response);
    state.json.set_data(state.datapoint);
    state.viewport.update_layout();
    state.viewport.first_visible_candle =std::max(0, (int)state.datapoint.size() - state.viewport.candle_count);
    state.viewport.selected_candle =std::max(0, state.viewport.candle_count - 1);
    initialize_indicators();
    state.polling=true;
    state.httpclient.limit = 1;
}
void application :: handle_input(char key){

    if(key=='a'){
        state.viewport.pan(state.datapoint,-1);
    }
    if(key=='d'){
        state.viewport.pan(state.datapoint,1);
    }
    if(key=='l'){
        state.viewport.select_candle(1);
    }
    if(key=='j'){
        state.viewport.select_candle(-1);
    }
    if(key=='s'){
        state.toggles.sma=!state.toggles.sma;
    }
    if(key=='e'){
        state.toggles.ema=!state.toggles.ema;
    }
    if(key=='v'){
        state.toggles.volume=!state.toggles.volume;
    }
    if(key=='m'){
        state.toggles.macd=!state.toggles.macd;
    }
    if(key=='r'){
        state.toggles.rsi=!state.toggles.rsi;
    }
    if(key=='i'){
        std::lock_guard<std::mutex> lock(state.mtx);
        load_timeframe(1);
    }
    if(key=='k'){
        std::lock_guard<std::mutex> lock(state.mtx);
        load_timeframe(-1);
    }
    if(key=='x'){
        std::lock_guard<std::mutex> lock(state.mtx);
        disable_raw_mode();
        state.httpclient.add_symbol();
        enable_raw_mode();        
    }
    if(key=='n'){
        std::lock_guard<std::mutex> lock(state.mtx);
        load_symbol(1);
    }
    if(key=='b'){
        std::lock_guard<std::mutex> lock(state.mtx);
        load_symbol(-1);
    }
    if(key=='q'){
        state.running=false;
    }

}

void application:: polling_loop(){
    state.httpclient.limit=1;
                    
    while(state.running){
        if (!state.polling) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        bool data_fetch=state.httpclient.fetch_data();
        bool json_parse=state.json.parse_json(state.httpclient.response);
       
        if(json_parse && data_fetch ){
            lock_guard<mutex> lock(state.mtx);
            
            
            
            if(!state.json.set_data(state.datapoint)){  
                state.viewport.first_visible_candle=max(0, static_cast<int>(state.datapoint.size()) - state.viewport.candle_count);  
                update_indicators();
            }
            else{
                refresh_indicators();
            }
            
        }
    
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void application :: render_loop(){
    enable_raw_mode();
        
    while(state.running){
            {   
            lock_guard<mutex> lock(state.mtx);
            state.viewport.update_layout();
            state.viewport.selected_candle =min(state.viewport.selected_candle,static_cast<int>(state.datapoint.size()) - state.viewport.first_visible_candle - 1);
            state.CONFIG.update_grid_config(state.viewport);
            system("clear");
            if(state.datapoint.empty()){cout<<"Fetching Data"<<endl; continue;}
            cout<<state.httpclient.symbol<<"            TimeFrame : "<<state.httpclient.timeframe_array[state.httpclient.timeframe_tracker]<<endl;
            renderer.render(state.datapoint,state.CONFIG,state.viewport,state.toggles,state.sma20,state.ema20,state.macd_line,state.rsi14);
            }
            
            char key=get_key();
            {
            lock_guard<mutex> lock(state.input_mtx);
            handle_input(key);
            }
            this_thread::sleep_for(chrono::milliseconds(33));
    }
    disable_raw_mode();
}

void application:: start_realtime(){
    lock_guard<mutex> lock(state.mtx);

    cout << "Enter Symbol : ";
    cin >> state.httpclient.symbol;
    state.httpclient.limit=300;
    state.httpclient.setup();
    bool data_fetch = state.httpclient.fetch_data();

    if (state.datapoint.empty())
    {
        if (data_fetch)
        {
            bool json_parse = state.json.parse_json(state.httpclient.response);
            if (json_parse)
            {
                state.json.set_data(state.datapoint);
                state.viewport.update_layout();
                state.viewport.first_visible_candle=max(0, static_cast<int>(state.datapoint.size()) - state.viewport.candle_count);
                state.viewport.selected_candle =min(state.viewport.selected_candle,static_cast<int>(state.datapoint.size()) - state.viewport.first_visible_candle - 1);
                initialize_indicators();
                state.polling=true;
            }
        }
        else
        {
            cerr << "DATA FETCH FAILED\n";
        }
    }
    if (!state.datapoint.empty() && state.polling)
    {   polling=std::thread(&application::polling_loop,this);
        
    }
    
}

void application :: menu(){
    

    while (state.running)
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

            state.datapoint.push_back({timestamp, open_price, high_price, low_price, closing_price, Adj_close, volume});
            state.viewport.first_visible_candle=max(0, static_cast<int>(state.datapoint.size()) - state.viewport.candle_count);

            initialize_indicators();
        }
        else if (choice == "2")
        {

            string file_address;
            string file_name;

            cout << "ENTER FILE NAME : ";
            cin >> file_name;

            file_address = "../CSV_files/" + file_name;

            state.datapoint = load_csv(file_address);
            state.viewport.first_visible_candle=max(0, static_cast<int>(state.datapoint.size()) - state.viewport.candle_count);
            state.viewport.selected_candle =min(state.viewport.selected_candle,static_cast<int>(state.datapoint.size()) - state.viewport.first_visible_candle - 1);
                
            initialize_indicators();
        }
        else if (choice == "3")
        {   
            
            start_realtime();
            break;
        }

        else
        {
            cout << endl
                 << "Data Capture Terminated" << endl;
            break;
        };
    }
}

void application::run()
{
    menu();

    state.CONFIG.update_grid_config(state.viewport);

    render = std::thread(&application::render_loop, this);

    if (polling.joinable())
        polling.join();

    if (render.joinable())
        render.join();
}

