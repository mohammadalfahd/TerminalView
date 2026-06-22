#include <iostream>
#include <vector>
#include <map>
using namespace std;


class candle{
    public:
    int timestamp;
    int price;
};

void render(vector<candle> &data){
    if (data.empty()){
        cout<<"No Data Available\n";
    }
    else{
    int y_max=data[0].price;

    for(int i=0;i<data.size();i++){
        if(y_max<data[i].price){
            y_max=data[i].price;
        }
    }

    for (int i=0;i<=y_max;i++){
        for(int j=0;j<y_max;j++){
            if(i==y_max-1){
                cout<<"=";
            }
            else if (j==0){
                cout<<"|";
            } 
            
            else {
                bool value=false;
                for(int n=data.size()-1;n>=0;--n){
                    if(i==y_max-data[n].price && j==data[n].timestamp){
                        value=true;
                        break;
                    }
                }

                if(value==true){
                    cout<<"*";
                }
                else {cout<<" ";};
            };

        }
    cout<<endl;
    }
};
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
            int price;

            cout<<endl<<"Enter Year : ";
            cin>>timestamp;
            cout<<endl<<"Enter price : ";
            cin>>price;

            datapoint.push_back({timestamp,price});

        }
        else {cout<<endl<<"Data Capture Terminated"<<endl; break;};
    }

    Stock.emplace(name,datapoint);

    cout<<"============="<<name<<"================"<<endl;
    render(datapoint);

    return 0;
}
