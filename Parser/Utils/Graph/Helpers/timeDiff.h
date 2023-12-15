#pragma once
#include "data.h"
using namespace std;

Time getArrDepTimeDiff(int curInventoryID, int nextInventoryID)
{
    DateTime curTime = getInventoryTime(curInventoryID).second;
    DateTime nextTime = getInventoryTime(nextInventoryID).first;
    return nextTime - curTime;
}

Time getFlightDuration(int curInventoryID)
{
    return getArrDepTimeDiff(curInventoryID, curInventoryID);
}

Time getDepTimeDiff(int curInventoryID, int nextInventoryID)
{
    DateTime curTime = getInventoryTime(curInventoryID).first;
    DateTime nextTime = getInventoryTime(nextInventoryID).first;
    return abs(nextTime - curTime);
}

Time getArrTimeDiff(int curInventoryID, int nextInventoryID)
{
    DateTime curTime = getInventoryTime(curInventoryID).second;
    DateTime nextTime = getInventoryTime(nextInventoryID).second;
    return abs(nextTime - curTime);
}