#include<iostream>
#include<string>
#include<map>
#include "../Utils/constants.h"
using namespace std;

int getClassCode(string str){
    if(str=="FC") return 1;
    if(str=="BC") return 2;
    if(str=="PC") return 3;
    if(str=="EC") return 4;
    cout<<"Class Code not found: "<<str<<endl;
    return 0;
}

string getClass(int x){
    if(x==1) return "FC";
    if(x==2) return "BC";
    if(x==3) return "PC";
    if(x==4) return "EC";
    cout<<"Class not found: "<<x<<endl;
    return " ";
}

int getActionCode(string str){
    if(str=="HK") return 1;
    if(str=="PD") return 2;
    cout<<"Action Code not found:"<<str<<endl;
    return 0;
}
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