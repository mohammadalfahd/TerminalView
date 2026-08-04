#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

#include "application/application.h"


using namespace std;

int main()
{
    map<string, vector<candle>> Stock;
    application app;
    app.run();
    
    app.state.CONFIG.update_grid_config(app.state.viewport);
    Stock.emplace(app.state.httpclient.symbol, app.state.datapoint);

    return 0;
}