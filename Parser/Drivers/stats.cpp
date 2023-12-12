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

void createAffectedJourneys(){
    for(auto [curJourneyID, curJourney]:journeyMap){

        if((curJourney->flights.size()==1) && (CancelledFlights.find(curJourney->flights[0])!=CancelledFlights.end())){
            // cancelled single flight
            AffectedJourneys.push_back(curJourneyID);
        }
        else{
            //Single journey has multiple flights
            for(int i=0;i<curJourney->flights.size();i++){
                bool cond1 = false;
                if(DelayedFlights.find(curJourney->flights[i])!=DelayedFlights.end()) cond1 = (DelayedFlights[curJourney->flights[i]] > MAXIMUM_ALLOWED_TIME_DIFF);
                // if flight delay is more than 72 hours


                bool cond2 = false;

                if((i+1)<curJourney->flights.size())
                    cond2 = (getArrDepTimeDiff(curJourney->flights[i],curJourney->flights[i+1]) < MINIMUM_CONNECTING_TIME);
                //if time difference between 2 consecutive flights after delay taken into consideration is less than 1hr

                if(cond1 || cond2){
                    AffectedJourneys.push_back(curJourneyID);
                    break;
                }

            }
        }
    }
}

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
    createAffectedjourneys();

    int totalaffectedjourneys = AffectedJourneys.size();  // |I|

    ifstream ifs(argv[5], ifstream::in);
    freopen(argv[6], "w", stdout);

    int no_of_samples;
    ifs >> no_of_samples;

    graphWUGenerator();
    graphUVAndGraphDVGenerator();
    set<pair<int, int>> VDedges, WUedges;
    for (int d = 0; d < graphDV.size(); d++){
        for (auto &v: graphDV[d]) VDedges.insert(make_pair(v, d));
    }
    for (int w = 0; w < graphWU.size(); w++){
        for (auto &u: graphWU[w]) WUedges.insert(make_pair(w, u));
    }

    for (int iter = 0; iter < no_of_samples; iter++){

        int solvedjourneys = 0;  // |S|

        set<pair<int, int>> UCedges, UVedges, DWedges;
        Time totaldelay = Time(0, 0);

        cout << "Solution " << iter + 1 << endl;

        int oneone = 0;
        int onemul = 0;
        int mulone = 0;
        int mulmul = 0;

        int no_of_uc_edges; ifs >> no_of_uc_edges;
        solvedjourneys += no_of_uc_edges;
        for(int dup = 0; dup < no_of_uc_edges; dup++){
            int u,c;
            ifs >> u >> c;
            UCedges.insert(make_pair(u, c));
            int j_id = uIndexGenerator.getVal(u);
            int o_inv_id = journeyMap[j_id]->flights[0];
            journeyMap[j_id]->flights.size() == 1 ? onemul += 1: mulmul += 1;
            vector<int> inventorys;
            for(auto x:cIndexGenerator.getVal(c)) inventorys.push_back(x.first);
            totaldelay += getDepTimeDiff(o_inv_id, inventorys.front()) + getArrDepTimeDiff(o_inv_id, inventorys.back());
        }

        int no_of_uv_edges; ifs >> no_of_uv_edges;
        solvedjourneys += no_of_uv_edges;
        for(int dup = 0; dup < no_of_uv_edges; dup++){
            int u, v;
            ifs >> u >> v;
            UVedges.insert(make_pair(u, v));
            int j_id = uIndexGenerator.getVal(u);
            journeyMap[j_id]->flights.size() == 1 ? oneone += 1: mulone += 1;
            int o_inv_id = journeyMap[j_id]->flights[0];
            pair<int, CLASS_CD> pr = vIndexGenerator.getVal(v);
            totaldelay += getDepTimeDiff(o_inv_id, pr.first) + getArrDepTimeDiff(o_inv_id, pr.first);
        }

        int no_of_wd_edges; ifs >> no_of_wd_edges;
        solvedjourneys += no_of_wd_edges;
        for (int dup = 0; dup < no_of_wd_edges; dup++){
            int w, d;
            ifs >> w >> d;
            DWedges.insert(make_pair(w, d));
        }

        int solvedDefault = 0;

        function<bool(int, int)> doit = [&](int d, int u)->bool{
            for(auto &dw: DWedges){
                if (d == dw.first){
                    for(auto &wu: WUedges){
                        if (dw.second == wu.first and wu.second == u) return true;
                    }
                }
            }
        };

        for (auto &uv: UVedges){
            for (auto &vd: VDedges){
                if (uv.second == vd.first and doit(vd.second, uv.first)){
                    solvedDefault += 1;
                    break;
                }
            }
        }

        cout << "Percent of Successful / Impacted journeys: " << solvedjourneys * 100. / totalaffectedjourneys << "%" << endl;
        cout << "Percentage of Successful journeys with default soln: " << solvedDefault * 100. / solvedjourneys << "%" << endl;
        cout << "Delay in successful journeys: " << totaldelay.value() * 50. / solvedjourneys << "%" << endl;

        cout << "Percentage in one one: " << oneone * 100. / solvedjourneys << "%" << endl;
        cout << "Percentage in one mul: " << onemul * 100. / solvedjourneys << "%" << endl;
        cout << "Percentage in mul one: " << mulone * 100. / solvedjourneys << "%" << endl;
        cout << "Percentage in mul mul: " << mulmul * 100. / solvedjourneys << "%" << endl;
    }

}

/*
        int numberofjourneysswithoutsolution = totalaffectedjourneys - solvedjourneys;
        double Pnumberofjourneysswithoutsolution = (numberofPNRswithoutsolution * 100.) / totaljourneys;
        double Pnumberofsolnwithexception = (solvedjourneys * 100.) / totaljourneys;
        cout << "Percentage of PNRs without any solution: " << PnumberofPNRswithoutsolution << endl;
        cout << "Percentage of PNRs stayed in default flight: " << 100. - (totalaffectedjourneys * 100. / totaljourneys) << endl;
        cout << "Percentage of PNRs exception (got solution in other flight / connectting flight): " << Pnumberofsolnwithexception << endl;
        double totaldelaytimeofdelayedjourneys = totaldelay.value() / 2.;
        cout << "Average delay of delayed journeys: " << totaldelaytimeofdelayedjourneys / solvedjourneys << endl;
        cout << "Average delay of total successful journeys: " << totaldelaytimeofdelayedjourneys / (totaljourneys - numberofPNRswithoutsolution) << endl;

        int numberofjourneysswithoutsolution = totalaffectedjourneys - solvedjourneys;
        double totaldelaytimeofdelayedjourneys = totaldelay.value() / 2.;
        cout << "Average delay of delayed journeys: " << totaldelaytimeofdelayedjourneys / solvedjourneys << endl;
        cout << "Average delay of total successful journeys: " << totaldelaytimeofdelayedjourneys / (totaljourneys - numberofjourneysswithoutsolution) << endl;


        cout << "Percentage of PNRs without any solution: " << unsolvedPNRs * 100. / pnrMap.size() << "%" << endl;
        cout << "Percentage of PNRs with orginal solution: " << UneffectedPNRs * 100. / pnrMap.size() << "%" << endl;
        cout << "Percentage of PNRs with exception soln: " << solvedPNRs * 100. / pnrMap.size() << "%" << endl;
        cout << "One-One: " << oneone_in_affectedPNRs * 100. / solvedPNRs << "%" << endl;
        cout << "One-Mul: " << onemul_in_affcetedPNRs * 100. / solvedPNRs << "%" << endl;
        cout << "Mul-One: " << mulone_in_affectedPNRs * 100. / solvedPNRs << "%" << endl;
        cout << "Mul-Mul: " << mulmul_in_affectedPNrs * 100. / solvedPNRs << "%" << endl;
*/


