// we hoped in the final run this is not needed
#pragma once
#include<map>
#include "DateTime/dateTime.h"
#include "../DataModels/inventory.h"
#include "../DataModels/schedule.h"
using namespace std
// Map to get the corresponding InventoryID for a pair of Flight Number and Departure DTML
// Used while parsing passenger booking details and creating Journey

map<pair<pair<int,CityPair>,pair<DateTime,DateTime>>,int> flightNumberWithDateTimeToInventoryMap;       //map< pair<FlightNum,pair<DEP_DTML,ARR_DTML>> , InventoryID >
bool flightInventoryMapCal = false;

void calculateFlightInventoryMap(){
    for(auto x:inventoryMap){
        int curInvID = x.first;
        Inventory curInventory= *(x.second);
        int curScheduleID = inventoryToScheduleMap[curInvID];
        Schedule *curSchedule = scheduleMap[curScheduleID];

        DateTime departureTime(curInventory.departureDate, curSchedule->departureTime);
        DateTime arrivalTime(curInventory.arrivalDate, curSchedule->arrivalTime);

        auto cur = make_pair(
                    make_pair(
                            curSchedule->flightNum,
                            CityPair(curSchedule->srcCity, curSchedule->destCity)
                            ),
                    make_pair(
                                departureTime,
                                arrivalTime
                            )
                );
        assert(flightNumberWithDateTimeToInventoryMap.find(cur) == flightNumberWithDateTimeToInventoryMap.end());
        flightNumberWithDateTimeToInventoryMap[cur].insert(
            make_pair(
                cur,
                curInvID
            )
        );
    }
}

int getFlight(int flightNumber,string srcCity, string destCity,DateTime departureTime, DateTime arrivalTime){
    if(!flightInventoryMapCal){
        calculateFlightInventoryMap();
        flightInventoryMapCal = true;
    }

    auto cur = make_pair(
                make_pair(
                        flightNumber,
                        CityPair(srcCity, destCity),
                ),
                make_pair(
                        departureTime,
                        arrivalTime
                        )

            );

    if(flightNumberWithDateTimeToInventoryMap.find(cur)==flightNumberWithDateTimeToInventoryMap.end()){
        cout<<"No flight found"<<endl;
        return -1;
    }

    return flightNumberWithDateTimeToInventoryMap[cur];
}