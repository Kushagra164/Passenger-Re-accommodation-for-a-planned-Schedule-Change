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
        string ScheduleID;
        string CarrierCD;
        int FlightNum;

        pair<string,string> EquipmentNo;

        string Src;
        string Dest;

        Time DepartureTime;
        Time ArrivalTime;
        Date StartDate;
        Date EndDate;

        STATUS Status;

        string FlightPattern;
        string tempString;

        getline(inputString, ScheduleID, ',');
        int uuid=scheduleUuidGenerator.getID(ScheduleID);


        getline(inputString,CarrierCD, ',');

        getline(inputString,tempString, ',');
        FlightNum = atoi(tempString.c_str());

        getline(inputString, EquipmentNo.first, ',');
        getline(inputString, EquipmentNo.second, ',');

        getline(inputString, tempString , ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        Src=tempString;

        getline(inputString, tempString, ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        Dest=tempString;
 
        flightNumberMap[FlightNum]= CityPair(Src,Dest);
        cityToFlightNumberMap[CityPair(Src,Dest)]=FlightNum;

        getline(inputString, tempString, ',');
        DepartureTime=Time(tempString);

        getline(inputString, tempString, ',');
        ArrivalTime=Time(tempString);

        getline(inputString, tempString, ',');
        StartDate=Date(tempString);

        getline(inputString, tempString, ',');
        EndDate=Date(tempString);

        getline(inputString, tempString, ',');
        Status = static_cast<STATUS> (getStatusCode(tempString));

        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, FlightPattern, ',');


        Schedule* S = new Schedule(uuid,CarrierCD,EquipmentNo,DepartureTime,ArrivalTime,StartDate,EndDate,FlightPattern,FlightNum,Status);

        scheduleMap[uuid]=S;
    }
}