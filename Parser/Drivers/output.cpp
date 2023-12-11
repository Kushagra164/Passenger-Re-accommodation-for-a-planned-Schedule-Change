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

    getScheduleInput(scheduleFile);
    getInventoryInput(inventoryFile);
    getBookingInput(bookingFile);
    getPassengerInput(passengerFile);

    //Graph Creation
    graphWUGenerator();
    graphUVAndGraphDVGenerator();
    graphUCAndGraphCVGenerator();
    graphWDGenerator();

    //Output File Generation
    freopen( argv[5], "r", stdin);

    int noOfSolutions;
    cin>>noOfSolutions;

    for(int i=0;i<noOfSolutions;i++){
        map<int,vector<pair<int,CLASS_CD>>> journeyToConnectingMap;
        map<int,pair<int,CLASS_CD>> journeyToFlightMap;
        map<int,int> cancelledFlightToSolutionFlightMap;

        int noOfUCEdges;
        cin>>noOfUCEdges;
        for(int j=0;j<noOfUCEdges;j++){
            int u,c;
            cin>>u>>c;

            int journeyID = uIndexGenerator.getID(u);
            journeyToConnectingMap[journeyID]=cIndexGenerator.getVal(c);
        }

        int noOfUVEdges;
        cin>>noOfUVEdges;
        for(int j=0;j<noOfUVEdges;j++){
            int u,v;
            cin>>u>>v;

            int journeyID = uIndexGenerator.getID(u);

            journeyToFlightMap[journeyID]=vIndexGenerator.getVal(v);

        }

        int noOfWDEdges;
        cin>>noOfWDEdges;
        for (int i=0;i<noOfWDEdges;i++){
            int w,d;
            cin>>w>>d;
            int cancelledFlightInventoryID = wIndexGenerator.getID(w);
            int solutionFlightInventoryID = dIndexGenerator.getID(d);

            cancelledFlightToSolutionFlightMap[cancelledFlightInventoryID]=solutionFlightInventoryID;
        }

        ofstream schedule(argv[5+3*i+1],ofstream::out);
        getScheduleOutput(schedule);

        ofstream inventory(argv[5+3*i+2],ofstream::out);
        getInventoryOutput(inventory,cancelledFlightToSolutionFlightMap);

        ofstream booking(argv[5+3*i+3],ofstream::out);
        getBookingOutput(booking,journeyToConnectingMap,journeyToFlightMap);
    }

}