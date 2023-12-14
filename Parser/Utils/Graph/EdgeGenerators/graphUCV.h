#pragma once
#include "../Helpers/helper.h"
using namespace std;

void graphUCAndGraphCVGenerator(){
    for(int curInventoryID: CancelledFlights){
        int curScheduleID = inventoryToScheduleMap[curInventoryID];
        Schedule* curSchedule = scheduleMap[curScheduleID];
        auto srcCity = curSchedule->srcCity;
        auto destCity = curSchedule->destCity;

        vector<int> flightsFromSrc = findAllRelevantFlightsFromSrc(curInventoryID);
        vector<int> flightsToDest = findAllRelevantFlightsToDest(curInventoryID);

        vector<vector<int>> connectingFlights = getConnectingFlights(flightsFromSrc, flightsToDest);
        vector<pair<long long,vector<pair<int,CLASS_CD>>>> bestConnectingFlights 
                        = getBestConnectingFlights(curInventoryID, connectingFlights);

        for(auto [curScore, curFlights]: bestConnectingFlights){
            int curCIdx = cIndexGenerator.getIndex(curFlights);
            if(curCIdx != (cIndexGenerator.getSize()-1)) continue;
            int maxPaxCnt = allowedPassengers(curFlights);

            // UC
            for(int curJourneyID: AffectedJourneys){
                if(journeyMap[curJourneyID]->src != srcCity) continue;
                if(journeyMap[curJourneyID]->dest != destCity) continue;
                if(maxPaxCnt < pnrMap[journeyToPnrMap[curJourneyID]]->paxCnt) continue;
                Time curDiff = getDepTimeDiff(journeyMap[curJourneyID]->flights[0], curFlights[0].first)
                                +getArrTimeDiff(journeyMap[curJourneyID]->flights.back(), curFlights.back().first);
                if(curDiff>(MAXIMUM_ALLOWED_TIME_DIFF*2)) continue;
                
                bool isClassNotAllowed = false;
                CLASS_CD journeyClassCD = journeyMap[curJourneyID]->classCD;
                for(auto [curFlight, curClassCD]: curFlights){
                    if(
                        ((curClassCD<journeyClassCD)&& !CLASS_UPGRADE_ALLOWED)||
                        ((curClassCD>journeyClassCD)&& !CLASS_DOWNGRADE_ALLOWED)
                    ){
                        isClassNotAllowed = true;
                        break;
                    }
                }
                
                if(isClassNotAllowed) continue;

                int curUIdx = uIndexGenerator.getIndex(curJourneyID);
                
                graphUC[curUIdx].push_back(make_pair(curCIdx, curScore*pnrScore(curJourneyID, journeyMap[curJourneyID]->classCD)));
            }

            for(auto curFlight: curFlights){
                int curVIdx = vIndexGenerator.getIndex(curFlight);
                graphCV[curCIdx].push_back(curVIdx);
            }
        }
    }
}