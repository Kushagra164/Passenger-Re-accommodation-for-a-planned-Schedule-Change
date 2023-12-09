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
    getline(bookingFile, line); line.clear();

    int uuid=0;
    int prev_seg_seq = 0;

    while (getline(bookingFile, line)) {
        string tempString = "";
        Date date;
        Time time;
        bool flag=true;

        stringstream inputString(line);

        string RECLOC;

        Date CREATION_DTZ;
        ACTION_CD actionCD;
        char CLS_CD;
        int SEG_SEQ, PAX_CNT, FLT_NUM;
        string ORIG_CD;
        string DEST_CD;
        DateTime ARR_DTMZ, DEP_DTMZ;

        getline(inputString, RECLOC, ',');
        int pnr_id=pnrUuidGenerator.getID(RECLOC);

        getline(inputString, tempString, ',');
        CREATION_DTZ = Date(tempString);  

        getline(inputString, tempString, ','); 
        getline(inputString,tempString, ',');
        actionCD = static_cast<ACTION_CD> (getActionCode(tempString));

        getline(inputString,tempString, ',');
        CLS_CD = tempString[0];

        getline(inputString, tempString, ',');
        SEG_SEQ = atoi(tempString.c_str());  
        getline(inputString, tempString, ',');
        PAX_CNT = atoi(tempString.c_str());  

        if(!pnrMap[pnr_id]){
            Pnr *P = new Pnr(pnr_id,CREATION_DTZ,PAX_CNT);
            pnrMap[pnr_id] = P;
            flag=false;
            prev_seg_seq=0;
        }


        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ',');
        FLT_NUM = atoi(tempString.c_str());  

        getline(inputString, tempString , ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        ORIG_CD=tempString;
         

        getline(inputString, tempString, ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        DEST_CD=tempString;
         

        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 
        getline(inputString, tempString, ','); 

        getline(inputString, tempString, ' ');
        date = Date(tempString);  
        getline(inputString, tempString, ',');
        time = Time(tempString);  
        DEP_DTMZ = DateTime(date, time);

        getline(inputString, tempString, ' ');
        date = Date(tempString);  
        getline(inputString, tempString, ',');
        time = Time(tempString);  
        ARR_DTMZ = DateTime(date, time);
        int inv_id=getFlight(FLT_NUM,DEP_DTMZ, ARR_DTMZ);

        Inventory* I=inventoryMap[inv_id];

        int x=getClassCode(cabinToClassMap[CLS_CD]);

        if(x==0) x=4;

        if(flag && (SEG_SEQ==prev_seg_seq)){
            Journey* J=journeyMap[uuid];
            J->flights.push_back(inv_id);
            J->dest=DEST_CD;
            J->classCD = static_cast <CLASS_CD> (min(J->classCD,static_cast <CLASS_CD> (x)));
        }
        else{
            if(SEG_SEQ<prev_seg_seq) prev_seg_seq=1;
            else prev_seg_seq++;
            Journey* J = new Journey(uuid,actionCD,static_cast <CLASS_CD> (x),ORIG_CD,DEST_CD);
            journeyMap[uuid] = J;
            journeyToPnrMap[uuid]=pnr_id;
            J->flights.push_back(inv_id);
            pnrMap[pnr_id]->journeys.push_back(uuid);
            uuid++;
        }
    }
}