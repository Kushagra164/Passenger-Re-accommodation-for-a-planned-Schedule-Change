#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include "../Utils/DateTime/dateTime.h"
#include "../DataModels/pnr.h"
#include "../DataModels/journey.h"
#include "../DataModels/schedule.h"
#include "../DataModels/inventory.h"
#include "../DataModels/flight.h"
#include "../Utils/uuidGenerator.h"
#include "../Utils/constants.h"

using namespace std;

void printDetails(ofstream& fw,string recLoc, Pnr* curPnr,int curJourneyID,int segSeq, int curInventoryID, CLASS_CD clsCD){
    fw<<recLoc<<" ";
    fw<<curPnr->creationDate.to_string()<<" ";

    if(journeyMap[curJourneyID]->actionCD == ACTION_CD::HK) fw<<"HK"<<" ";
    else if(journeyMap[curJourneyID]->actionCD == ACTION_CD::PD) fw<<"PD"<<" ";

    if(clsCD == CLASS_CD::FC) fw<<"FC"<<" ";
    else if(clsCD == CLASS_CD::BC) fw<<"BC"<<" ";
    else if(clsCD == CLASS_CD::PC) fw<<"PC"<<" ";
    else if(clsCD == CLASS_CD::EC) fw<<"EC"<<" ";

    fw<<segSeq<<" ";
    fw<<curPnr->paxCnt<<" ";
    fw<<scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum<<" ";
    fw<<flightNumberMap[scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum].srcCity<<" ";
    fw<<flightNumberMap[scheduleMap[inventoryToScheduleMap[curInventoryID]]->flightNum].destCity<<" ";

    Date departureDate = inventoryMap[curInventoryID]->departureDate;
    Time departureTime = scheduleMap[inventoryToScheduleMap[curInventoryID]]->departureTime;
    fw<<DateTime(departureDate,departureTime).to_string()<<" ";

    Date arrivalDate = inventoryMap[curInventoryID]->arrivalDate;
    Time arrivalTime = scheduleMap[inventoryToScheduleMap[curInventoryID]]->arrivalTime;
    fw<<DateTime(arrivalDate,arrivalTime).to_string()<<endl;
}

void getBookingOutput(ofstream& fw,map<int,vector<pair<int,CLASS_CD>>> &journeyToConnectingMap,
map<int,pair<int,CLASS_CD>> &journeyToFlightMap){
    for(auto x:pnrMap){
        int curPnrID = x.first;
        Pnr* curPnr = x.second;
        string recLoc=pnrUuidGenerator.getString(curPnrID);
        int segSeq=1;
        for(int curJourneyID:curPnr->journeys){
            if(journeyToConnectingMap.find(curJourneyID)!=journeyToConnectingMap.end()){
                auto itr = journeyToConnectingMap.find(curJourneyID);
                for(auto e: itr->second){
                    printDetails(fw,recLoc,curPnr,curJourneyID,segSeq,e.first,e.second);
                }
                segSeq++;
            }
            else if(journeyToFlightMap.find(curJourneyID)!=journeyToFlightMap.end()){
                auto itr = journeyToFlightMap.find(curJourneyID);
                printDetails(fw,recLoc,curPnr,curJourneyID,segSeq,itr->second.first,itr->second.second);
                segSeq++;
            }
            else if(find(AffectedJourneys.begin(),AffectedJourneys.end(),curJourneyID) != AffectedJourneys.end()) segSeq++;
            else{
                for(auto curInventoryID: journeyMap[curJourneyID]->flights){
                    printDetails(fw,recLoc,curPnr,curJourneyID,segSeq,curInventoryID,journeyMap[curJourneyID]->classCD);
                }
                segSeq++;
            }
        }
    }
}