#include<bits/stdc++.h>

class Pnr{
private:
    int PnrID;
    Date CreationDate;
public:
    int PaxCnt;
    vector<int> Passengers;          //vector<PassengerID>
    vector<int> Journeys;            //vector<JourneyID>

    Pnr(int uuid,Date c_date, int pax_cnt){
        PnrID = uuid;
        CreationDate = c_date;
        PaxCnt = pax_cnt;
    }
};

map<int,Pnr*> pnrMap;                    //map<PnrID,Pnr*>