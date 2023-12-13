#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include "../Utils/DateTime/dateTime.h"
#include "../DataModels/inventory.h"
#include "../Utils/uuidGenerator.h"
#include "../DataModels/schedule.h"
#include "../Utils/Graph/Helpers/data.h"
using namespace std;

void getInventoryInput(ifstream& inventoryFile){
    string line;
    getline(inventoryFile, line);

    while (getline(inventoryFile, line)) {
        stringstream inputString(line);

        Date departureDate, arrivalDate;
        int totalCapacity, totalInventory;
        int fcTotalCapacity, fcTotalInventory;
        int bcTotalCapacity, bcTotalInventory;
        int pcTotalCapacity, pcTotalInventory;
        int ecTotalCapacity, ecTotalInventory;

        string tempString;

        getline(inputString, tempString, ',');       //InventoryID
        int uuid = inventoryUuidGenerator.getID(tempString);

        getline(inputString, tempString, ',');       //ScheduleID
        int scheduleID = scheduleUuidGenerator.getID(tempString);

        inventoryToScheduleMap[uuid]=scheduleID;

        getline(inputString, tempString, ',');        //CarrierCode

        if(SCHEDULE_LEVEL_CANCELLATION && scheduleMap[scheduleID]->status == SCHEDULE_STATUS::SCHEDULE_CANCELLED) CancelledFlights.insert(uuid);

        scheduleMap[scheduleID]->DepartureDates.push_back(uuid);

        getline(inputString, tempString, ',');         //DepKey

        getline(inputString, tempString, ',');         //FlightNum

        getline(inputString, tempString, ',');         //AircraftType

        getline(inputString, tempString, ',');        //DepartureDate
        departureDate = Date(tempString);

        getline(inputString, tempString, ',');         //DepartureDateTime

        getline(inputString, tempString, ' ');         //Arrival Date
        arrivalDate = Date(tempString);

        getline(inputString, tempString, ',');         //Arrival Time

        getline(inputString, tempString, ',');         //Src
        getline(inputString, tempString, ',');         //Dest

        getline(inputString, tempString, ','); totalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); totalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ','); fcTotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); bcTotalCapacity= atoi(tempString.c_str());
        getline(inputString, tempString, ','); pcTotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); ecTotalCapacity = atoi(tempString.c_str());

        getline(inputString, tempString, ','); fcTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');


        getline(inputString, tempString, ','); bcTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ','); pcTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ','); ecTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ',');


        Inventory* newInventory;

        if(RANDOM_INPUT_SIMULATIOM){
            newInventory = new Inventory(uuid, departureDate, arrivalDate, totalCapacity, totalInventory,0,
                                         fcTotalCapacity, fcTotalInventory,0,
                                         bcTotalCapacity,bcTotalInventory,0,
                                         pcTotalCapacity, pcTotalInventory,0,
                                         ecTotalCapacity, ecTotalInventory, 0);
        }
        else{
            string status;
            Time delayTime;
            getline(inputString, tempString, ',');

            getline(inputString, status, ',');

            getline(inputString, tempString, ',');
            delayTime = Time(tempString);

            newInventory= new Inventory(uuid, departureDate, arrivalDate, totalCapacity, totalInventory,0,
                                        fcTotalCapacity, fcTotalInventory,0,
                                        bcTotalCapacity,bcTotalInventory,0,
                                        pcTotalCapacity, pcTotalInventory,0,
                                        ecTotalCapacity, ecTotalInventory, 0,
                                        status,delayTime);

            if(newInventory->status == INVENTORY_STATUS::INVENTORY_CANCELLED) CancelledFlights.insert(uuid);

            if(newInventory->status == INVENTORY_STATUS::INVENTORY_DELAYED) DelayedFlights[uuid]=delayTime;


        }
        
        inventoryMap[uuid]=newInventory;
    }
    cout<<"Inventory Finished"<<endl;
}