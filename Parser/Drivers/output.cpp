#include<fstream>
#include<string>
#include "../Input/schedule.h"
#include "../Input/inventory.h"
#include "../Input/booking.h"
#include "../Input/passenger.h"
#include "../Utils/graphOutput.h"
#include "../Output/schedule.h"
#include "../Output/inventory.h"
#include "../Output/booking.h"
#include "../Utils/uuidGenerator.h"
#include "../Utils/Graph/Helpers/graphIndexGenerator.h"


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
    ifstream input(argv[5]);

    int noOfSolutions;
    input>>noOfSolutions;

    string outputFilePath = argv[6];
    ofstream output(outputFilePath, ofstream::out);

    output<<noOfSolutions<<"\n";
    for(int i=0;i<noOfSolutions;i++){
        map<int,vector<pair<int,CLASS_CD>>> journeyToConnectingMap;
        map<int,pair<int,CLASS_CD>> journeyToFlightMap;
        map<int,int> cancelledFlightToSolutionFlightMap;

        int noOfUCEdges;
        input>>noOfUCEdges;
        for(int j=0;j<noOfUCEdges;j++){
            int u,c;
            input>>u>>c;

            int journeyID = uIndexGenerator.getVal(u);
            journeyToConnectingMap[journeyID]=cIndexGenerator.getVal(c);
        }

        int noOfUVEdges;
        input>>noOfUVEdges;
        for(int j=0;j<noOfUVEdges;j++){
            int u,v;
            input>>u>>v;

            int journeyID = uIndexGenerator.getVal(u);

            journeyToFlightMap[journeyID]=vIndexGenerator.getVal(v);

        }

        int noOfWDEdges;
        input>>noOfWDEdges;
        for (int i=0;i<noOfWDEdges;i++){
            int w,d;
            input>>w>>d;
            int cancelledFlightInventoryID = wIndexGenerator.getVal(w);
            int solutionFlightInventoryID = dIndexGenerator.getVal(d);

            cancelledFlightToSolutionFlightMap[cancelledFlightInventoryID]=solutionFlightInventoryID;
        }
        
        getScheduleOutput(output);
        output<<"break\n";
        getInventoryOutput(output,cancelledFlightToSolutionFlightMap);
        output<<"break\n";
        getBookingOutput(output,journeyToConnectingMap,journeyToFlightMap);
        output<<"break\n";
    }

}