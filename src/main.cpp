#include <iostream>
#include <vector>
using namespace std;


class candle{
    public:
    int timestamp;
    int price;
};

void render(vector<candle> &data){
    
    for (int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(i==9){
                cout<<"=";
            }
            else if (j==0){
                cout<<"|";
            } 
            
            else {
                bool value=false;
                for(int n=data.size()-1;n>=0;n--){
                    if(i==9-data[n].price && j==data[n].timestamp){
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
}


int main(){

    vector<candle> data;
    data.push_back({1,1});
    data.push_back({2,2});
    data.push_back({3,3});
    data.push_back({4,4});
    data.push_back({5,5});

    render(data);
    return 0;
}
