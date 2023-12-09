#pragma once
#include<vector>
#include<map>
#include "../Utils/DateTime/dateTime.h"
using namespace std;

class Pnr{
private:
    int pnrID;
    Date creationDate;
public:
    int paxCnt;
    vector<int> passengers;          //vector<PassengerID>
    vector<int> journeys;            //vector<JourneyID>

    Pnr(int _pnrID,Date _creationDate, int _paxCnt):
        pnrID(_pnrID), creationDate(_creationDate), paxCnt(_paxCnt){}
};

map<int,Pnr*> pnrMap;                    //map<PnrID,Pnr*>