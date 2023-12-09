// we hope in the final run this is not needed
#include<map>
#include "DateTime/dateTime.h"
#include "../DataModels/inventory.h"
#include "../DataModels/schedule.h"
using namespace std;
// Map to get the corresponding InventoryID for a pair of Flight Number and Departure DTMZ
// Used while parsing passenger booking details and creating Journey

multimap<pair<int,pair<DateTime,DateTime>>,int> flightNumberWithDateTimeToInventoryMap;       //map< pair<FlightNum,pair<DEP_DTMZ,ARR_DTMZ>> , InventoryID >
bool flightInventoryMapCal = false;

void calculateFlightInventoryMap(){
    for(auto x:inventoryMap){
        int curInvID = x.first;
        Inventory curInventory= *(x.second);
        int curScheduleID = inventoryToScheduleMap[curInvID];
        Schedule *curSchedule = scheduleMap[curScheduleID];

        DateTime departureTime(curInventory.departureDate, curSchedule->departureTime);
        DateTime arrivalTime(curInventory.arrivalDate, curSchedule->arrivalTime);

        flightNumberWithDateTimeToInventoryMap.insert(
            make_pair(
                make_pair(
                    curSchedule->flightNum,
                    make_pair(departureTime, arrivalTime)
                ),
                curInvID
            )
        );
    }
}

int getFlight(int flightNumber,DateTime departureTime, DateTime arrivalTime){

    if(!flightInventoryMapCal){
        calculateFlightInventoryMap();
        flightInventoryMapCal = true;
    }

    pair<int,pair<DateTime,DateTime>> cur = make_pair(
        flightNumber,
        make_pair(
            departureTime,
            arrivalTime
        )      
    );

    int cnt = flightNumberWithDateTimeToInventoryMap.count(cur);

    if(cnt==0){
        cout<<"No flight found"<<endl;
        return -1;
    }
    else{
        auto it = flightNumberWithDateTimeToInventoryMap.find(cur);
        if(cnt>1){
            cout<<"Too many matching flight found"<<endl;
        }
        return it->second;
    }
}