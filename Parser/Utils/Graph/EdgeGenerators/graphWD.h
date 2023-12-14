#pragma once
#include "../Helpers/helper.h"
using namespace std;

void graphWDGenerator()
{
    for (int cancelledInventoryID : CancelledFlights)
    {
        int curWIdx = wIndexGenerator.getIndex(cancelledInventoryID);
        for (auto [inventoryID, _] : inventoryMap)
        {
            if ((CancelledFlights.find(inventoryID) == CancelledFlights.end()) && (isSameCityPair(inventoryID, cancelledInventoryID)) && ((getDepTimeDiff(cancelledInventoryID, inventoryID) + getArrTimeDiff(cancelledInventoryID, inventoryID)) <= MAXIMUM_ALLOWED_TIME_DIFF * 2))
            {
                int curDIdx = dIndexGenerator.getIndex(inventoryID);
                graphWD[curWIdx].push_back(curDIdx);
            }
        }
    }
}