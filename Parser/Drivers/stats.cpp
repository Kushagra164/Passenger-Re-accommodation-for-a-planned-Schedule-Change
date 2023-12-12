#pragma once
#include<fstream>
#include<string>
#include<functional>
#include "../Input/schedule.h"
#include "../Input/inventory.h"
#include "../Input/booking.h"
#include "../Input/passenger.h"
#include "../Utils/graphOutput.h"
using namespace std;

// sch.csv, inv.csv, booking.csv, pass.csv, edg.txt, stats.txt
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

    // edges.txt
    graphWUGenerator();

    int totalAffectedJourneys = AffectedJourneys.size();  // |I|

    inputtream input(argv[5], inputtream::in);
    ofstream output(argv[6], ofstream::out);

    int noOfSamples;
    input>>noOfSamples;

    graphWUGenerator();
    graphUVAndGraphDVGenerator();
    set<pair<int,int>> edgesDV, edgesWU;
    for (int d = 0; d < graphDV.size(); d++){
        for (auto &v: graphDV[d]) edgesDV.insert(make_pair(d, v));
    }
    for (int w = 0; w < graphWU.size(); w++){
        for (auto &u: graphWU[w]) edgesWU.insert(make_pair(w, u));
    }

    for (int iter = 0; iter < noOfSamples; iter++){

        int solvedJourneys = 0;  // |S|

        set<pair<int, int>> edgesUC, edgesUV, edgesWD;
        Time totalDelay = Time(0, 0);

        output << "Solution " << iter + 1 << endl;

        int oneOne = 0;
        int oneMul = 0;
        int mulOne = 0;
        int mulMul = 0;

        int noOfUCEdges;
        input>>noOfUCEdges;

        solvedJourneys += noOfUCEdges;
        for(int dup = 0; dup < noOfUCEdges; dup++){
            int u,c;
            input >> u >> c;
            edgesUC.insert(make_pair(u, c));
            int journeyID = uIndexGenerator.getVal(u);
            journeyMap[journeyID]->flights.size() == 1 ? oneMul += 1: mulMul += 1;
            vector<int> flights;
            for(auto x:cIndexGenerator.getVal(c)) inventorys.push_back(x.first);
            totalDelay += getDepTimeDiff((journeyMap[j_id]->flights).front(),flights.front()) + getArrTimeDiff(journeyMap[j_id]->flights).back(),flights.back());
        }

        int noOfUVEdges; 
        input >> noOfUVEdges;
        
        solvedJourneys += noOfUVEdges;
        for(int dup = 0; dup < noOfUVEdges; dup++){
            int u, v;
            input >> u >> v;
            edgesUV.insert(make_pair(u, v));
            int journeyID = uIndexGenerator.getVal(u);
            journeyMap[journeyID]->flights.size() == 1 ? oneOne += 1: mulOne += 1;
            pair<int, CLASS_CD> pr = vIndexGenerator.getVal(v);
            totalDelay += getDepTimeDiff((journeyMap[j_id]->flights).front(),pr.first) + getArrTimeDiff((journeyMap[j_id]->flights).back(),pr.first);
        }

        int noOfWDEdges;
        input >> noOfWDEdges;
        solvedJourneys += noOfWDEdges;
        for (int dup = 0; dup < noOfWDEdges; dup++){
            int w,d;
            input >> w >> d;
            edgesWD.insert(make_pair(w, d));
        }

        int solvedDefault = 0;

        function<bool(int, int)> doit = [&](int d, int u)->bool{
            for(auto &wd: edgesWD){
                if (d == wd.second){
                    for(auto &wu: edgesWU){
                        if (wd.first == wu.first and wu.second == u) return true;
                    }
                }
            }
        };

        for (auto &uv: edgesUV){
            for (auto &dv: edgesDV){
                if (uv.second == dv.second and doit(dv.first,uv.first)){
                    solvedDefault += 1;
                    break;
                }
            }
        }

        output << "Percentage of Affected PNRs with Alternate Flight Solution: " << solvedJourneys * 100. / totalaffectedjourneys << "%" << endl;
        output << "Percentage of Solutions coming under default flight solution : " << solvedDefault * 100. / solvedJourneys << "%" << endl;
        output << "Average Delay in Solutions : " << totalDelay.value() * 50. / solvedJourneys << "%" << endl;

        output << "Percentage of One-One Solutions: " << oneOne * 100. / solvedJourneys << "%" << endl;
        output << "Percentage of One-Multi Solutions: " << oneMul * 100. / solvedJourneys << "%" << endl;
        output << "Percentage of Multi-One Solutions: " << mulOne * 100. / solvedJourneys << "%" << endl;
        output << "Percentage of Multi-Multi Solutions: " << mulMul * 100. / solvedJourneys << "%" << endl;
    }

}


