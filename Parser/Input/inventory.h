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

        string InventoryID;
        string ScheduleID;
        Date DepartureDate, ArrivalDate;
        int TotalCapacity, TotalInventory;
        int FCTotalCapacity, FCTotalInventory;
        int BCTotalCapacity, BCTotalInventory;
        int PCTotalCapacity, PCTotalInventory;
        int ECTotalCapacity, ECTotalInventory;

        string tempString;

        getline(inputString, InventoryID, ',');
        int uuid = inventoryUuidGenerator.getID(InventoryID);

        getline(inputString, ScheduleID, ',');

        int s_id = scheduleUuidGenerator.getID(ScheduleID);

        inventoryToScheduleMap[uuid]=s_id;

        if(scheduleMap[s_id]->status == 2) CancelledFlights.insert(uuid);

        scheduleMap[s_id]->DepartureDates.push_back(uuid);

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');
        DepartureDate = Date(tempString);


        getline(inputString, tempString, ',');
        ArrivalDate = Date(tempString);

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ','); TotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); TotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ','); FCTotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); BCTotalCapacity= atoi(tempString.c_str());
        getline(inputString, tempString, ','); PCTotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); ECTotalCapacity = atoi(tempString.c_str());

        getline(inputString, tempString, ','); FCTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');


        getline(inputString, tempString, ','); BCTotalInventory = atoi(tempString.c_str()); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ','); PCTotalInventory = atoi(tempString.c_str()); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ','); ECTotalInventory = atoi(tempString.c_str()); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 

        while(flag){
            for (auto &ch: {"FC", "BC", "PC", "EC"}) {
                getline(inputString, tempString, '"');
                 
                getline(inputString, tempString, '"');
                for (auto &c: tempString) if (c >= 'A' and c <= 'Z') cabinToClassMap[c] = ch;
                 
            }
            flag=false;
        }

        Inventory* I= new Inventory(uuid, DepartureDate, ArrivalDate, TotalCapacity, TotalInventory,0, FCTotalCapacity, FCTotalInventory,0, BCTotalCapacity, BCTotalInventory,0, PCTotalCapacity, PCTotalInventory,0, ECTotalCapacity, ECTotalInventory, 0);

        inventoryMap[uuid]=I;
    }
}