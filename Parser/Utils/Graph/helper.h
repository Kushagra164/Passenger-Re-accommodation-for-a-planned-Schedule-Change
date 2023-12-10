#include "data.h"
#include "../../DataModels/inventory.h"
#include "../../DataModels/schedule.h"
#include "../../DataModels/journey.h"
#include "../../DataModels/passenger.h"
#include "../../DataModels/pnr.h"
#include "../../DataModels/flight.h"
using namespace std;

Time getArrDepTimeDiff(int curInvID, int nextInvID){
    DateTime curTime = DateTime(inventoryMap[curInvID]->departureDate,
                            scheduleMap[inventoryToScheduleMap[curInvID]]->arrivalTime);
    DateTime nextTime = DateTime(inventoryMap[nextInvID]->departureDate,
                            scheduleMap[inventoryToScheduleMap[nextInvID]]->departureTime);
    return nextTime-curTime;
}

Time getDepTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id]->departureDate,
                            scheduleMap[inventoryToScheduleMap[o_inv_id]]->departureTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id]->departureDate,
                            scheduleMap[inventoryToScheduleMap[p_inv_id]]->departureTime);
    return abs(original - proposed);
}

Time getArrTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id]->arrivalDate,
                            scheduleMap[inventoryToScheduleMap[o_inv_id]]->arrivalTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id]->arrivalDate,
                            scheduleMap[inventoryToScheduleMap[p_inv_id]]->arrivalTime);
    return abs(original - proposed);
}

bool isSufficientInventoryAvailable(int pax_cnt ,int inv_id,int x){
    int avl_inv;
    if(x==1) avl_inv = inventoryMap[inv_id]->fcTotalInventory - inventoryMap[inv_id]->fcBookedInventory;
    else if(x==2) avl_inv = inventoryMap[inv_id]->bcTotalInventory - inventoryMap[inv_id]->bcBookedInventory;
    else if(x==3) avl_inv = inventoryMap[inv_id]->pcTotalInventory - inventoryMap[inv_id]->pcBookedInventory;
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