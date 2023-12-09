#include<iostream>
#include<string>
#include<map>
#include "../Utils/constants.h"
using namespace std;

class CityPair{
    public:
        string srcCity, destCity;
        CityPair(string _src, string _dest):srcCity(_src),destCity(_dest){}
        CityPair(){}
        CityPair& operator = (const CityPair& other){
            if(srcCity.empty()){
                if((other.srcCity.length()!=CITY_CODE_LENGTH)||(other.destCity.length()!=CITY_CODE_LENGTH)){
                    cout<<"Inconsistant city code length"<<endl;
                }
                srcCity = other.srcCity;
                destCity = other.destCity;
            }
            else{
                if((srcCity!=other.srcCity)||(destCity!=other.destCity)){
                    cout<<"Inconsistency in flight numbers"<<endl;
                }
            }
            return *this;
        }
};
class FlightNumberMap{
    map<int,CityPair> flightNumberMap;
    public:
        CityPair& operator [](int ind){
            if(flightNumberMap.find(ind)==flightNumberMap.end()){
                flightNumberMap[ind] = CityPair();
            }
            return flightNumberMap[ind];
        }
};

FlightNumberMap flightNumberMap;
map<CityPair,int> cityToFlightNumberMap;