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
        string RecLoc;

        string LastName;string FirstName;string Nationality;string PhoneNum;string Email;string DocID; string DocType;
        SPECIAL_NAME1 SPECIAL_NAME_CD1;
        SPECIAL_NAME2 SPECIAL_NAME_CD2;
        vector<SSR_CD> ssrCodes;

        string tempString = "";

        getline(inputString, RecLoc, ',');

        int pnr_id = pnrUuidGenerator.getID(RecLoc);

        passengerToPnrMap[uuid] = pnr_id;


        pnrMap[pnr_id]->passengers.push_back(uuid);

        getline(inputString, tempString, ','); 

        getline(inputString, LastName, ',');
        getline(inputString, FirstName, ',');
        getline(inputString, Nationality, ',');
        getline(inputString, PhoneNum, ',');
        getline(inputString, Email, ',');
        getline(inputString, DocID, ',');
        getline(inputString, DocType, ',');

        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD1 = static_cast<SPECIAL_NAME1>(getSN1Code(tempString));  
        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD2 = static_cast<SPECIAL_NAME2>(getSN2Code(tempString));  
        getline(inputString, tempString, ',');

        //SSR_CODE_CD1 = static_cast<SSR_CD>(getSSRCode(tempString));  
        Passenger* P = new Passenger(uuid,LastName,FirstName,Nationality,PhoneNum,Email,DocID,DocType,SPECIAL_NAME_CD1,SPECIAL_NAME_CD2,SSR_CODE_CD1);
        if(true){                                                            //SSR_CODE_CD1 != 1
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