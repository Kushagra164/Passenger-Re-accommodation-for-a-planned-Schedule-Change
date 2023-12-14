#pragma once
#include <vector>
#include <map>
#include "../Utils/DateTime/dateTime.h"
using namespace std;

class Pnr
{
private:
    int pnrID;

public:
    DateTime creationDate;
    int paxCnt;
    vector<int> passengers; // vector<PassengerID>
    vector<int> journeys;   // vector<JourneyID>

    Pnr(int _pnrID, DateTime _creationDate, int _paxCnt) : 
        pnrID(_pnrID), creationDate(_creationDate), paxCnt(_paxCnt) {}
};

map<int, Pnr *> pnrMap; // map<PnrID,Pnr*>