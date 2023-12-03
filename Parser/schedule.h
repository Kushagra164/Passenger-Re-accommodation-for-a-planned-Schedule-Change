#define CARRIER_CODE_LENGTH 3            //plus 1 to accomodate null character
#define CITY_CODE_LENGTH 4
#include<bits/stdc++.h>

class Schedule{
private:
    int ScheduleID;
    char CarrierCD[CARRIER_CODE_LENGTH];
    pair<string,string> EquipmentNo;    //pair<AircraftType,AircraftTailNo>
public:
    Time DepartureTime;
    Time ArrivalTime;
    Date StartDate;
    Date EndDate;
    string FlightPattern;
    int FlightNum;
    bool Status;                        //{true: Scheduled , false: Planned}
    vector<int> DepartureDates;         // vector<InventoryID>

    Schedule(int uuid,char carrier_cd[CARRIER_CODE_LENGTH], pair<string,string> &equipment_no,Time dep_time, Time arr_time, Date start_date, Date end_date, string flight_pattern, int flight_num, bool status){
        ScheduleID=uuid;
        strncpy(CarrierCD,carrier_cd,CARRIER_CODE_LENGTH);
        EquipmentNo=equipment_no;
        DepartureTime=dep_time;
        ArrivalTime=arr_time;
        StartDate=start_date;
        EndDate=end_date;
        FlightPattern=flight_pattern;
        FlightNum=flight_num;
        Status=status;
    }
};

map<int,Schedule*> scheduleMap;
map<int,pair<char*,char*>> flightNumberMap;                  //map<FlightNum,pair<Src,Dest>>
map<pair<char*,char*>,int> cityToFlightNumberMap;













