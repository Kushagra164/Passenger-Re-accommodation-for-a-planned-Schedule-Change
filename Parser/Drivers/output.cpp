#include<fstream>
#include<string>
#include "../Input/schedule.h"
#include "../Input/inventory.h"
#include "../Input/booking.h"
#include "../Input/passenger.h"
#include "../Utils/graphOutput.h"
#include "../Output/inventory.h"
#include "../Output/booking.h"
#include "../Output/cancelledAndDelayedFlights.h"
#include "../Utils/uuidGenerator.h"
#include "../Utils/Graph/Helpers/graphIndexGenerator.h"

class Delay{
private:
    int lt6,lt12,lt18,lt24,mt24;
public:
    Delay():
            lt6(0),lt12(0),lt18(0),lt24(0),mt24(0){}

    void checkAndIncrement(double timeDelay){
        if(timeDelay <= 6) lt6++;
        else if(timeDelay <= 12) lt12++;
        else if(timeDelay <= 12) lt18++;
        else if(timeDelay <= 12) lt24++;
        else mt24++;
    }
    void display(ofstream& output){
        output<<lt6<<" ";
        output<<lt12<<" ";
        output<<lt18<<" ";
        output<<lt24<<" ";
        output<<mt24<<endl;
    }
};

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

    ifstream cancelledAndDelayedFlightsFile(argv[5]);

    getScheduleInput(scheduleFile);
    getInventoryInput(inventoryFile);
    getBookingInput(bookingFile);
    getPassengerInput(passengerFile);


    //Cancelled Flights set and Delayed Flights Map creation
    getCancelledAndDelayedFlightsInput(cancelledAndDelayedFlightsFile);


    //Graph Creation
    graphWUGenerator();
    graphUVAndGraphDVGenerator();
    graphUCAndGraphCVGenerator();
    graphWDGenerator();

    //Statistics Generation
    int totalAffectedJourneys = AffectedJourneys.size();
    set<pair<int,int>> edgesDV, edgesWU;
    for (int d = 0; d < graphDV.size(); d++){
        for (auto &v: graphDV[d]) edgesDV.insert(make_pair(d, v));
    }
    for (int w = 0; w < graphWU.size(); w++){
        for (auto &u: graphWU[w]) edgesWU.insert(make_pair(w, u));
    }

    //Output File Generation
    ifstream input(argv[6]);

    string outputFilePath = argv[7];
    ofstream output(outputFilePath, ofstream::out);

    //For Output
    map<int,vector<pair<int,CLASS_CD>>> journeyToConnectingMap;
    map<int,pair<int,CLASS_CD>> journeyToFlightMap;
    map<int,int> cancelledFlightToSolutionFlightMap;

    //For Statistics
    int solvedJourneys = 0;
    set<pair<int, int>> edgesUC, edgesUV, edgesWD;
    vector<bool> visitedJourney(uIndexGenerator.getSize(),false);
    Delay delay;
    int oneOne = 0;
    int oneMul = 0;
    int mulOne = 0;
    int mulMul = 0;

    int noOfUVEdges;
    input>>noOfUVEdges;

    for(int j=0;j<noOfUVEdges;j++){
        int u,v;
        input>>u>>v;

        if(visitedJourney[u]) continue;
        else visitedJourney[u]=true;

        int journeyID = uIndexGenerator.getVal(u);
        journeyToFlightMap[journeyID]=vIndexGenerator.getVal(v);

        solvedJourneys++;

        Time totalDelay = Time(0, 0);
        edgesUV.insert(make_pair(u, v));
        journeyMap[journeyID]->flights.size() == 1 ? oneOne += 1: mulOne += 1;
        pair<int, CLASS_CD> pr = vIndexGenerator.getVal(v);
        totalDelay += getDepTimeDiff((journeyMap[journeyID]->flights).front(),pr.first) + getArrTimeDiff((journeyMap[journeyID]->flights).back(),pr.first);
        delay.checkAndIncrement(totalDelay.value()/2);
    }

    int noOfUCEdges;
    input>>noOfUCEdges;

    for(int j=0;j<noOfUCEdges;j++){
        int u,c;
        input>>u>>c;

        if(visitedJourney[u]) continue;
        else visitedJourney[u]=true;

        int journeyID = uIndexGenerator.getVal(u);
        journeyToConnectingMap[journeyID]=cIndexGenerator.getVal(c);

        solvedJourneys++;

        Time totalDelay = Time(0, 0);
        edgesUC.insert(make_pair(u, c));
        journeyMap[journeyID]->flights.size() == 1 ? oneMul += 1: mulMul += 1;
        vector<int> curFlights;
        for(auto x:cIndexGenerator.getVal(c)) curFlights.push_back(x.first);
        totalDelay += getDepTimeDiff((journeyMap[journeyID]->flights).front(),curFlights.front()) + getArrTimeDiff((journeyMap[journeyID]->flights).back(),curFlights.back());
        delay.checkAndIncrement(totalDelay.value()/2);
    }

    int noOfWDEdges;
    input>>noOfWDEdges;
    for (int i=0;i<noOfWDEdges;i++){
        int w,d;
        input>>w>>d;
        int cancelledFlightInventoryID = wIndexGenerator.getVal(w);
        int solutionFlightInventoryID = dIndexGenerator.getVal(d);

        cancelledFlightToSolutionFlightMap[cancelledFlightInventoryID]=solutionFlightInventoryID;

        edgesWD.insert(make_pair(w, d));
    }

    int solvedDefault = 0;

    function<bool(int, int)> checkDefaultSolution = [&](int d, int u)->bool{
        for(auto &wd: edgesWD){
            if (d == wd.second){
                for(auto &wu: edgesWU){
                    if (wd.first == wu.first and wu.second == u) return true;
                }
            }
        }
        return false;
    };

    for (auto &uv: edgesUV){
        for (auto &dv: edgesDV){
            if (uv.second == dv.second and checkDefaultSolution(dv.first,uv.first)){
                solvedDefault += 1;
                break;
            }
        }
    }

    getInventoryOutput(output,cancelledFlightToSolutionFlightMap);
    output<<"break\n";
    getBookingOutput(output,journeyToConnectingMap,journeyToFlightMap);
    output<<"break\n";

    output << oneOne << " ";
    output << oneMul << " ";
    output << mulOne << " ";
    output << mulMul << endl;

    output << solvedDefault  << " ";
    output << solvedJourneys - solvedDefault << " ";
    output << totalAffectedJourneys - solvedJourneys << endl;

    delay.display(output);

}