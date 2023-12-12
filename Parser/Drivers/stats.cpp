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
            output<<mt24<<endl;;
        }    
};

int main(int argc,char* argv[]) {

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

    graphWUGenerator();

    int totalAffectedJourneys = AffectedJourneys.size();

    graphUVAndGraphDVGenerator();
    set<pair<int,int>> edgesDV, edgesWU;
    for (int d = 0; d < graphDV.size(); d++){
        for (auto &v: graphDV[d]) edgesDV.insert(make_pair(d, v));
    }
    for (int w = 0; w < graphWU.size(); w++){
        for (auto &u: graphWU[w]) edgesWU.insert(make_pair(w, u));
    }

    ifstream input(argv[5], ifstream::in);
    ofstream output(argv[6], ofstream::out);

    int noOfSamples;
    input>>noOfSamples;

    for (int iter = 0; iter < noOfSamples; iter++){

        int solvedJourneys = 0;

        set<pair<int, int>> edgesUC, edgesUV, edgesWD;

        Delay delay;

        int oneOne = 0;
        int oneMul = 0;
        int mulOne = 0;
        int mulMul = 0;

        int noOfUCEdges;
        input>>noOfUCEdges;

        solvedJourneys += noOfUCEdges;
        for(int dup = 0; dup < noOfUCEdges; dup++){
            int u,c;
            Time totalDelay = Time(0, 0);
            input >> u >> c;
            edgesUC.insert(make_pair(u, c));
            int journeyID = uIndexGenerator.getVal(u);
            journeyMap[journeyID]->flights.size() == 1 ? oneMul += 1: mulMul += 1;
            vector<int> flights;
            for(auto x:cIndexGenerator.getVal(c)) flights.push_back(x.first);
            totalDelay += getDepTimeDiff((journeyMap[journeyID]->flights).front(),flights.front()) + getArrTimeDiff((journeyMap[journeyID]->flights).back(),flights.back());
            delay.checkAndIncrement(totalDelay.value()/2);
        }
        
        

        int noOfUVEdges; 
        input >> noOfUVEdges;
        
        solvedJourneys += noOfUVEdges;
        for(int dup = 0; dup < noOfUVEdges; dup++){
            int u, v;
            Time totalDelay = Time(0, 0);
            input >> u >> v;
            edgesUV.insert(make_pair(u, v));
            int journeyID = uIndexGenerator.getVal(u);
            journeyMap[journeyID]->flights.size() == 1 ? oneOne += 1: mulOne += 1;
            pair<int, CLASS_CD> pr = vIndexGenerator.getVal(v);
            totalDelay += getDepTimeDiff((journeyMap[journeyID]->flights).front(),pr.first) + getArrTimeDiff((journeyMap[journeyID]->flights).back(),pr.first);
            delay.checkAndIncrement(totalDelay.value()/2);
        }

        int noOfWDEdges;
        input >> noOfWDEdges;
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

        output << oneOne << " ";
        output << oneMul << " ";
        output << mulOne << " ";
        output << mulMul << endl;

        output << solvedDefault  << " ";
        output << solvedJourneys - solvedDefault << " ";
        output << totalAffectedJourneys - solvedJourneys << endl;
        
        delay.display();
    }

}


