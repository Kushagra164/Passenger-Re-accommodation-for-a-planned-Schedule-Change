#include "data.h"
#include "timeDiff.h"
using namespace std;

bool isSufficientInventoryAvailable(int pax_cnt,int inv_id,int x){
    int avl_inv;
    if(x==0) avl_inv = inventoryMap[inv_id]->fcTotalInventory - inventoryMap[inv_id]->fcBookedInventory;
    else if(x==1) avl_inv = inventoryMap[inv_id]->bcTotalInventory - inventoryMap[inv_id]->bcBookedInventory;
    else if(x==2) avl_inv = inventoryMap[inv_id]->pcTotalInventory - inventoryMap[inv_id]->pcBookedInventory;
    else avl_inv = inventoryMap[inv_id]->ecTotalInventory - inventoryMap[inv_id]->ecBookedInventory;

    return pax_cnt <= avl_inv;
}

bool isSameCityPair(int originalId, int proposedId){
    return (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].srcCity ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].srcCity) &&
           (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].destCity ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].destCity) &&
           (getArrTimeDiff(originalId, proposedId) + getDepTimeDiff(originalId, proposedId) <= MAXIMUM_ALLOWED_TIME_DIFF*2);
}