#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;
#include "master.h"

int main() {
    //Parsing of Schedule File

    cerr<<__LINE__<<"aaaa"<<endl;

    ifstream scheduleFile;
    scheduleFile.open("schedule1.csv");                  //argv[1]
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

        Statuses Status;

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
        Status = static_cast<Statuses> (getStatusCode(tempString));
        tempString.clear();

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
    inventoryFile.open("inventory1.csv");                               //argv[2]
    line="";
    getline(inventoryFile, line);
    line ="";

    bool flag=true;

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
        tempString="";
        int uuid = inventoryUuidGenerator.getID(InventoryID);
        //cerr<<uuid<<endl;

        getline(inputString, ScheduleID, ',');
        tempString="";

        int s_id = scheduleUuidGenerator.getID(ScheduleID);

        inventoryToScheduleMap[uuid]=s_id;

        //cerr<<__LINE__ << " " << scheduleMap[s_id]->Status<<endl;

        if(scheduleMap[s_id]->Status == 2) CancelledFlights.insert(uuid);

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
        getline(inputString, tempString, ',');tempString="";

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ',');
        tempString="";

        getline(inputString, tempString, ','); FCTotalCapacity = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); BCTotalCapacity= atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); PCTotalCapacity = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ','); ECTotalCapacity = atoi(tempString.c_str());tempString="";

        getline(inputString, tempString, ','); FCTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";


        getline(inputString, tempString, ','); BCTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";

        getline(inputString, tempString, ','); PCTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";
        getline(inputString, tempString, ',');tempString="";

        getline(inputString, tempString, ','); ECTotalInventory = atoi(tempString.c_str());tempString="";
        getline(inputString, tempString, ',');tempString="";
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
        Inventory* I= new Inventory(uuid, DepartureDate, ArrivalDate, TotalCapacity, TotalInventory,0, FCTotalCapacity, FCTotalInventory,0, BCTotalCapacity, BCTotalInventory,0, PCTotalCapacity, PCTotalInventory,0, ECTotalCapacity, ECTotalInventory, 0);

        inventoryMap[uuid]=I;


    }








    // Parsing of Passenger Booking File

    ifstream bookingFile;
    bookingFile.open("booking1.csv");                       //argv[3]
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
        ActionCDs ACTION_CD;
        char CLS_CD;
        int SEG_SEQ, PAX_CNT, FLT_NUM;
        char ORIG_CD[CITY_CODE_LENGTH],DEST_CD[CITY_CODE_LENGTH];
        DateTime DEP_DTMZ;

        getline(inputString, RECLOC, ',');
        int pnr_id=pnrUuidGenerator.getID(RECLOC);

        getline(inputString, tempString, ',');
        CREATION_DTZ = Date(tempString); tempString.clear();

        getline(inputString, tempString, ',');tempString.clear();
        getline(inputString,tempString, ',');
        ACTION_CD = static_cast<ActionCDs> (getActionCode(tempString));

        getline(inputString,tempString, ',');
        CLS_CD = (char) tempString[0];

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

        Inventory* I=inventoryMap[inv_id];

        int x=getClassCode(cabinToClassMap[CLS_CD]);

        if(x==0) x=4;

        if(flag && (SEG_SEQ==prev_seg_seq)){
            Journey* J=journeyMap[uuid];
            J->flights.push_back(inv_id);
            strncpy(J->Dest,DEST_CD,CITY_CODE_LENGTH);
            J->ClassCD = static_cast <ClassCDs> (min(J->ClassCD,static_cast <ClassCDs> (x)));
        }
        else{
            if(SEG_SEQ<prev_seg_seq) prev_seg_seq=1;
            else prev_seg_seq++;
            Journey* J = new Journey(uuid,ACTION_CD,static_cast <ClassCDs> (x),ORIG_CD,DEST_CD);
            journeyMap[uuid] = J;
            journeyToPnrMap[uuid]=pnr_id;
            J->flights.push_back(inv_id);
            //cerr<<__LINE__<<" "<<inv_id<<endl;
            pnrMap[pnr_id]->Journeys.push_back(uuid);
            uuid++;
        }

        //cerr<<__LINE__<<endl;
    }









    //Parsing of Passenger Details File


    ifstream passengerFile;
    passengerFile.open("passenger1.csv");                   //argv[4]
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

        //cerr<<__LINE__<<endl;

        pnrMap[pnr_id]->Passengers.push_back(uuid);

        getline(inputString, tempString, ',');tempString="";

        getline(inputString, LastName, ',');
        getline(inputString, FirstName, ',');
        getline(inputString, Nationality, ',');
        getline(inputString, PhoneNum, ',');
        getline(inputString, Email, ',');
        getline(inputString, DocID, ',');
        getline(inputString, DocType, ',');

        //cerr<<__LINE__<<endl;

        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD1 = static_cast<SpecialNames1>(getSN1Code(tempString)); tempString.clear();
        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD2 = static_cast<SpecialNames2>(getSN2Code(tempString)); tempString.clear();
        getline(inputString, tempString, ',');
        //cerr<<__LINE__<< tempString << endl;
        SSR_CODE_CD1 = static_cast<SSRCodes>(getSSRCode(tempString)); tempString.clear();
        //cerr<<__LINE__<< tempString << endl;
        Passenger* P = new Passenger(uuid,LastName,FirstName,Nationality,PhoneNum,Email,DocID,DocType,SPECIAL_NAME_CD1,SPECIAL_NAME_CD2,SSR_CODE_CD1);
        //cerr<<__LINE__<< tempString << endl;
        if(true){                                                            //SSR_CODE_CD1 != 1
            Pnr* pnr=pnrMap[pnr_id];
            //cerr<<__LINE__ << endl;
            for(int j_id: pnr->Journeys){
                for(int inv_id: journeyMap[j_id]->flights){
                    //cerr << j_id << " " << inv_id << "\n";
                    Inventory* I=inventoryMap[inv_id];
                    I->BookedInventory++;
                    int x=journeyMap[j_id]->ClassCD;
                    if(x==1) I->FCBookedInventory++;
                    else if(x==2) I->BCBookedInventory++;
                    else if(x==3) I->PCBookedInventory++;
                    else if(x==4) I->ECBookedInventory++;
                }
            }
        }
        //cerr<<__LINE__<< tempString << endl;

        passengerMap[uuid] = P;

        uuid++;
        //cerr<<__LINE__<<endl;
    }
    //Graph Creation
    int m1=0;
    int m6=graphWUGenerator();
    pair<int,int> p=graphUVandDVGenerator();
    int m2=p.first;
    int m3=0;
    int m4=p.second;
    int m5=graphWDGenerator();


    //Output File Generation
    freopen("edges.txt",  "r", stdin);
    ofstream fw("output.txt",ofstream::out);

    int no_of_samples;
    cin>>no_of_samples;

    for(int i=0;i<no_of_samples;i++){

        fw<<"Solution"<<i+1<<endl;

        int no_of_uc_edges;
        cin>>no_of_uc_edges;
        for(int j=0;j<no_of_uc_edges;j++){
            int u,c;
            cin>>u>>c;

            int j_id = uIndexGenerator.getID(u);

        }



        int no_of_uv_edges;
        cin>>no_of_uv_edges;
        for(int j=0;j<no_of_uv_edges;j++){
            int u,v;
            cin>>u>>v;

            int j_id = uIndexGenerator.getID(u);
            cerr<<__LINE__<<" "<<j_id<<endl;
            pair<int,ClassCDs> p=vIndexGenerator.getID(v);
            cerr<<__LINE__<<" "<<journeyToPnrMap[j_id]<<endl;
            fw<<pnrUuidGenerator.getString(journeyToPnrMap[j_id])<<" "<<inventoryUuidGenerator.getString(p.first)<<" "<<p.second<<endl;
        }

        int no_of_wd_edges;
        cin >> no_of_wd_edges;
        for (int iter = 0; iter < no_of_wd_edges; ++iter){
            int w, d;
            cin >> w >> d;
            int winvId = wIndexGenerator.getID(w);
            int dinvId = dIndexGenerator.getID(d);
            fw << inventoryUuidGenerator.getString(winvId) << " " << inventoryUuidGenerator.getString(dinvId)<<endl;
        }
    }
    fw.close();

}