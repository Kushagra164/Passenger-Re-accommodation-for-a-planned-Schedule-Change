#include<string>
#include<map>
#include "../'Utils/constants.h"
using namespace std;

int getSN1Code(string str){
    if(str=="INS") return 1;
    else if(str=="INF") return 2;
    else if(str=="CHD") return 3;
    else if(str=="ADT") return 4;
    else if(str=="UNN") return 5;
    else if(str=="S65") return 6;
    cout<<"Special Name 1 not found: "<<str<<endl;
    else return 0;
}

int getSN2Code(string str){
    if(str=="NRPS") return 1;
    else if(str=="NRSA") return 2;
    cout<<"Special Name 2 not found: "<<str<<endl;
    else return 0;
}

int getSSRCode(string str){
    if(str=="WCHR") return 1;
    else if(str=="WCHS") return 2;
    else if(str=="WCHC") return 3;
    else if(str=="LANG") return 4;
    else if(str=="MAAS") return 5;
    else if(str=="UNMR") return 6;
    else if(str=="BLND") return 7;
    else if(str=="DEAF") return 8;
    else if(str=="EXST") return 9;
    else if(str=="MEAL") return 10;
    else if(str=="NSST") return 11;
    cout<<"SSR not found: "<<str<<endl;
    else return 0;
}

class Passenger{
private:
    int passengerID;
    string lastName;
    string firstName;
    string nationality;
    string phoneNum;
    string email;
    string docID;
    string docType;
public:
    SPECIAL_NAME1 specialName1;
    SPECIAL_NAME2 specialName2;
    vector<SSR_CD> ssrCodes;

    Passenger(int _passengerID, string _lastName, string _firstName, string _nationality,
              string _phoneNum, string _email, string _docId, string _docType,
              SPECIAL_NAME1 _specialName1, SPECIAL_NAME2 _specialName2, vector<SSR_CD> _ssrCodes):
              passengerID(_passengerID),
              lastName(_lastName),
              firstName(_firstName),
              nationality(_nationality),
              phoneNum(_phoneNum),
              email(_email),
              docID(_docId),
              docType(_docType),
              SPECIAL_NAME1(_specialName1),
              SPECIAL_NAME2(_specialName2),
              vector<SSR_CD>(_ssrCodes){}
};

map<int,Passenger*> passengerMap;       //map<PassengerID,Passenger*>
map<int,int> passengerToPnrMap;         //map<PassengerID,PnrID>