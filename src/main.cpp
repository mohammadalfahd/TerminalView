#include "input.h"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

class candle{
    public:
    string timestamp;
    double open_price;
    double high_price;
    double low_price;
    double closing_price;
    double adj_close;
    double volume;
};

struct Viewport{
    int first_visible_candle=0;
    int candle_count=20;

    int selected_candle=19;
};

int get_y_lable_width(vector<candle> &data);
int get_highest_price(vector<candle> &data,Viewport &Viewport);
int get_lowest_price(vector<candle> &data,Viewport &Viewport);


class GridConfig{
    public: 
    int y_label_width=1;
    int x_label_height=3;
    int spacing=2;
    int candle_count;
    int chart_height=20;
    int chart_width;


    int total_width;
    int total_height;



    GridConfig(){}
    
    GridConfig(Viewport &visible_region){

        candle_count=visible_region.candle_count;
        chart_width=spacing*candle_count;
        total_width=chart_width+y_label_width+3;
        total_height=chart_height+x_label_height+1;

    }
    
};

vector<candle> load_csv(string file_address)
{
    vector<candle> vector_data;

    ifstream file(file_address);

    string line;
    getline(file, line); 

    while (getline(file, line))
    {
        stringstream file_line(line);

        string timestamp;
        string parameter;

        double open, high, low, close,adjclose, volume;

        getline(file_line, timestamp, ',');

        getline(file_line, parameter, ',');
        open = stod(parameter);

        getline(file_line, parameter, ',');
        high = stod(parameter);

        getline(file_line, parameter, ',');
        low = stod(parameter);

        getline(file_line, parameter, ',');
        close = stod(parameter);

        getline(file_line, parameter, ',');
        adjclose = stod(parameter);
        
        getline(file_line, parameter, ',');
        volume = stod(parameter);

        vector_data.push_back({timestamp,open,high,low,close,adjclose,volume});
    }

    return vector_data;
}


vector<vector<string>> draw_grid(GridConfig &config){

    vector<vector<string>> grid(config.total_height,vector<string>(config.total_width," "));
    
    return grid;
}

void draw_axes(vector<vector<string>> &grid,GridConfig &config){

    for(int i=0;i<config.total_height;i++){

        grid[i][0]="\033[36m|\033[0m";
        grid[i][config.total_width-1]="\033[36m|\033[0m"; 
    }
    for(int i=0;i<config.chart_height;i++){

        grid[i][config.chart_width+1]="\033[36m|\033[0m";    
    }

    int bottom=config.total_height-1;

    for(int i=0;i<config.total_width;i++){

        grid[bottom][i]="\033[36m=\033[0m";
        grid[config.chart_height+1][i]="\033[36m=\033[0m";
        grid[0][i]="\033[36m=\033[0m";
    }

    grid[bottom][0]="\033[36m+\033[0m";
}

void y_draw_labels(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport){
    int highest = get_highest_price(data,Viewport);
    int lowest  = get_lowest_price(data,Viewport);

    const int label_count = 5;

    for (int i = 0; i < label_count; i++)
    {
        int row = i * (config.chart_height - 1) / (label_count - 1);

        int price =
            highest -
            row * (highest - lowest) / (config.chart_height - 1);

        grid[row+1][config.chart_width + 2] = to_string(price);
    }
}

void x_draw_label(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport){

    int label_count = 3;

    for (int i = 0; i < label_count; i++)
    {   
        int screen_index=i*(Viewport.candle_count-1)/(label_count-1);
        int candle_index =Viewport.first_visible_candle+screen_index;

        int x = 1 + screen_index ;

        grid[config.total_height-2][x] =data[candle_index].timestamp;
            
    }
}

int get_highest_price(vector<candle> &data,Viewport &Viewport){
    
    int highest_price=0;
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle;i<end;i++){
        if(data[i].high_price>highest_price){
            highest_price=data[i].high_price;
        }
    }

    return highest_price;
}


int get_lowest_price(vector<candle> &data,Viewport &Viewport){
    
    int lowest_price = data[0].low_price;
    int end = min((Viewport.first_visible_candle + Viewport.candle_count),int(data.size()));

    for(int i=Viewport.first_visible_candle;i<end;i++){
        if(data[i].low_price<lowest_price){
            lowest_price=data[i].low_price;
        }
    }

    return lowest_price;
}


int scale(GridConfig &config,int price,int highest_price,int lowest_price){

    int price_gap=highest_price-lowest_price;

    if(price_gap==0){
        return config.chart_height/2;
    }
    else{
    int scaled_price=(price-lowest_price)*(config.chart_height-1)/price_gap;

    return config.chart_height - 1 - scaled_price;
    } 
}

void draw_candle(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport){

    const int candle_spacing=config.spacing;
    int highest_price=get_highest_price(data,Viewport);
    int lowest_price=get_lowest_price(data,Viewport);

    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());

    int visible = end - Viewport.first_visible_candle;

    for(int screen_index = 0; screen_index < visible; screen_index++){
    int data_index = Viewport.first_visible_candle + screen_index;
        
        int high_y=scale(config,data[data_index].high_price,highest_price,lowest_price);
        int low_y=scale(config,data[data_index].low_price,highest_price,lowest_price);
        int open_y=scale(config,data[data_index].open_price,highest_price,lowest_price);
        int close_y=scale(config,data[data_index].closing_price,highest_price,lowest_price);

        bool selected=(screen_index==Viewport.selected_candle);

        int x = 1 + screen_index * config.spacing;

        int body_top=max(open_y,close_y);
        int body_bottom=min(open_y,close_y);
        
        
        for(int j=low_y;j>=body_bottom;j--){

            grid[j][x]="|";
        }
        for(int j=body_top+1;j>=high_y;j--){

            grid[j][x]="|";
        }
        for(int j=body_bottom;j<=body_top;j++){

            if(open_y>close_y){
                grid[j][x]="\033[32m█\033[0m";
            }
            else {grid[j][x]="\033[31m█\033[0m";}
            if(selected){
                grid[j][x]="\033[33m█\033[0m";
            }
        }
    }
}
double calculate_moving_average(vector<candle> &data,int index,int period = 20){

    
    double sum=0;
    double sma=0;
    for(int i=index;i>index-period;i--){
        if(i<0){
            continue;
        }
        sum+=data[i].closing_price;
    }
    sma =sum/period;

    return sma;
}
void draw_moving_average(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config,Viewport &Viewport,int period=20){
    int highest_price = get_highest_price(data, Viewport);
    int lowest_price = get_lowest_price(data, Viewport);

    bool first_point = true;
    

    int prev_x;
    int prev_y;
    

    int end = min(Viewport.first_visible_candle + Viewport.candle_count,(int)data.size());
    int visible = end - Viewport.first_visible_candle;

    for (int screen_index = 0; screen_index < visible; screen_index++)
    {   
        int data_index=screen_index+Viewport.first_visible_candle;
        double sma=calculate_moving_average(data,data_index,period);
        int y = scale(config, sma, highest_price, lowest_price);
        int x = 1 + screen_index * config.spacing;
        if (y < 0 || y >= config.chart_height)
            continue;
        if (first_point)
        {   
            grid[y][x] = "\033[95m*\033[0m";

            prev_x = x;
            prev_y = y;
            first_point = false;
        }
        else
        {
            grid[y][x] = "\033[95m*\033[0m";

            prev_x = x;
            prev_y = y;
        }
    }
}

void print_grid(vector<vector<string>> &grid){

    for(int i=0;i<grid.size();i++){
        
        for(int j=0;j<grid[i].size();j++){
            cout<<grid[i][j];
        }

        cout<<endl;
    }
}

void status_bar(Viewport &Viewport,vector<candle> &data){
    int data_index=Viewport.first_visible_candle+Viewport.selected_candle;

    cout<<"\033[36m*-------------------------------------------------------------*"<<endl<<endl;
    cout<<"📅 Date : "<<data[data_index].timestamp<<endl;
    cout<<"\033[32m🟢 O : "<<data[data_index].open_price<<"   🔺 H : "<<data[data_index].high_price<<endl;
    cout<<"\033[91m🔻 L : "<<data[data_index].low_price<<"    🔴 C : "<<data[data_index].closing_price<<endl<<endl;
    cout<<"\033[38;5;202m📦 Volume : "<<data[data_index].volume<<endl;
    cout<<"\033[35m📈 SMA20 : "<<  calculate_moving_average(data,data_index,20) <<endl;
    cout<<"\033[36m*-------------------------------------------------------------*\033[0m"<<endl;

}

void render(vector<candle> &data,GridConfig &config,Viewport &Viewport){
    auto grid=draw_grid(config);

    draw_axes(grid,config);
    y_draw_labels(grid,data,config,Viewport);
    x_draw_label(grid,data,config,Viewport);

    draw_candle(grid,data,config,Viewport);
    draw_moving_average(grid,data,config,Viewport,20);

    print_grid(grid);

    status_bar(Viewport,data);

}

void pan(Viewport &Viewport,vector<candle> &data,int direction){
    int last_start = max(0, (int(data.size()) - Viewport.candle_count));

    if(direction==-1){
        if(Viewport.first_visible_candle!=0){
            Viewport.first_visible_candle--;
        }
    }
    else if (direction==1){
        if (Viewport.first_visible_candle + Viewport.candle_count < data.size())
        {
            Viewport.first_visible_candle++;
        }
    }
}

void select_candle(Viewport &viewport,int direction){
    if(direction==1 && viewport.selected_candle<19){
        viewport.selected_candle++;
    }
    else if(direction==-1 && viewport.selected_candle>0){
        viewport.selected_candle--;
    }
    
}


int main(){

    cout<<"Name of Comapny : ";
    string name;
    cin>>name;
    
    map<string,vector<candle>> Stock;
    vector<candle> datapoint;
    Viewport visible_region;

    while(true){
        cout<<endl<<"1 -> Manual Data Entry"<<endl<<
            "2 -> CSV Import "<<endl<<"Enter anything else to see graph "<<endl;
        string choice;
        cout<<"Enter choice : ";
        cin>>choice;

        if(choice=="1"){
            string timestamp;
            double open_price;
            double high_price;
            double low_price;
            double closing_price;
            double Adj_close;
            double volume;

            cout<<endl<<"Enter Year : ";
            cin>>timestamp;
            cout<<endl<<"Enter Open Price : ";
            cin>>open_price;
            cout<<endl<<"Enter high price : ";
            cin>>high_price;
            cout<<endl<<"Enter low price : ";
            cin>>low_price;
            cout<<endl<<"Enter Closing price : ";
            cin>>closing_price;
            cout<<endl<<"Enter Adjacent Close : ";
            cin>>Adj_close;
            cout<<endl<<"Enter Volume : ";
            cin>>volume;

            datapoint.push_back({timestamp,open_price,high_price,low_price,closing_price,Adj_close,volume});

        }
        else if(choice=="2"){

            string file_address;
            string file_name;

            cout<<"ENTER FILE NAME : ";
            cin>>file_name;

            file_address="../CSV_files/"+file_name;

            datapoint=load_csv(file_address);
        }
        else {cout<<endl<<"Data Capture Terminated"<<endl; break;};
    }
    GridConfig CONFIG(visible_region);

    Stock.emplace(name,datapoint);

    auto grid=draw_grid(CONFIG);

    enable_raw_mode();

    while(true){

            
            system("clear");
            cout<<"============= "<<name<<" ================"<<endl;
            render(datapoint,CONFIG,visible_region);

            char key=get_key();

            if(key=='a'){
                pan(visible_region,datapoint,-1);
            }
            if(key=='d'){
                pan(visible_region,datapoint,1);
            }
            if(key=='l'){
                select_candle(visible_region,1);
            }
            if(key=='j'){
                select_candle(visible_region,-1);
            }
            if(key=='q'){
                break;
            }
            
            
    }
    disable_raw_mode();

    return 0;
}