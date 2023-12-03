#include <bits/stdc++.h>
#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;
#include "master.h"

int main() {

    //Parsing of Schedule File

    ifstream scheduleFile;
    scheduleFile.open("Schedule.csv");
    string line="";
    getline(scheduleFile, line);
    line ="";

    while (getline(scheduleFile, line)) {

        stringstream inputString(line);
        //ScheduleID,CarrierCD,FlightNumber, AircraftType, AircraftTailNumber,DepartureTime,ArrivalTime,StartDate,EndDate,FlightPattern,Status,DepartureDates
        string ScheduleID;
        char CarrierCD[CARRIER_CODE_LENGTH];
        int FlightNum;

        pair<string,string> EquipmentNo;

        char Src[CITY_CODE_LENGTH];
        char Dest[CITY_CODE_LENGTH];

        Time DepartureTime;
        Time ArrivalTime;
        Date StartDate;
        Date EndDate;

        bool Status=false;

        string FlightPattern;
        string tempString;
        tempString="";

        getline(inputString, ScheduleID, ',');
        int uuid=scheduleUuidGenerator.getID(ScheduleID);


        getline(inputString,tempString, ',');
        for(int i=0;i<CARRIER_CODE_LENGTH;i++) CarrierCD[i]= tempString[i];
        tempString="";

        getline(inputString,tempString, ',');
        FlightNum = atoi(tempString.c_str());
        tempString="";

        getline(inputString, EquipmentNo.first, ',');
        getline(inputString, EquipmentNo.second, ',');

        getline(inputString, tempString , ',');
        for(int i=0;i<CITY_CODE_LENGTH;i++) Src[i]= tempString[i];
        tempString="";

        getline(inputString, tempString, ',');
        for(int i=0;i<CITY_CODE_LENGTH;i++) Dest[i]= tempString[i];
        tempString="";

        flightNumberMap[FlightNum]={Src,Dest};
        cityToFlightNumberMap[make_pair(Src,Dest)]=FlightNum;

        getline(inputString, tempString, ',');
        DepartureTime=Time(tempString);
        tempString="";

        getline(inputString, tempString, ',');
        ArrivalTime=Time(tempString);
        tempString="";

        getline(inputString, tempString, ',');
        StartDate=Date(tempString);
        tempString="";

        getline(inputString, tempString, ',');
        EndDate=Date(tempString);
        tempString="";

        getline(inputString, tempString, ',');

        if(tempString=="Scheduled") Status=true;

        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, FlightPattern, ',');

        line = "";

        Schedule* S = new Schedule(uuid,CarrierCD,EquipmentNo,DepartureTime,ArrivalTime,StartDate,EndDate,FlightPattern,FlightNum,Status);

        scheduleMap[uuid]=S;
    }








    //Parsing of Inventory File

    ifstream inventoryFile;
    inventoryFile.open("Inventory.csv");
    line="";
    getline(inventoryFile, line);
    line ="";

    bool flag=true;

    while (getline(inventoryFile, line)) {
        stringstream inputString(line);

        string InventoryID;
        string ScheduleID;
        Date DepartureDate, ArrivalDate;
        int TotalCapacity, TotalInventory,BookedInventory;
        int FCTotalCapacity, FCTotalInventory,FCBookedInventory;
        int BCTotalCapacity, BCTotalInventory,BCBookedInventory;
        int PCTotalCapacity, PCTotalInventory,PCBookedInventory;
        int ECTotalCapacity, ECTotalInventory,ECBookedInventory;

        string tempString;

        getline(inputString, InventoryID, ',');
        tempString="";
        int uuid = inventoryUuidGenerator.getID(InventoryID);

        getline(inputString, ScheduleID, ',');
        tempString="";

        int s_id = scheduleUuidGenerator.getID(ScheduleID);

        inventoryToScheduleMap[uuid]=s_id;

        scheduleMap[s_id]->DepartureDates.push_back(uuid);

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ',');
        DepartureDate = Date(tempString);
        tempString="";

        getline(inputString, tempString, ',');
        ArrivalDate = Date(tempString);
        tempString="";

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ','); TotalCapacity = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); TotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); BookedInventory = atoi(tempString.c_str());tempString="";

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ','); FCTotalCapacity = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); BCTotalCapacity= atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); PCTotalCapacity = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); ECTotalCapacity = atoi(tempString.c_str());tempString="";

        getline(inputString, tempString, ','); FCTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); FCBookedInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";


        getline(inputString, tempString, ','); BCTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); BCBookedInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";

        getline(inputString, tempString, ','); PCTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); PCBookedInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";

        getline(inputString, tempString, ','); ECTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); ECBookedInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";

        while(flag){
            for (auto &ch: {"FC", "BC", "PC", "EC"}) {
                getline(inputString, tempString, '"');
                tempString.clear();
                getline(inputString, tempString, '"');
                for (auto &c: tempString) if (c >= 'A' and c <= 'Z') cabinToClassMap[c] = ch;
                tempString.clear();
            }
            flag=false;
        }

        line = "";
        Inventory* I= new Inventory(uuid, DepartureDate, ArrivalDate, TotalCapacity, TotalInventory, BookedInventory, FCTotalCapacity, FCTotalInventory, FCBookedInventory, BCTotalCapacity, BCTotalInventory, BCBookedInventory, PCTotalCapacity, PCTotalInventory, PCBookedInventory, ECTotalCapacity, ECTotalInventory, ECBookedInventory );

        inventoryMap[uuid]=I;

    }








    // Parsing of Passenger Booking File

    ifstream bookingFile;
    bookingFile.open("Booking.csv");
    line = "";
    getline(bookingFile, line); line.clear();

    int uuid=0;
    int prev_seg_seq = 0;


    while (getline(bookingFile, line)) {
        string tempString = "";
        Date date;
        Time time;
        flag=true;

        stringstream inputString(line);

        string RECLOC;

        Date CREATION_DTZ;
        string ACTION_CD;
        string CLS_CD;
        int SEG_SEQ, PAX_CNT, FLT_NUM;
        char ORIG_CD[CITY_CODE_LENGTH],DEST_CD[CITY_CODE_LENGTH];
        DateTime DEP_DTMZ;

        getline(inputString, RECLOC, ',');
        int pnr_id=pnrUuidGenerator.getID(RECLOC);

        getline(inputString, tempString, ',');
        CREATION_DTZ = Date(tempString); tempString.clear();

        getline(inputString, tempString, ',');tempString.clear();
        getline(inputString, ACTION_CD, ',');
        getline(inputString, CLS_CD, ',');

        getline(inputString, tempString, ',');
        SEG_SEQ = atoi(tempString.c_str()); tempString.clear();
        getline(inputString, tempString, ',');
        PAX_CNT = atoi(tempString.c_str()); tempString.clear();

        if(!pnrMap[pnr_id]){
            Pnr *P = new Pnr(pnr_id,CREATION_DTZ,PAX_CNT);
            pnrMap[pnr_id] = P;
            flag=false;
            prev_seg_seq=0;
        }


        getline(inputString, tempString, ',');tempString.clear();

        getline(inputString, tempString, ',');
        FLT_NUM = atoi(tempString.c_str()); tempString.clear();

        getline(inputString, tempString , ',');
        for(int i=0;i<CITY_CODE_LENGTH;i++) ORIG_CD[i]= tempString[i];
        tempString="";

        getline(inputString, tempString, ',');
        for(int i=0;i<CITY_CODE_LENGTH;i++) DEST_CD[i]= tempString[i];
        tempString="";

        getline(inputString, tempString, ',');tempString.clear();
        getline(inputString, tempString, ',');tempString.clear();
        getline(inputString, tempString, ',');tempString.clear();

        getline(inputString, tempString, ' ');
        date = Date(tempString); tempString.clear();
        getline(inputString, tempString, ',');
        time = Time(tempString); tempString.clear();
        DEP_DTMZ = DateTime(date, time);

        line="";

        int inv_id=getFlight(FLT_NUM,DEP_DTMZ);

        if(flag && (SEG_SEQ==prev_seg_seq)){
            Journey* J=journeyMap[uuid];
            J->flights.push_back(inv_id);
            strncpy(J->Dest,DEST_CD,CITY_CODE_LENGTH);
            J->ClassCD = static_cast <ClassCDs> (min(J->ClassCD,static_cast <ClassCDs> (getClassCode(cabinToClassMap[CLS_CD]))));
        }
        else{
            if(SEG_SEQ<prev_seg_seq) prev_seg_seq=1;
            else prev_seg_seq++;
            Journey* J = new Journey(uuid,ACTION_CD,static_cast <ClassCDs> (getClassCode(cabinToClassMap[CLS_CD])),ORIG_CD,DEST_CD);
            journeyMap[uuid] = J;
            J->flights.push_back(inv_id);
            pnrMap[pnr_id]->Journeys.push_back(uuid);
            uuid++;
        }
    }









    //Parsing of Passenger Details File


    ifstream passengerFile;
    passengerFile.open("Passenger.csv");
    line = "";
    getline(passengerFile, line);
    line.clear();

    uuid=0;

    while (getline(passengerFile, line)){
        stringstream inputString(line);
        string RecLoc;

        string LastName;string FirstName;string Nationality;string PhoneNum;string Email;string DocID; string DocType;
        SpecialNames1 SPECIAL_NAME_CD1;
        SpecialNames2 SPECIAL_NAME_CD2;
        SSRCodes SSR_CODE_CD1;

        string tempString = "";

        getline(inputString, RecLoc, ',');

        int pnr_id = pnrUuidGenerator.getID(RecLoc);

        passengerToPnrMap[uuid] = pnr_id;

        pnrMap[pnr_id]->Passengers.push_back(uuid);

        getline(inputString, tempString, ',');tempString="";

        getline(inputString, LastName, ',');
        getline(inputString, FirstName, ',');
        getline(inputString, Nationality, ',');
        getline(inputString, PhoneNum, ',');
        getline(inputString, Email, ',');
        getline(inputString, DocID, ',');
        getline(inputString, DocType, ',');

        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD1 = static_cast<SpecialNames1>(getSN1Code(tempString)); tempString.clear();
        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD2 = static_cast<SpecialNames2>(getSN2Code(tempString)); tempString.clear();
        getline(inputString, tempString, ',');
        SSR_CODE_CD1 = static_cast<SSRCodes>(getSSRCode(tempString)); tempString.clear();

        Passenger* P = new Passenger(uuid,LastName,FirstName,Nationality,PhoneNum,Email,DocID,DocType,SPECIAL_NAME_CD1,SPECIAL_NAME_CD2,SSR_CODE_CD1);

        passengerMap[uuid] = P;

        uuid++;
    }

}