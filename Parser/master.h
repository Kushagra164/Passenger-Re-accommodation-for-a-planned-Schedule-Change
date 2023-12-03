#include "datetime.h"
#include "uuidgenerator.h"
#include "inventory.h"
#include "schedule.h"
#include "journey.h"
#include "pnr.h"
#include "passenger.h"
#include <bits/stdc++.h>
using namespace std;

map<char,string> cabinToClassMap;



map<pair<int,DateTime>,int> flightToInventoryMap;       //map< pair<FlightNum,DEP_DTMZ> , InventoryID >
bool precalc = true;

void _fn(){
    for (auto &x: inventoryMap){
        int inv_id = x.first;
        Inventory I = *x.second;
        int s_id = inventoryToScheduleMap[inv_id];
        Schedule *S = scheduleMap[s_id];

        DateTime DepDTMZ(I.DepartureDate,S->DepartureTime);

        flightToInventoryMap[make_pair(S->FlightNum,DepDTMZ)] = inv_id;
    }
}

int getFlight(int FLT_NUM,DateTime DepartureDTMZ){

    if (precalc){
        _fn();
        precalc = false;
    }
    if (! flightToInventoryMap[make_pair(FLT_NUM,DepartureDTMZ)])
        return -1;

    return flightToInventoryMap[make_pair(FLT_NUM,DepartureDTMZ)];
}