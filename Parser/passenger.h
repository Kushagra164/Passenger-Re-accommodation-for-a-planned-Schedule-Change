#include<bits/stdc++.h>
enum SpecialNames1 {INS,INF,CHD,ADT,UNN,S65};
enum SpecialNames2 {NRPS,NRSA};
enum SSRCodes {WCHR,WCHS,WCHC,LANG,MAAS,UNMR,BLND,DEAF,EXST,MEAL,NSST};

int getSN1Code(string str){
    if(str=="INS") return 0;
    else if(str=="INF") return 1;
    else if(str=="CHD") return 2;
    else if(str=="ADT") return 3;
    else if(str=="UNN") return 4;
    else if(str=="S65") return 5;
    else return -1;
}

int getSN2Code(string str){
    if(str=="NRPS") return 0;
    else if(str=="NRSA") return 1;
    else return -1;
}

int getSSRCode(string str){
    if(str=="WCHR") return 0;
    else if(str=="WCHS") return 1;
    else if(str=="WCHC") return 2;
    else if(str=="LANG") return 3;
    else if(str=="MAAS") return 4;
    else if(str=="UNMR") return 5;
    else if(str=="BLND") return 6;
    else if(str=="DEAF") return 7;
    else if(str=="EXST") return 8;
    else if(str=="MEAL") return 9;
    else if(str=="NSST") return 10;
    else return -1;
}

class Passenger{
private:
    int PassengerID;
    string LastName;
    string FirstName;
    string Nationality;
    string PhoneNum;
    string Email;
    string DocID;
    string DocType;
public:
    SpecialNames1 SpecialName1;
    SpecialNames2 SpecialName2;
    SSRCodes SSRCode;

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