#pragma once
#include<fstream>
#include<string>
#include "../Input/schedule.h"
#include "../Input/inventory.h"
#include "../Input/booking.h"
#include "../Input/passenger.h"
#include "../Utils/graphOutput.h"
using namespace std;

int main(int argc,char* argv[]) {
    //Parsing of Schedule File

    // handling command line arguments
    ifstream scheduleFile;
    scheduleFile.open(argv[1]);
    ifstream inventoryFile;
    inventoryFile.open(argv[2]);
    ifstream bookingFile;
    bookingFile.open(argv[3]);
    ifstream passengerFile;
    passengerFile.open(argv[4]);
    
    ofstream fw(argv[5],ofstream::out);

    getScheduleInput(scheduleFile);
    getInventoryInput(inventoryFile);
    getBookingInput(bookingFile);
    getPassengerInput(passengerFile);

     //Graph Creation
    graphWUGenerator();
    graphUVAndGraphDVGenerator();
    graphUCAndGraphCVGenerator();
    graphWDGenerator();

    graphOutput(fw);

    fw.close();


}


