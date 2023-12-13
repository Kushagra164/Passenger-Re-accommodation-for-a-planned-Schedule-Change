#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include "../Utils/DateTime/dateTime.h"
#include "../DataModels/pnr.h"
#include "../DataModels/journey.h"
#include "../Utils/uuidGenerator.h"
#include "../Utils/constants.h"
#include "../Utils/getFlight.h"
using namespace std;

void getBookingInput(ifstream& bookingFile){
    string line;
    getline(bookingFile, line);

    int uuid=0;

    map<int,int> pnrToFlightMap;               //To generate connections

    while (getline(bookingFile, line)) {
        string tempString = "";
        Date date;
        Time time;
        bool newPnr=false;

        stringstream inputString(line);

        string recLoc;

        DateTime creationDate;
        ACTION_CD actionCD;
        CLASS_CD clsCD;
        int paxCnt, flightNum;
        string srcCity;
        string destCity;
        DateTime arrDTML, depDTML;

        getline(inputString, recLoc, ',');
        int pnrID=pnrUuidGenerator.getID(recLoc);

        getline(inputString, tempString, ' ');
        date = Date(tempString);
        getline(inputString, tempString, ',');
        time = Time(tempString);
        creationDate=DateTime(date,time);

        getline(inputString, tempString, ','); 
        getline(inputString,tempString, ',');
        actionCD = getActionCode(tempString);

        getline(inputString,tempString, ',');
        clsCD = getClassCode(tempString);

        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        paxCnt = atoi(tempString.c_str());  

        if(pnrMap.count(pnrID)==0){
            Pnr *P = new Pnr(pnrID,creationDate,paxCnt);
            pnrMap[pnrID] = P;
            newPnr = true;
        }


        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ',');
        flightNum = atoi(tempString.c_str());  

        getline(inputString, tempString , ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        srcCity=tempString;
         

        getline(inputString, tempString, ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        destCity=tempString;
         

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ' ');
        date = Date(tempString);  
        getline(inputString, tempString, ',');
        time = Time(tempString);  
        depDTML = DateTime(date, time);

        getline(inputString, tempString, ' ');
        date = Date(tempString);  
        getline(inputString, tempString, ',');
        time = Time(tempString);  
        arrDTML = DateTime(date, time);
        int curInventoryID=getFlight(flightNum,srcCity,destCity,depDTML, arrDTML);

        bool cond = false;
        if(!newPnr){
            int prevInventoryID = pnrToFlightMap[pnrID];
            DateTime prevArrivalTime = DateTime(inventoryMap[prevInventoryID]->arrivalDate,scheduleMap[inventoryToScheduleMap[prevInventoryID]]->arrivalTime);
            DateTime curDepartureTime = DateTime(inventoryMap[curInventoryID]->departureDate,scheduleMap[inventoryToScheduleMap[curInventoryID]]->departureTime);
            Journey* curJourney = pnrMap[pnrID]->journeys.back();
            cond = (((curDepartureTime-prevArrivalTime) <= MAXIMUM_ALLOWED_TIME_DIFF_FOR_CONNECTING) &&
                    (scheduleMap[inventoryToScheduleMap[curInventoryID]]->destCity != curJourney->src) &&
                    (scheduleMap[inventoryToScheduleMap[curInventoryID]]->srcCity == curJourney->dest));

            if(cond){
                curJourney->dest = scheduleMap[inventoryToScheduleMap[curInventoryID]]->destCity;
                curJourney->flights.push_back(curInventoryID);
            }
        }


        if(newPnr || !cond){
            Journey* curJourney = new Journey(uuid,actionCD,clsCD,srcCity,destCity);
            journeyMap[uuid] = curJourney;
            journeyToPnrMap[uuid]=pnrID;
            curJourney->flights.push_back(curInventoryID);
            pnrMap[pnrID]->journeys.push_back(uuid);
            uuid++;
        }

        pnrToFlightMap[pnrID]=curInventoryID;

    }
    cout<<"Booking Finished   Total Journeys: "<<uuid+1<<endl;
}