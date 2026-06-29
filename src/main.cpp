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
struct GridConfig{
    int width;
    int height;
    int spacing;
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


vector<vector<string>> draw_grid(vector<candle> &data,GridConfig &config){

    vector<vector<string>> grid(config.height,vector<string>(config.width," "));
    
    return grid;
}

void draw_axes(vector<vector<string>> &grid,GridConfig &config){

    for(int i=0;i<config.height;i++){

        grid[i][0]='|';
    }

    int bottom=config.height-1;

    for(int i=0;i<config.width;i++){

        grid[bottom][i]='=';
    }

    grid[bottom][0]="+";
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

int scale(vector<candle> &data,GridConfig &config,int price,int highest_price,int lowest_price){

    int price_gap=highest_price-lowest_price;

    if(price_gap==0){
        return config.height/2;
    }
    else{
    int scaled_price=(price-lowest_price)*(config.height-1)/price_gap;

    return config.height - 1 - scaled_price;
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
        
        

        
        
        for(int j=low_y;j>=body_bottom;j--){
            grid[j][x]="|";
        }
        for(int j=body_top;j>=high_y;j--){
            grid[j][x]="|";
        }
        for(int j=body_bottom;j<body_top+1;j++){
            grid[j][x]="█";
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
    auto grid=draw_grid(data,config);

    draw_axes(grid,config);

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
    GridConfig CONFIG;
    CONFIG.height=20;
    CONFIG.spacing=2;
    CONFIG.width=datapoint.size()*CONFIG.spacing+2;

    Stock.emplace(name,datapoint);

    auto grid=draw_grid(datapoint,CONFIG);

    cout<<"============="<<name<<"================"<<endl;
    render(datapoint,CONFIG);

    return 0;
}
