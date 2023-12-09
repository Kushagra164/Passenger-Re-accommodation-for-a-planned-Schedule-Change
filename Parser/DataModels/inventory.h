#include<map>
#include "../Utils/DateTime/dateTime.h"
using namespace std;

class Inventory{
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

    Inventory(int _inventoryID, Date _departureDate, Date _arrivalDate,
              int _totalCapacity, int _totalInventory, int _bookedInventory,
              int _fcTotalCapacity, int _fcTotalInventory, int _fcBookedInventory,
              int _bcTotalCapacity, int _bcTotalInventory, int _bcBookedInventory,
              int _pcTotalCapacity, int _pcTotalInventory, int _pcBookedInventory,
              int _ecTotalCapacity, int _ecTotalInventory, int _ecBookedInventory):
            inventoryID(_inventoryID),
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
            ecBookedInventory(_ecBookedInventory){}
};

map<int,Inventory*> inventoryMap;
map<int,int> inventoryToScheduleMap;  //map<InventoryID,ScheduleID>