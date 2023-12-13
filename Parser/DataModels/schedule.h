#pragma once
#include<iostream>
#include<string>
#include<map>
#include "../Utils/DateTime/dateTime.h"
#include "../Utils/constants.h"
using namespace std;

class Schedule{
private:
    int scheduleID;
public:
    string carrierCD;
    pair<string,string> equipmentNo;    //pair<AircraftType,AircraftTailNo>
    Time departureTime;
    Time arrivalTime;
    Date startDate;
    Date endDate;
    string flightPattern;
    int flightNum;
    string srcCity;
    string destCity;
    SCHEDULE_STATUS status;
    vector<int> DepartureDates;         // vector<InventoryID>

    Schedule(int _scheduleID,string _carrierCD, pair<string,string> _equipmentNo,Time _departureTime, 
        Time _arrivalTime, Date _startDate, Date _endDate, 
        string _flightPattern, int _flightNum,string _srcCity, string _destCity ,string _status):
        scheduleID(_scheduleID), carrierCD(_carrierCD), equipmentNo(_equipmentNo),
        departureTime(_departureTime), arrivalTime(_arrivalTime), startDate(_startDate),
        endDate(_endDate), flightPattern(_flightPattern), flightNum(_flightNum),srcCity(_srcCity),destCity(_destCity) ,status(getScheduleStatus(_status))
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














