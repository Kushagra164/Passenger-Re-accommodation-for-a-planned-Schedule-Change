#pragma once
#include "../Helpers/helper.h"
using namespace std;

void graphUCAndGraphCVGenerator(){
    for(int curJourneyID: AffectedJourneys){
        vector<int> flightsFromSrc = findAllRelevantFlightsFromSrc(journeyMap[curJourneyID]->flights[0]);
        vector<int> flightsToDest = findAllRelevantFlightsToDest(journeyMap[curJourneyID]->flights[0]);
        vector<vector<int>> connectingFlights = getConnectingFlights(flightsFromSrc, flightsToDest);
        vector<pair<long long,vector<pair<int,CLASS_CD>>>> bestConnectingFlights 
                        = getBestConnectingFlights(curJourneyID, connectingFlights);

        int curUIdx = uIndexGenerator.getIndex(curJourneyID);

        for(auto [curScore, curFlights]: bestConnectingFlights){
            int curCIdx = cIndexGenerator.getIndex(curFlights);

            graphUC[curUIdx].push_back(make_pair(curCIdx, curScore));

            for(auto curFlight: curFlights){
                int curVIdx = vIndexGenerator.getIndex(curFlight);
                if(find(graphCV[curCIdx].begin(),graphCV[curCIdx].end(), curVIdx) 
                    == graphCV[curCIdx].end()){
                    graphCV[curCIdx].push_back(curVIdx);
                }
            }
        }
    }
}