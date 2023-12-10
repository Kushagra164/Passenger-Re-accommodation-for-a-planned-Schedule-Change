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
        vector<SSR_CD> ssrCodes;

        string tempString = "";

        getline(inputString, recLoc, ',');

        int pnr_id = pnrUuidGenerator.getID(recLoc);

        passengerToPnrMap[uuid] = pnr_id;


        pnrMap[pnr_id]->passengers.push_back(uuid);

        getline(inputString, tempString, ','); 

        getline(inputString, lastName, ',');
        getline(inputString, firstName, ',');
        getline(inputString, nationality, ',');
        getline(inputString, phoneNum, ',');
        getline(inputString, email, ',');
        getline(inputString, docID, ',');
        getline(inputString, docType, ',');

        getline(inputString, tempString, ',');
        specialNameCD1 = static_cast<SPECIAL_NAME1>(getSN1Code(tempString));
        getline(inputString, tempString, ',');
        specialNameCD2 = static_cast<SPECIAL_NAME2>(getSN2Code(tempString));
        while(getline(inputString, tempString, ',')){
            ssrCodes.push_back(static_cast<SSR_CD> (getSSRCode(tempString)));
        }


        Passenger* P = new Passenger(uuid,lastName,firstName,nationality,phoneNum,email,docID,docType,specialNameCD1,specialNameCD2,ssrCodes);
        if(specialNameCD1 != 1){
            Pnr* pnr=pnrMap[pnr_id];
            for(int j_id: pnr->journeys){
                for(int inv_id: journeyMap[j_id]->flights){
                    Inventory* I=inventoryMap[inv_id];
                    I->bookedInventory++;
                    int x=journeyMap[j_id]->classCD;
                    if(x==1) I->fcBookedInventory++;
                    else if(x==2) I->bcBookedInventory++;
                    else if(x==3) I->pcBookedInventory++;
                    else if(x==4) I->ecBookedInventory++;
                }
            }
        }

        passengerMap[uuid] = P;

        uuid++;
    }
}