#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

class candle{
    public:
    int timestamp;
    int open_price;
    int high_price;
    int low_price;
    int closing_price;
};

int get_y_lable_width(vector<candle> &data);
int get_highest_price(vector<candle> &data);
int get_lowest_price(vector<candle> &data);


class GridConfig{
    public: 
    int y_label_width;
    int x_label_height=3;
    int spacing=2;
    int candle_count;
    int chart_height;
    int chart_width;


    int total_width;
    int total_height;



    GridConfig(){}
    
    GridConfig(vector<candle> &data){
        y_label_width=get_y_lable_width(data);
        chart_height=20;
        candle_count=data.size();
        chart_width=spacing*candle_count;
        total_width=chart_width+y_label_width+3;
        total_height=chart_height+x_label_height+1;

    }
    
};

vector<candle> load_csv(string file_address){
    
    vector<candle> vector_data;
    vector<int> store_data;
    string data;
    ifstream file(file_address);
    file>>data;

    string line;
    getline(file,line);

    while(getline(file,line)){
        stringstream file_line(line);
        string parameter;

        store_data.clear();

        while(getline(file_line,parameter,',')){

            store_data.push_back(stoi(parameter));
        };
        
        vector_data.push_back({store_data[0],store_data[1],store_data[2],store_data[3],store_data[4]});
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

void y_draw_labels(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config){
    int highest = get_highest_price(data);
    int lowest  = get_lowest_price(data);

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

void x_draw_label(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config){

    int label_count = 5;

    for (int i = 0; i < label_count; i++)
    {
        int candle_index =
            i * (data.size() - 1) / (label_count - 1);

        int x = 1 + candle_index * config.spacing;

        grid[config.total_height-2][x] =to_string(data[candle_index].timestamp);
            
    }
}

int get_highest_price(vector<candle> &data){
    
    int highest_price=0;

    for(int i=0;i<data.size();i++){
        if(data[i].high_price>highest_price){
            highest_price=data[i].high_price;
        }
    }

    return highest_price;
}


int get_lowest_price(vector<candle> &data){
    
    int lowest_price=0;

    for(int i=0;i<data.size();i++){
        if(data[i].low_price<lowest_price){
            lowest_price=data[i].low_price;
        }
    }

    return lowest_price;
}

int get_y_lable_width(vector<candle> &data){
    string max_width_string=to_string(get_highest_price(data));
    int y_label_width=size(max_width_string);

    return y_label_width;
    
}



int scale(vector<candle> &data,GridConfig &config,int price,int highest_price,int lowest_price){

    int price_gap=highest_price-lowest_price;

    if(price_gap==0){
        return config.total_height/2;
    }
    else{
    int scaled_price=(price-lowest_price)*(config.total_height-1)/price_gap;

    return config.total_height - 1 - scaled_price;
    } 
}

void draw_candle(vector<vector<string>> &grid,vector<candle> &data,GridConfig &config){

    const int candle_spacing=config.spacing;
    int highest_price=get_highest_price(data);
    int lowest_price=get_lowest_price(data);

    for(int i=0;i<data.size();i++){

        int high_y=scale(data,config,data[i].high_price,highest_price,lowest_price);
        int low_y=scale(data,config,data[i].low_price,highest_price,lowest_price);
        int open_y=scale(data,config,data[i].open_price,highest_price,lowest_price);
        int close_y=scale(data,config,data[i].closing_price,highest_price,lowest_price);

        int x=1+i*candle_spacing;

        int body_top=max(open_y,close_y);
        int body_bottom=min(open_y,close_y);
        
        

        
        
        for(int j=low_y+1;j>=body_bottom+1;j--){
            grid[j][x]="|";
        }
        for(int j=body_top+1;j>=high_y+1;j--){
            grid[j][x]="|";
        }
        for(int j=body_bottom+1;j<=body_top+1;j++){
            if(open_y>close_y){
                grid[j][x]="\033[31m█\033[0m";
            }
            else {grid[j][x]="\033[32m█\033[0m";}
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

void render(vector<candle> &data,GridConfig &config){
    auto grid=draw_grid(config);

    draw_axes(grid,config);
    y_draw_labels(grid,data,config);
    x_draw_label(grid,data,config);

    draw_candle(grid,data,config);

    print_grid(grid);

}

int main(){

    cout<<"Name of Comapny : ";
    string name;
    cin>>name;
    
    map<string,vector<candle>> Stock;
    vector<candle> datapoint;

    while(true){
        cout<<endl<<"1 -> Manual Data Entry"<<endl<<
            "2 -> CSV Import "<<endl<<"Enter anything else to see graph "<<endl;
        string choice;
        cout<<"Enter choice : ";
        cin>>choice;

        if(choice=="1"){
            int timestamp;
            int open_price;
            int high_price;
            int low_price;
            int closing_price;

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

            datapoint.push_back({timestamp,open_price,high_price,low_price,closing_price});

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
    GridConfig CONFIG(datapoint);

    Stock.emplace(name,datapoint);

    auto grid=draw_grid(CONFIG);

    cout<<"============= "<<name<<" ================"<<endl;
    render(datapoint,CONFIG);

    return 0;
}
