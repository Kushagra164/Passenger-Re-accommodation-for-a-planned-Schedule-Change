#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include "../Utils/DateTime/dateTime.h"
#include "../DataModels/flight.h"
#include "../DataModels/inventory.h"
#include "../Utils/uuidGenerator.h"
#include "../DataModels/schedule.h"
#include "../Utils/Graph/Helpers/data.h"
using namespace std;

void getInventoryOutput(ofstream& fw,map<int,int> &cancelledFlightToSolutionFlightMap){
    for(auto x:inventoryMap){
        int curInventoryID=x.first;
        Inventory* curInventory=x.second;

        fw<<inventoryUuidGenerator.getString(curInventoryID)<<" ";
        fw<<scheduleUuidGenerator.getString(inventoryToScheduleMap[curInventoryID])<<" ";
        fw<<scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum<<" ";
        fw<<scheduleMap[inventoryToScheduleMap[curInventoryID]]->equipmentNo.first<<" ";
        fw<<curInventory->departureDate.to_string()<<" ";
        fw<<curInventory->arrivalDate.to_string()<<" ";
        fw<<flightNumberMap[scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum].srcCity<<" ";
        fw<<flightNumberMap[scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum].destCity<<" ";

        if(INVENTORY_LEVEL_CANCELLATION){
            if(curInventory->status == INVENTORY_STATUS::INVENTORY_SCHEDULED) fw<<"Scheduled"<<" "<<"null";
            else if(curInventory->status == INVENTORY_STATUS::INVENTORY_PLANNED) fw<<"Planned"<<" "<<"null";
            else if(curInventory->status == INVENTORY_STATUS::INVENTORY_DELAYED) fw<<"Delayed"<<" "<<"null";
        }
        else{
            if(DelayedFlights.find(curInventoryID)!=DelayedFlights.end()) fw<<"Delayed"<<" "<<"null";
            else if(scheduleMap[inventoryToScheduleMap[curInventoryID]]->status == SCHEDULE_SCHEDULED) fw<<"Scheduled"<<" "<<"null";
            else if(scheduleMap[inventoryToScheduleMap[curInventoryID]]->status == SCHEDULE_PLANNED) fw<<"Planned"<<" "<<"null";
        }

        if((INVENTORY_LEVEL_CANCELLATION && curInventory->status == INVENTORY_CANCELLED) ||
                (scheduleMap[inventoryToScheduleMap[curInventoryID]]->status == SCHEDULE_CANCELLED)){
            if((cancelledFlightToSolutionFlightMap.find(curInventoryID) !=
                cancelledFlightToSolutionFlightMap.end())) fw<<"Rescheduled"<<" "<<cancelledFlightToSolutionFlightMap[curInventoryID];
            else fw<<"Cancelled"<<" "<<"null";
        }
        fw<<endl;
    }
}