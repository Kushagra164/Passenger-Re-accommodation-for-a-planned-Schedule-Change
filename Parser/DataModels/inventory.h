#pragma once
#include <map>
#include "../Utils/DateTime/dateTime.h"
#include "../Utils/constants.h"
#include "../Utils/Graph/Helpers/data.h"
#include "schedule.h"
using namespace std;

class Inventory
{
private:
    int inventoryID;

public:
    Date departureDate;
    Date arrivalDate;

    int totalCapacity;
    int totalInventory;
    int bookedInventory;

    int fcTotalCapacity;
    int fcTotalInventory;
    int fcBookedInventory;

    int bcTotalCapacity;
    int bcTotalInventory;
    int bcBookedInventory;

    int pcTotalCapacity;
    int pcTotalInventory;
    int pcBookedInventory;

    int ecTotalCapacity;
    int ecTotalInventory;
    int ecBookedInventory;

    INVENTORY_STATUS status;
    Time timeDelay;

    Inventory(int _inventoryID, Date _departureDate, Date _arrivalDate,
              int _totalCapacity, int _totalInventory, int _bookedInventory,
              int _fcTotalCapacity, int _fcTotalInventory, int _fcBookedInventory,
              int _bcTotalCapacity, int _bcTotalInventory, int _bcBookedInventory,
              int _pcTotalCapacity, int _pcTotalInventory, int _pcBookedInventory,
              int _ecTotalCapacity, int _ecTotalInventory, int _ecBookedInventory, string _status,
              Time _timeDelay) : inventoryID(_inventoryID),
                                 departureDate(_departureDate),
                                 arrivalDate(_arrivalDate),
                                 totalCapacity(_totalCapacity),
                                 totalInventory(_totalInventory),
                                 bookedInventory(_bookedInventory),
                                 fcTotalCapacity(_fcTotalCapacity),
                                 fcTotalInventory(_fcTotalInventory),
                                 fcBookedInventory(_fcBookedInventory),

                                 bcTotalCapacity(_bcTotalCapacity),
                                 bcTotalInventory(_bcTotalInventory),
                                 bcBookedInventory(_bcBookedInventory),

                                 pcTotalCapacity(_pcTotalCapacity),
                                 pcTotalInventory(_pcTotalInventory),
                                 pcBookedInventory(_pcBookedInventory),

                                 ecTotalCapacity(_ecTotalCapacity),
                                 ecTotalInventory(_ecTotalInventory),
                                 ecBookedInventory(_ecBookedInventory),
                                 status(getInventoryStatus(_status)),
                                 timeDelay(_timeDelay)
    {
    }

    Inventory(int _inventoryID, Date _departureDate, Date _arrivalDate,
              int _totalCapacity, int _totalInventory, int _bookedInventory,
              int _fcTotalCapacity, int _fcTotalInventory, int _fcBookedInventory,
              int _bcTotalCapacity, int _bcTotalInventory, int _bcBookedInventory,
              int _pcTotalCapacity, int _pcTotalInventory, int _pcBookedInventory,
              int _ecTotalCapacity, int _ecTotalInventory,
              int _ecBookedInventory) : inventoryID(_inventoryID),
                                        departureDate(_departureDate),
                                        arrivalDate(_arrivalDate),

                                        totalCapacity(_totalCapacity),
                                        totalInventory(_totalInventory),
                                        bookedInventory(_bookedInventory),

                                        fcTotalCapacity(_fcTotalCapacity),
                                        fcTotalInventory(_fcTotalInventory),
                                        fcBookedInventory(_fcBookedInventory),

                                        bcTotalCapacity(_bcTotalCapacity),
                                        bcTotalInventory(_bcTotalInventory),
                                        bcBookedInventory(_bcBookedInventory),

                                        pcTotalCapacity(_pcTotalCapacity),
                                        pcTotalInventory(_pcTotalInventory),
                                        pcBookedInventory(_pcBookedInventory),

                                        ecTotalCapacity(_ecTotalCapacity),
                                        ecTotalInventory(_ecTotalInventory),
                                        ecBookedInventory(_ecBookedInventory)
    {
    }
};

map<int, Inventory *> inventoryMap;
map<int, int> inventoryToScheduleMap; // map<InventoryID,ScheduleID>

pair<DateTime, DateTime> getInventoryTime(int inventoryID)
{
    DateTime depDateTime = DateTime(inventoryMap[inventoryID]->departureDate, scheduleMap[inventoryToScheduleMap[inventoryID]]->departureTime);
    DateTime arrDateTime = DateTime(inventoryMap[inventoryID]->arrivalDate, scheduleMap[inventoryToScheduleMap[inventoryID]]->arrivalTime);

    if (DelayedFlights.find(inventoryID) != DelayedFlights.end())
    {
        arrDateTime += DelayedFlights[inventoryID];
        depDateTime += DelayedFlights[inventoryID];
    }

    return make_pair(depDateTime, arrDateTime);
}

int getPassengers(int flightID, CLASS_CD classCD)
{
    Inventory *curFlight = inventoryMap[flightID];
    if (classCD == FC)
        return curFlight->fcTotalInventory - curFlight->fcBookedInventory;
    if (classCD == BC)
        return curFlight->bcTotalInventory - curFlight->bcBookedInventory;
    if (classCD == PC)
        return curFlight->pcTotalInventory - curFlight->pcBookedInventory;
    if (classCD == EC)
        return curFlight->ecTotalInventory - curFlight->ecBookedInventory;

    cout << "Invalid Class" << endl;
    exit(-1);
}