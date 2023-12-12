#pragma once
#include "data.h"
#include "timeDiff.h"
using namespace std;

bool isSufficientInventoryAvailable(int paxCnt ,int inventoryID, CLASS_CD toClassCD){
    int availableInventory= getPassengers(inventoryID,CLASS_CD);
    return paxCnt <= availableInventory;
}

bool isSameCityPair(int originalId, int proposedId){
    return (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].srcCity ==
            flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].srcCity) &&
           (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].destCity ==
            flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].destCity) &&
           (getArrTimeDiff(originalId, proposedId) + getDepTimeDiff(originalId, proposedId) <= MAXIMUM_ALLOWED_TIME_DIFF*2);
}