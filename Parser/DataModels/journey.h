#pragma once
#include<iostream>
#include<string>
#include<map>
#include "../Utils/constants.h"
using namespace std;

class Journey{
private:
    int journeyID;
public:
    ACTION_CD actionCD;
    CLASS_CD classCD;
    string src;
    string dest;
    vector<int> flights;              //vector<InventoryID>

    Journey(int _journeyID, ACTION_CD _actionCD, CLASS_CD _classCD, string _src, string _dest):
        journeyID(_journeyID),
        actionCD(_actionCD),
        classCD(_classCD),
        src(_src),
        dest(_dest){}
};
map<int,Journey*> journeyMap;             //map<JourneyID,Journey*>
map<int,int> journeyToPnrMap;             //map<JourneyID,PnrID>