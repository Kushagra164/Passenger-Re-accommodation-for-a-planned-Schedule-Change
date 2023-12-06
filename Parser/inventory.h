

class Inventory{
private:
    int InventoryID;
public:
    Date DepartureDate;
    Date ArrivalDate;

    int TotalCapacity;
    int TotalInventory;
    int BookedInventory;

    int FCTotalCapacity;
    int FCTotalInventory;
    int FCBookedInventory;

    int BCTotalCapacity;
    int BCTotalInventory;
    int BCBookedInventory;

    int PCTotalCapacity;
    int PCTotalInventory;
    int PCBookedInventory;

    int ECTotalCapacity;
    int ECTotalInventory;
    int ECBookedInventory;

    Inventory(int uuid, Date dep_date, Date arr_date, int total_cap, int total_inv, int book_inv, int fctotal_cap, int fctotal_inv, int fcbook_inv, int bctotal_cap, int bctotal_inv, int bcbook_inv, int pctotal_cap, int pctotal_inv, int pcbook_inv, int ectotal_cap, int ectotal_inv, int ecbook_inv){
        InventoryID = uuid;
        DepartureDate=dep_date;
        ArrivalDate=arr_date;

        TotalCapacity=total_cap;
        TotalInventory=total_inv;
        BookedInventory=book_inv;

        FCTotalCapacity=fctotal_cap;
        FCTotalInventory=fctotal_inv;
        FCBookedInventory=fcbook_inv;

        BCTotalCapacity=bctotal_cap;
        BCTotalInventory=bctotal_inv;
        BCBookedInventory=bcbook_inv;

        PCTotalCapacity=pctotal_cap;
        PCTotalInventory=pctotal_inv;
        PCBookedInventory=pcbook_inv;

        ECTotalCapacity=ectotal_cap;
        ECTotalInventory=ectotal_inv;
        ECBookedInventory=ecbook_inv;

    }
};

map<int,Inventory*> inventoryMap;

map<int,int> inventoryToScheduleMap;           //map<InventoryID,ScheduleID>