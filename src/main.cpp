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

int get_ylimit(vector<candle> &data){
    int y_limit=0;

    for(int i=0;i<data.size();i++){
        if(data[i].high_price>=y_limit){
            y_limit=data[i].high_price;
        }
    }

    return y_limit;
}
pair<int,int> grid_limit(vector<candle> &data){
    int y_limit=0;
    int x_limit=data.size()+1;

    for(int i=0;i<data.size();i++){
        if(data[i].high_price>=y_limit){
            y_limit=data[i].high_price;
        }
    }

    return {y_limit,x_limit};
}

vector<vector<char>> draw_grid(vector<candle> &data){
    
    pair<int,int> limits=grid_limit(data);
    vector<vector<char>> grid(limits.first,vector<char>(limits.second,' '));
    
    return grid;
}

void draw_axes(vector<vector<char>> &grid){

    for(int i=0;i<grid.size();i++){

        grid[i][0]='|';
    }

    int bottom=grid.size()-1;

    for(int i=0;i<grid[bottom].size();i++){

        grid[bottom][i]='=';
    }
}

void draw_candle(vector<vector<char>> &grid,vector<candle> &data){

    int inversion_factor=get_ylimit(data);
    
    for(int i=0;i<data.size();i++){
        
        int x=i+1;

        int bottom=min(data[i].open_price,data[i].closing_price);
        int top=max(data[i].open_price,data[i].closing_price);

        int wick_top=data[i].high_price;
        int wick_bottom=data[i].low_price;

        for(int j=bottom;j<top;j++){

            grid[j][x]='H';
        }

        for(int j=wick_bottom;j<bottom;j++){
            grid[(top-bottom)+j+1][x]='|';
        }
    }
}
void print_grid(vector<vector<char>> &grid){

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
