#pragma once
#include "data.h"
using namespace std;

Time getArrDepTimeDiff(int curInventoryID, int nextInventoryID){
    DateTime curTime = DateTime(inventoryMap[curInventoryID]->arrivalDate,
                            scheduleMap[inventoryToScheduleMap[curInventoryID]]->arrivalTime);
    DateTime nextTime = DateTime(inventoryMap[nextInventoryID]->departureDate,
                            scheduleMap[inventoryToScheduleMap[nextInventoryID]]->departureTime);
    return nextTime-curTime;
}

Time getFlightDuration(int curInventoryID){
    return getArrDepTimeDiff(curInventoryID, curInventoryID);
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