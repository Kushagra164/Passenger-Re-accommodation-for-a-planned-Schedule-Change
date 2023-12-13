#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include "../DataModels/schedule.h"
#include "../DataModels/flight.h"
#include "../Utils/constants.h"
#include "../Utils/DateTime/dateTime.h"
#include "../Utils/uuidGenerator.h"
using namespace std;
void getScheduleInput(ifstream& scheduleFile){
    string line;
    getline(scheduleFile, line);

    while (getline(scheduleFile, line)) {

        stringstream inputString(line);
        //ScheduleID,CarrierCD,FlightNumber, AircraftType, AircraftTailNumber,DepartureTime,ArrivalTime,StartDate,EndDate,FlightPattern,Status,DepartureDates
        string scheduleID;
        string carrierCD;
        int flightNum;

        pair<string,string> equipmentNo;

        string srcCity;
        string destCity;

        Time departureTime;
        Time arrivalTime;
        Date startDate;
        Date endDate;

        string status;

        string flightPattern;
        string tempString;

        getline(inputString, scheduleID, ',');
        int uuid=scheduleUuidGenerator.getID(scheduleID);


        getline(inputString,carrierCD, ',');

        getline(inputString,tempString, ',');
        flightNum = atoi(tempString.c_str());

        getline(inputString, equipmentNo.first, ',');
        getline(inputString, equipmentNo.second, ',');

        getline(inputString, tempString , ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        srcCity=tempString;

        getline(inputString, tempString, ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        destCity=tempString;
 
        flightNumberMap[flightNum].push_back(CityPair(srcCity,destCity));


        getline(inputString, tempString, ',');
        departureTime=Time(tempString);

        getline(inputString, tempString, ',');
        arrivalTime=Time(tempString);

        getline(inputString, tempString, ',');
        startDate=Date(tempString);

        getline(inputString, tempString, ',');
        endDate=Date(tempString);

        getline(inputString, status, ',');

        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, flightPattern, ',');


        Schedule* S = new Schedule(uuid,carrierCD,equipmentNo,departureTime,arrivalTime,
                                   startDate,endDate,flightPattern,flightNum,srcCity,destCity,status);

        scheduleMap[uuid]=S;
    }
}