#pragma once
#include<iostream>
#include<string>
#include<map>
#include "../Utils/constants.h"
using namespace std;

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
    SSR_CD ssrCode;
    LOYALTY_CD loyaltyCode;

    Passenger(int _passengerID, string _lastName, string _firstName, string _nationality,
              string _phoneNum, string _email, string _docId, string _docType,
              SPECIAL_NAME1 _specialName1, SPECIAL_NAME2 _specialName2, SSR_CD _ssrCode,LOYALTY_CD _loyaltyCode):
              passengerID(_passengerID),
              lastName(_lastName),
              firstName(_firstName),
              nationality(_nationality),
              phoneNum(_phoneNum),
              email(_email),
              docID(_docId),
              docType(_docType),
              specialName1(_specialName1),
              specialName2(_specialName2),
              ssrCode(_ssrCode),
              loyaltyCode(_loyaltyCode){}
};

map<int,Passenger*> passengerMap;       //map<PassengerID,Passenger*>
map<int,int> passengerToPnrMap;         //map<PassengerID,PnrID>