#pragma once
#include "data.h"
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