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
        
        if(curInventory->status == INVENTORY_STATUS::INVENTORY_SCHEDULED) fw<<"Scheduled"<<" ";
        else if(curInventory->status == INVENTORY_STATUS::INVENTORY_CANCELLED) fw<<"Cancelled"<<" ";
        else if(curInventory->status == INVENTORY_STATUS::INVENTORY_PLANNED) fw<<"Planned"<<" ";
        else if(curInventory->status == INVENTORY_STATUS::INVENTORY_DELAYED) fw<<"Delayed"<<" ";

        if(curInventory->status == INVENTORY_STATUS::INVENTORY_CANCELLED){
            fw<<cancelledFlightToSolutionFlightMap[curInventoryID]<<endl;
        }
        else fw<<"null"<<endl;
    }
}