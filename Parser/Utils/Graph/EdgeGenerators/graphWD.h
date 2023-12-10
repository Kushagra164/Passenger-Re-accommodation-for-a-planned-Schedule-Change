#pragma once
#include "../Helpers/helper.h"
using namespace std;

void graphWDGenerator(){
    for (int cancelledInventoryID: CancelledFlights){
        int curWIdx = wIndexGenerator.getIndex(cancelledInventoryID);
        for (auto [inventoryID, _]: inventoryMap){
            if ((CancelledFlights.find(inventoryID) == CancelledFlights.end()) 
                    && (isSameCityPair(inventoryID, cancelledInventoryID))){
                int curDIdx = dIndexGenerator.getIndex(inventoryID);
                graphWD[curWIdx].push_back(curDIdx);
            }
        }
    }
}