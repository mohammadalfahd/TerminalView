#pragma once 

#include <iostream>
#include "app_state.h"
#include "chart/renderer.h"
#include <thread>

class application{
    public:
    void run();

    
    app_state state;
    Renderer renderer;

    std::thread polling;
    std::thread render;

    void menu();
    void start_realtime();
    void render_loop();
    void polling_loop();
    void handle_input(char key);
    void load_timeframe(int direction);
    void load_symbol(int direction);
    void initialize_indicators();
    void update_indicators();
    void refresh_indicators();
};