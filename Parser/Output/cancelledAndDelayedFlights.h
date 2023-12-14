#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "../Utils/DateTime/dateTime.h"
#include "../DataModels/pnr.h"
#include "../DataModels/journey.h"
#include "../DataModels/schedule.h"
#include "../DataModels/inventory.h"
#include "../DataModels/flight.h"
#include "../Utils/uuidGenerator.h"
#include "../Utils/constants.h"

using namespace std;

void getCancelledAndDelayedFlightsInput(ifstream &input)
{
    int noOfCancelledFlights, noOfDelayedFlights;
    input >> noOfCancelledFlights;
    string tempString;
    for (int i = 0; i < noOfCancelledFlights; i++)
    {
        input >> tempString;
        if (tempString == "break")
            break;
        int inventoryID = atoi(tempString.c_str());
        CancelledFlights.insert(inventoryID);
    }
    input >> noOfDelayedFlights;
    for (int i = 0; i < noOfDelayedFlights; i++)
    {
        input >> tempString;
        int inventoryID = atoi(tempString.c_str());
        input >> tempString;
        DelayedFlights[inventoryID] = Time(tempString);
    }
    input.close();
}
