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
    int prevSegSeq = 0;

    while (getline(bookingFile, line)) {
        string tempString = "";
        Date date;
        Time time;
        bool flag=true;

        stringstream inputString(line);

        string recLoc;

        Date creationDate;
        ACTION_CD actionCD;
        char clsCD;
        int segSeq, paxCnt, flightNum;
        string srcCity;
        string destCity;
        DateTime arrDTMZ, depDTMZ;

        getline(inputString, recLoc, ',');
        int pnr_id=pnrUuidGenerator.getID(recLoc);

        getline(inputString, tempString, ',');
        creationDate = Date(tempString);  

        getline(inputString, tempString, ','); 
        getline(inputString,tempString, ',');
        actionCD = static_cast<ACTION_CD> (getActionCode(tempString));

        getline(inputString,tempString, ',');
        clsCD = tempString[0];

        getline(inputString, tempString, ',');
        segSeq = atoi(tempString.c_str());  
        getline(inputString, tempString, ',');
        paxCnt = atoi(tempString.c_str());  

        if(!pnrMap[pnr_id]){
            Pnr *P = new Pnr(pnr_id,creationDate,paxCnt);
            pnrMap[pnr_id] = P;
            flag=false;
            prevSegSeq=0;
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
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ' ');
        date = Date(tempString);  
        getline(inputString, tempString, ',');
        time = Time(tempString);  
        depDTMZ = DateTime(date, time);

        getline(inputString, tempString, ' ');
        date = Date(tempString);  
        getline(inputString, tempString, ',');
        time = Time(tempString);  
        arrDTMZ = DateTime(date, time);
        int inv_id=getFlight(flightNum,depDTMZ, arrDTMZ);

        Inventory* I=inventoryMap[inv_id];

        int x=getClassCode(cabinToClassMap[CLS_CD]);

        if(x==0) x=4;

        if(flag && (segSeq==prevSegSeq)){
            Journey* J=journeyMap[uuid];
            J->flights.push_back(inv_id);
            J->dest=destCity;
            J->classCD = static_cast <CLASS_CD> (min(J->classCD,static_cast <CLASS_CD> (x)));
        }
        else{
            if(segSeq<prevSegSeq) prevSegSeq=1;
            else prevSegSeq++;
            Journey* J = new Journey(uuid,actionCD,static_cast <CLASS_CD> (x),srcCity,destCity);
            journeyMap[uuid] = J;
            journeyToPnrMap[uuid]=pnr_id;
            J->flights.push_back(inv_id);
            pnrMap[pnr_id]->journeys.push_back(uuid);
            uuid++;
        }
    }
}