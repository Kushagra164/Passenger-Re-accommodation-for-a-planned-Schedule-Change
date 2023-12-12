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
        fw<<curInventory->departureDate<<" ";
        fw<<curInventory->arrivalDate<<" ";
        fw<<flightNumberMap[scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum].srcCity<<" ";
        fw<<flightNumberMap[scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum].destCity<<" ";

        if(INVENTORY_LEVEL_CANCELLATION){
            if(curInventory->status == INVENTORY_STATUS::INVENTORY_SCHEDULED) fw<<"Scheduled"<<" ";
            else if(curInventory->status == INVENTORY_STATUS::INVENTORY_CANCELLED) fw<<"Cancelled"<<" ";
            else if(curInventory->status == INVENTORY_STATUS::INVENTORY_PLANNED) fw<<"Planned"<<" ";
            else if(curInventory->status == INVENTORY_STATUS::INVENTORY_DELAYED) fw<<"Delayed"<<" ";
        }
        else{
            if(scheduleMap[inventoryToScheduleMap[curInventoryID]]->status == SCHEDULE_SCHEDULED) fw<<"Scheduled"<<" ";
            else if(scheduleMap[inventoryToScheduleMap[curInventoryID]]->status == SCHEDULE_PLANNED) fw<<"Planned"<<" ";
            else if(scheduleMap[inventoryToScheduleMap[curInventoryID]]->status == SCHEDULE_CANCELLED) fw<<"Cancelled"<<" ";
        }

        if((INVENTORY_LEVEL_CANCELLATION && curInventory->status == INVENTORY_CANCELLED) ||
            scheduleMap[inventoryToScheduleMap[curInventoryID]]->status == SCHEDULE_CANCELLED){
            fw<<cancelledFlightToSolutionFlightMap[curInventoryID]<<endl;
        }
        else fw<<"null"<<endl;
    }
}