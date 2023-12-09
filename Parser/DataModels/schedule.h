#include<string>
#include<map>
#include "../Utils/dateTime.h"
#include "../Utils/constants.h"
using namespace std;

int getStatusCode(string str){
    if(str=="Scheduled") return 0;
    else if(str=="Planning") return 1;
    else if(str=="Cancelled") return 2;
    else if(str=="Delayed") return 3;
    return -1;
}

class Schedule{
private:
    int ScheduleID;
    char CarrierCD[CARRIER_CODE_LENGTH];
public:
    pair<string,string> EquipmentNo;    //pair<AircraftType,AircraftTailNo>
    Time DepartureTime;
    Time ArrivalTime;
    Date StartDate;
    Date EndDate;
    string FlightPattern;
    int FlightNum;
    Statuses Status;                        //{true: Scheduled , false: Planned}
    vector<int> DepartureDates;         // vector<InventoryID>

    Schedule(int uuid,char carrier_cd[CARRIER_CODE_LENGTH], pair<string,string> &equipment_no,Time dep_time, Time arr_time, Date start_date, Date end_date, string flight_pattern, int flight_num, Statuses status){
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
map<int,pair<string,string>> flightNumberMap;                  //map<FlightNum,pair<Src,Dest>>
map<pair<string,string>,int> cityToFlightNumberMap;













