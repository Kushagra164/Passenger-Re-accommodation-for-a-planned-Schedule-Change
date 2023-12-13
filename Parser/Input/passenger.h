#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include "../DataModels/pnr.h"
#include "../DataModels/inventory.h"
#include "../DataModels/journey.h"
#include "../Utils/uuidGenerator.h"
#include "../Utils/constants.h"
#include "../DataModels/passenger.h"
using namespace std;

void getPassengerInput(ifstream& passengerFile){
    string line;
    getline(passengerFile, line);

    int uuid=0;

    while (getline(passengerFile, line)){
        stringstream inputString(line);
        string recLoc;

        string lastName;string firstName;string nationality;string phoneNum;string email;
        string docID; string docType;
        SPECIAL_NAME1 specialNameCD1;
        SPECIAL_NAME2 specialNameCD2;
        SSR_CD ssrCode;
        LOYALTY_CD loyaltyCode;

        string tempString = "";

        getline(inputString, recLoc, ',');

        int pnrID = pnrUuidGenerator.getID(recLoc);

        passengerToPnrMap[uuid] = pnrID;


        pnrMap[pnrID]->passengers.push_back(uuid);

        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, lastName, ',');
        getline(inputString, firstName, ',');
        getline(inputString, nationality, ',');
        getline(inputString, tempString, ',');
        getline(inputString, phoneNum, ',');
        getline(inputString, email, ',');
        getline(inputString, docType, ',');
        getline(inputString, docID, ',');

        getline(inputString, tempString, ',');
        specialNameCD2 = getSpecialName2(tempString);
        getline(inputString, tempString, ',');
        ssrCode=getSSR(tempString);
        getline(inputString, tempString, ',');
        specialNameCD1 =getSpecialName1(tempString);

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');
        loyaltyCode = getLoyalty(tempString);


        Passenger* curPassenger = new Passenger(uuid,lastName,firstName,nationality,phoneNum,email,docID,docType,specialNameCD1,specialNameCD2,ssrCode,loyaltyCode);
        passengerMap[uuid] = curPassenger;

        Pnr* pnr=pnrMap[pnrID];
        if(specialNameCD1 != INS){
            for(int journeyID: pnr->journeys){
                for(int inventoryID: journeyMap[journeyID]->flights){
                    Inventory* curInventory = inventoryMap[inventoryID];
                    curInventory->bookedInventory++;
                    if(journeyMap[journeyID]->classCD == FC) curInventory->fcBookedInventory++;
                    else if(journeyMap[journeyID]->classCD == BC) curInventory->bcBookedInventory++;
                    else if(journeyMap[journeyID]->classCD == PC) curInventory->pcBookedInventory++;
                    else if(journeyMap[journeyID]->classCD == EC) curInventory->ecBookedInventory++;
                }
            }
        }
        else pnr->paxCnt--;

        uuid++;
    }
    cout<<"Passenger Finished"<<endl;
}