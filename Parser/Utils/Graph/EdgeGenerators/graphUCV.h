#pragma once
#include<set>
#include "../Helpers/helper.h"
using namespace std;

void graphUCAndGraphCVGenerator()
{
    // add UC edge 
    set<pair<int,int>> UCEdges;
    set<vector<pair<int, CLASS_CD>>> C;

    auto addUCEdge = [&](int curJourneyID, vector<pair<int,CLASS_CD>> curConnectingFlight, long long score){
        int curUIdx = uIndexGenerator.getIndex(curJourneyID);
        int curCIdx = cIndexGenerator.getIndex(curConnectingFlight);
        if(UCEdges.find(make_pair(curUIdx, curCIdx))!=UCEdges.end())
            return;
        UCEdges.insert(make_pair(curUIdx, curCIdx));
        Journey *curJourney = journeyMap[curJourneyID];

        string flightSrcCity = scheduleMap[inventoryToScheduleMap[curConnectingFlight[0].first]]->srcCity;
        string flightDestCity = scheduleMap[inventoryToScheduleMap[curConnectingFlight.back().first]]->destCity;

        if(curJourney->src!=flightSrcCity) return;
        if(curJourney->dest!=flightDestCity) return;

        int maxPaxCnt = allowedPassengers(curConnectingFlight);
        int curPaxCnt = pnrMap[journeyToPnrMap[curJourneyID]]->paxCnt;
        
        if(maxPaxCnt<curPaxCnt) return;
        
        Time curDiff = getDepTimeDiff(journeyMap[curJourneyID]->flights[0], curConnectingFlight[0].first) 
                        + getArrTimeDiff(journeyMap[curJourneyID]->flights.back(), curConnectingFlight.back().first);

        if(curDiff > (MAXIMUM_ALLOWED_TIME_DIFF*2))
            return;
        
        bool isClassNotAllowed = false;
        CLASS_CD journeyClassCD = journeyMap[curJourneyID]->classCD;
        for (auto [curFlight, curClassCD] : curConnectingFlight)
        {
            if (
                ((curClassCD < journeyClassCD) && !CLASS_UPGRADE_ALLOWED) ||
                ((curClassCD > journeyClassCD) && !CLASS_DOWNGRADE_ALLOWED))
            {
                isClassNotAllowed = true;
                break;
            }
        }

        if (isClassNotAllowed)
            return;

        C.insert(curConnectingFlight);
        graphUC[curUIdx].push_back(make_pair(curCIdx, score));
    };

    // hubs
    for (int curInventoryID : CancelledFlights)
    {
        int curScheduleID = inventoryToScheduleMap[curInventoryID];
        Schedule *curSchedule = scheduleMap[curScheduleID];
        auto srcCity = curSchedule->srcCity;
        auto destCity = curSchedule->destCity;

        vector<int> flightsFromSrc = findAllRelevantFlightsFromSrc(curInventoryID);
        vector<int> flightsToDest = findAllRelevantFlightsToDest(curInventoryID);

        vector<vector<int>> connectingFlights = getConnectingFlights(flightsFromSrc, flightsToDest);
        vector<pair<long long, vector<pair<int, CLASS_CD>>>> bestConnectingFlights = getBestConnectingFlights(curInventoryID, connectingFlights);

        for (auto [curScore, curConnectingFlight] : bestConnectingFlights)
        {
            for(int curJourneyID: AffectedJourneys){
                addUCEdge(curJourneyID, curConnectingFlight,
                    curScore * pnrScore(curJourneyID, journeyMap[curJourneyID]->classCD));
            }
        }
    }

    // Brute Via
    for (int curJourneyID : AffectedJourneys)
    {
        int curUIdx = uIndexGenerator.getIndex(curJourneyID);
        int curProposedFlights = graphUC[curUIdx].size() + graphUV[curUIdx].size();
        if (curProposedFlights >= MINIMUM_PROPOSED_FLIGHTS)
            continue;

        // Via flights
        string recLoc = pnrUuidGenerator.getString(journeyToPnrMap[curJourneyID]);
        Journey *curJourney = journeyMap[curJourneyID];
        vector<int> flightsFromSrc = findAllRelevantFlightsFromSrc(curJourney->flights[0]);
        vector<int> flightsToDest = findAllRelevantFlightsToDest(curJourney->flights.back());

        vector<vector<int>> connectingFlights = getConnectingFlights(flightsFromSrc, flightsToDest);
        vector<pair<long long, vector<pair<int, CLASS_CD>>>> bestConnectingFlights = 
                                        getBestConnectingFlightsForJourney(curJourneyID, connectingFlights);

        for (auto [curScore, curConnectingFlight] : bestConnectingFlights)
        {
            addUCEdge(curJourneyID, curConnectingFlight, curScore *
                        pnrScore(curJourneyID, journeyMap[curJourneyID]->classCD));
        }
    }

    // Adding CV edges
    for(auto curConnectingFlight: C){
        int curCIdx = cIndexGenerator.getIndex(curConnectingFlight);
        for (auto curFlight : curConnectingFlight){
            {
                int curVIdx = vIndexGenerator.getIndex(curFlight);
                graphCV[curCIdx].push_back(curVIdx);
            }
        }
    }
}