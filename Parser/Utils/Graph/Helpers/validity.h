#include "data.h"
#include "timeDiff.h"
using namespace std;

bool isSufficientInventoryAvailable(int paxCnt ,int inventoryID, CLASS_CD toClassCD){
    int availableInventory;
    switch(toClassCD){
        case FC:
            availableInventory = inventoryMap[inventoryID]->fcTotalInventory
                                     - inventoryMap[inventoryID]->fcBookedInventory;
            break;
        case BC:
            availableInventory = inventoryMap[inventoryID]->bcTotalInventory
                                     - inventoryMap[inventoryID]->bcBookedInventory;
            break;
        case PC:
            availableInventory = inventoryMap[inventoryID]->pcTotalInventory
                                     - inventoryMap[inventoryID]->pcBookedInventory;
            break;
        case EC:
            availableInventory = inventoryMap[inventoryID]->ecTotalInventory
                                     - inventoryMap[inventoryID]->ecBookedInventory;
            break;
        default:
            cout<<"Invalid Class"<<endl;
            exit(-1);
    }
    return paxCnt <= availableInventory;
}

bool isSameCityPair(int originalId, int proposedId){
    return (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].srcCity ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].srcCity) &&
           (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].destCity ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].destCity) &&
           (getArrTimeDiff(originalId, proposedId) + getDepTimeDiff(originalId, proposedId) <= MAXIMUM_ALLOWED_TIME_DIFF*2);
}