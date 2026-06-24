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

pair<int,int> grid_limit(vector<candle> &data){
    int y_limit=0;
    int x_limit=data.size();

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
