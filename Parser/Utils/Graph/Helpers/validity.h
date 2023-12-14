#pragma once
#include "data.h"
#include "timeDiff.h"
using namespace std;

bool isSufficientInventoryAvailable(int paxCnt, int inventoryID, CLASS_CD toClassCD)
{
    int availableInventory = getPassengers(inventoryID, toClassCD);
    return paxCnt <= availableInventory;
}

bool isSameCityPair(int originalId, int proposedId)
{
    return (scheduleMap[inventoryToScheduleMap[originalId]]->srcCity ==
            scheduleMap[inventoryToScheduleMap[proposedId]]->srcCity) &&
           (scheduleMap[inventoryToScheduleMap[originalId]]->destCity ==
            scheduleMap[inventoryToScheduleMap[proposedId]]->destCity) &&
           (getArrTimeDiff(originalId, proposedId) + getDepTimeDiff(originalId, proposedId) <= MAXIMUM_ALLOWED_TIME_DIFF * 2);
}