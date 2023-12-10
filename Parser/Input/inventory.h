#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include "../Utils/DateTime/dateTime.h"
#include "../DataModels/inventory.h"
#include "../Utils/uuidGenerator.h"
#include "../DataModels/schedule.h"
#include "../Utils/processing.h"
using namespace std;

void getInventoryInput(ifstream& inventoryFile){
    string line;
    getline(inventoryFile, line);

    bool flag = true;

    while (getline(inventoryFile, line)) {
        stringstream inputString(line);

        string inventoryID;
        string scheduleID;
        Date departureDate, arrivalDate;
        int totalCapacity, totalInventory;
        int fcTotalCapacity, fcTotalInventory;
        int bcTotalCapacity, bcTotalInventory;
        int pcTotalCapacity, pcTotalInventory;
        int ecTotalCapacity, ecTotalInventory;

        string tempString;

        getline(inputString, inventoryID, ',');
        int uuid = inventoryUuidGenerator.getID(inventoryID);

        getline(inputString, scheduleID, ',');

        int s_id = scheduleUuidGenerator.getID(scheduleID);

        inventoryToScheduleMap[uuid]=s_id;

        if(SCHEDULE_LEVEL_CANCELLATION && scheduleMap[s_id]->status == SCHEDULE_STATUS::SCHEDULE_CANCELLED) CancelledFlights.insert(uuid);

        scheduleMap[s_id]->DepartureDates.push_back(uuid);

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');
        departureDate = Date(tempString);


        getline(inputString, tempString, ',');
        arrivalDate = Date(tempString);

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

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

        if(flag){
            for (auto &ch: {"FC", "BC", "PC", "EC"}) {
                getline(inputString, tempString, '"');
                 
                getline(inputString, tempString, '"');
                for (auto &c: tempString) if (c >= 'A' and c <= 'Z') cabinToClassMap[c] = ch;
                 
            }
            flag=false;
        }
        else{
            for(int i=0;i<4;i++){
                getline(inputString, tempString, '"');
                getline(inputString, tempString, '"');
            }
        }

        if(INVENTORY_LEVEL_CANCELLATION){
            string status;
            getline(inputString, tempString, ',');

            getline(inputString, status, ',');

            getline(inputString, tempString, ',');

            Inventory* I= new Inventory(uuid, departureDate, arrivalDate, totalCapacity, totalInventory,0,
                                        fcTotalCapacity, fcTotalInventory,0,
                                        bcTotalCapacity,bcTotalInventory,0,
                                        pcTotalCapacity, pcTotalInventory,0,
                                        ecTotalCapacity, ecTotalInventory, 0,
                                        status,Time(tempString));

            if(I->status == INVENTORY_STATUS::INVENTORY_CANCELLED) CancelledFlights.insert(uuid);


        }
        else{
            Inventory* I= new Inventory(uuid, departureDate, arrivalDate, totalCapacity, totalInventory,0,
                                        fcTotalCapacity, fcTotalInventory,0,
                                        bcTotalCapacity,bcTotalInventory,0,
                                        pcTotalCapacity, pcTotalInventory,0,
                                        ecTotalCapacity, ecTotalInventory, 0);
        }

        inventoryMap[uuid]=I;
    }
}