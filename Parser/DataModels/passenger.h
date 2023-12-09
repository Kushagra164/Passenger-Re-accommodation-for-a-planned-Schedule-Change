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
    else return 0;
}

int getSN2Code(string str){
    if(str=="NRPS") return 1;
    else if(str=="NRSA") return 2;
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

    Passenger(int uuid, string lname, string fname, string nationality, string phonenum, string email, string docid, string doctype, SpecialNames1 sn1, SpecialNames2 sn2, SSRCodes ssr){
        PassengerID = uuid;
        LastName = lname;
        FirstName = fname;
        Nationality = nationality;
        PhoneNum = phonenum;
        Email = email;
        DocID = docid;
        DocType = doctype;
        SpecialName1 = sn1;
        SpecialName2 = sn2;
        SSRCode = ssr;
    }
};

map<int,Passenger*> passengerMap;       //map<PassengerID,Passenger*>
map<int,int> passengerToPnrMap;         //map<PassengerID,PnrID>