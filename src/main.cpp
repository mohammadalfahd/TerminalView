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

void render(vector<candle> &data){
    if (data.empty()){
        cout<<"No Data Available\n";
    }
    else{
    int y_max=data[0].high_price;

    for(int i=0;i<data.size();i++){
        if(y_max<data[i].high_price){
            y_max=data[i].high_price;
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
                bool bullish=false;
                bool consolidation=false;
                int target_candle;
                for(int n=data.size()-1;n>=0;--n){
                    if(i==y_max-data[n].closing_price && j==data[n].timestamp){
                        value=true;
                        target_candle=n;
                        if(data[n].closing_price>data[n].open_price){
                            bullish=true;
                        }
                        if(data[n].closing_price==data[n].open_price){
                            consolidation=true;
                        }
                        break;
                    }
                }

                if(value==true && bullish==true){
                    for(int i=0;i<(data[target_candle].closing_price-data[target_candle].open_price);i++){
                        cout<<"█";
                    }
                }
                else if (value==true && bullish ==false){
                    for(int i=0;i<(data[target_candle].open_price-data[target_candle].closing_price);i++){
                        cout<<"░";
                    }
                }
                else if(value==true && consolidation==true){
                    cout<<"=";
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

    cout<<"============="<<name<<"================"<<endl;
    render(datapoint);

    return 0;
}
