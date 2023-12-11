#pragma once
#include<iostream>
#include<string>
#include<map>
#include "../Utils/DateTime/dateTime.h"
#include "../Utils/constants.h"
using namespace std;

int getStatusCode(string str){
    if(str=="Scheduled") return 0;
    else if(str=="Planning") return 1;
    else if(str=="Cancelled") return 2;
    else if(str=="Delayed") return 3;
    cout<<"Status node found:"<<str<<endl;
    return -1;
}

class Schedule{
private:
    int scheduleID;
    string carrierCD;
public:
    pair<string,string> equipmentNo;    //pair<AircraftType,AircraftTailNo>
    Time departureTime;
    Time arrivalTime;
    Date startDate;
    Date endDate;
    string flightPattern;
    int flightNum;
    STATUS status;                    //{true: Scheduled , false: Planned}
    vector<int> DepartureDates;         // vector<InventoryID>

    Schedule(int _scheduleID,string _carrierCD, pair<string,string> _equipmentNo,Time _departureTime, 
        Time _arrivalTime, Date _startDate, Date _endDate, 
        string _flightPattern, int _flightNum, STATUS _status):
        scheduleID(_scheduleID), carrierCD(_carrierCD), equipmentNo(_equipmentNo),
        departureTime(_departureTime), arrivalTime(_arrivalTime), startDate(_startDate),
        endDate(_endDate), flightPattern(_flightPattern), flightNum(_flightNum), status(_status)
        {
            if(endDate<startDate){
                cout<<"Schedule Date Mismatch"<<endl;
            }
            if(arrivalTime<departureTime){
                cout<<"Schedule timings mismatch"<<endl;
            }
            if(carrierCD.length()!=CARRIER_CODE_LENGTH){
                cout<<"Carrier Code length differs:"<<carrierCD<<endl;
            }
        }
};

map<int,Schedule*> scheduleMap;














