#include <fstream>
#include <string>
#include "../Input/schedule.h"
#include "../Input/inventory.h"
#include "../Input/booking.h"
#include "../Input/passenger.h"
#include "../Utils/graphOutput.h"
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{

    // handling command line arguments
    ifstream scheduleFile;
    scheduleFile.open(argv[1]);
    ifstream inventoryFile;
    inventoryFile.open(argv[2]);
    ifstream bookingFile;
    bookingFile.open(argv[3]);
    ifstream passengerFile;
    passengerFile.open(argv[4]);

    ofstream fw(argv[5], ofstream::out);
    ofstream output(argv[6], ofstream::out);

    getScheduleInput(scheduleFile);
    getInventoryInput(inventoryFile);
    getBookingInput(bookingFile);
    getPassengerInput(passengerFile);

    // Random Input Simulation
    if (RANDOM_INPUT_SIMULATION)
    {
        srand(RANDOM_SEED);
        for (auto [curInventoryID, _] : inventoryMap)
        {
            if (CancelledFlights.find(curInventoryID) != CancelledFlights.end())
                continue;
            double x = rand();
            x /= RAND_MAX;
            if (x < PROBABILITY_FLIGHT_AFFECTED)
            {
                x = rand();
                x /= RAND_MAX;
                if (x < PROBABILITY_FLIGHT_CANCELLED)
                {
                    CancelledFlights.insert(curInventoryID);
                }
                else
                {
                    // Flight Delay
                    DelayedFlights[curInventoryID] = Time((rand() % 4) + 1);
                }
            }
        }
    }

    // Cancelled and Delayed Flights txt file generation

    output << CancelledFlights.size() << endl;
    for (auto inventoryID : CancelledFlights)
    {
        output << inventoryID << endl;
    }

    output << DelayedFlights.size() << endl;
    for (auto [inventoryID, timeDelay] : DelayedFlights)
    {
        output << inventoryID << " " << timeDelay.to_string() << endl;
    }

    // Graph Creation
    graphWUGenerator();
    cout << "WU finished" << endl;
    graphUVAndGraphDVGenerator();
    cout << "UVD finished" << endl;
    graphUCAndGraphCVGenerator();
    cout << "UCV finished" << endl;
    graphWDGenerator();
    cout << "WD finished" << endl;
    graphOutput(fw);

    fw.close();
}
