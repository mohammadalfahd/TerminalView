#include <iostream>
#include <vector>
#include <map>
using namespace std;


class candle{
    public:
    int timestamp;
    int open_price;
    int high_price;
    int low_price;
    int closing_price;
};

const int HEIGHT=20;
const int WIDTH=20;

vector<vector<string>> draw_grid(vector<candle> &data){

    vector<vector<string>> grid(HEIGHT,vector<string>(WIDTH," "));
    
    return grid;
}

void draw_axes(vector<vector<string>> &grid){

    for(int i=0;i<HEIGHT;i++){

        grid[i][0]='|';
    }

    int bottom=HEIGHT-1;

    for(int i=0;i<WIDTH;i++){

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

int scale(vector<candle> &data,int price,int highest_price,int lowest_price){

    int price_gap=highest_price-lowest_price;

    if(price_gap==0){
        return HEIGHT/2;
    }
    else{
    int scaled_price=(price-lowest_price)*(HEIGHT-1)/price_gap;

    return HEIGHT - 1 - scaled_price;
    } 
}

void draw_candle(vector<vector<string>> &grid,vector<candle> &data){

    int highest_price=get_highest_price(data);
    int lowest_price=get_lowest_price(data);

    for(int i=0;i<data.size();i++){

        int high_y=scale(data,data[i].high_price,highest_price,lowest_price);
        int low_y=scale(data,data[i].low_price,highest_price,lowest_price);
        int open_y=scale(data,data[i].open_price,highest_price,lowest_price);
        int close_y=scale(data,data[i].closing_price,highest_price,lowest_price);

        int x=i+1;

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

void render(vector<candle> &data){
    auto grid=draw_grid(data);

    draw_axes(grid);

    draw_candle(grid,data);

    print_grid(grid);

}

int main(){

    cout<<"Name of Comapny : ";
    string name;
    cin>>name;
    
    map<string,vector<candle>> Stock;
    vector<candle> datapoint;

    while(true){
        cout<<endl<<"Enter 1 to add a Datapoint "<<endl<<"Enter anything else to see graph "<<endl;
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
        else {cout<<endl<<"Data Capture Terminated"<<endl; break;};
    }

    Stock.emplace(name,datapoint);

    auto grid=draw_grid(datapoint);

    cout<<"============="<<name<<"================"<<endl;
    render(datapoint);

    return 0;
}
