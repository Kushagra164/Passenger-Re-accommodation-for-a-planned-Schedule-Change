#pragma once
#include <functional>
#include <queue>
#include <set>
#include "scoring.h"
#include "validity.h"
using namespace std;

vector<int> findAllRelevantFlightsFromSrc(int originalInventoryID)
{
    int originalScheduleID = inventoryToScheduleMap[originalInventoryID];
    Schedule *originalSchedule = scheduleMap[originalScheduleID];

    vector<int> result;

    auto srcCity = originalSchedule->srcCity;
    auto destCity = originalSchedule->destCity;

    for (auto [curInventoryID, curInventory] : inventoryMap)
    {
        Schedule *curSchedule = scheduleMap[inventoryToScheduleMap[curInventoryID]];
        if ((CancelledFlights.find(curInventoryID) == CancelledFlights.end()) && (curSchedule->srcCity == srcCity))
        {

            Time ArrivalTimeDiff = getDepTimeDiff(curInventoryID, originalInventoryID);
            if (ArrivalTimeDiff <= MAXIMUM_ALLOWED_TIME_DIFF)
            {
                result.push_back(curInventoryID);
            }
        }
    }

    return result;
}

vector<int> findAllRelevantFlightsToDest(int originalInventoryID)
{
    int originalScheduleID = inventoryToScheduleMap[originalInventoryID];
    Schedule *originalSchedule = scheduleMap[originalScheduleID];

    vector<int> result;

    auto srcCity = originalSchedule->srcCity;
    auto destCity = originalSchedule->destCity;

    for (auto [curInventoryID, curInventory] : inventoryMap)
    {
        Schedule *curSchedule = scheduleMap[inventoryToScheduleMap[curInventoryID]];
        if ((CancelledFlights.find(curInventoryID) == CancelledFlights.end()) && (curSchedule->destCity == destCity))
        {

            Time DepartureTimeDiff = getArrTimeDiff(curInventoryID, originalInventoryID);
            if (DepartureTimeDiff <= MAXIMUM_ALLOWED_TIME_DIFF)
            {
                result.push_back(curInventoryID);
            }
        }
    }

    return result;
}

bool canConnect(int fromInventoryID, int toInventoryID){
    if(CancelledFlights.find(fromInventoryID)!=CancelledFlights.end()) return false;
    if(CancelledFlights.find(toInventoryID)!=CancelledFlights.end()) return false;
    string fromDestCity = scheduleMap[inventoryToScheduleMap[fromInventoryID]]->destCity;
    string toSrcCity = scheduleMap[inventoryToScheduleMap[toInventoryID]]->srcCity;
    Time curDif = getArrDepTimeDiff(fromInventoryID, toInventoryID);
    return (curDif >= MINIMUM_CONNECTING_TIME) 
            && (curDif <= MAXIMUM_ALLOWED_TIME_DIFF_FOR_CONNECTING)
            && (toSrcCity == fromDestCity);
}

// return relevant journey
vector<vector<int>> getConnectingFlights(vector<int> &fromSrc, vector<int> &toDest)
{
    vector<vector<int>> connectingFlights;

    for (int srcFlight : fromSrc)
    {
        for (int destFlight : toDest)
        {
            if (canConnect(srcFlight, destFlight))
            {
                connectingFlights.push_back({srcFlight, destFlight});
            }
        }
    }
    return connectingFlights;
}

// populates graph
void createConnectingFlightGraph(vector<vector<int>> &flightAdj){
    for(auto [srcInventoryID, srcInventory]: inventoryMap){
        for(auto [destInventoryID, destInventory]: inventoryMap){
            if(canConnect(srcInventoryID, destInventoryID)){
                flightAdj[srcInventoryID].push_back(destInventoryID);
            }
        }
    }
}

int allowedPassengers(vector<pair<int, CLASS_CD>> flight)
{
    auto bottleNeck = min_element(flight.begin(), flight.end(), [&](auto a, auto b)
                                  { return getPassengers(a.first, a.second) < getPassengers(b.first, b.second); });
    int flightID = bottleNeck->first;
    CLASS_CD curClass = bottleNeck->second;
    return getPassengers(flightID, curClass);
}

vector<pair<long long, vector<pair<int, CLASS_CD>>>> getBestConnectingFlights(int cancelledInventoryID, vector<vector<int>> proposedFlights)
{

    vector<pair<long long, vector<pair<int, CLASS_CD>>>> allConnectingFlightsWithScore;

    for (auto curFlight : proposedFlights)
    {
        vector<pair<int, CLASS_CD>> curProposedFlight;
        function<void(int)> generateConnectingFlightWithClass = [&](int index)
        {
            if (index == curFlight.size())
            {
                allConnectingFlightsWithScore.push_back(
                    make_pair(
                        getFinalConnectingFlightScore(cancelledInventoryID, curProposedFlight),
                        curProposedFlight));
            }
            else
            {
                for (int toClassCD = 0; toClassCD < 4; toClassCD++)
                {
                    curProposedFlight.push_back(make_pair(curFlight[index],
                                                          static_cast<CLASS_CD>(toClassCD)));
                    generateConnectingFlightWithClass(index + 1);
                    curProposedFlight.pop_back();
                }
            }
        };
        generateConnectingFlightWithClass(0);
    }

    auto bestConnectingFlightsWithScore = allConnectingFlightsWithScore;
    sort(bestConnectingFlightsWithScore.begin(), bestConnectingFlightsWithScore.end(), [&](auto a, auto b)
         { return (a.first * allowedPassengers(a.second)) > (b.first * allowedPassengers(b.second)); });
    bestConnectingFlightsWithScore.resize(
        min(
            (int)MAXIMUM_ALLOWED_CONNECTING_FLIGHTS_PER_JOURNEY,
            (int)bestConnectingFlightsWithScore.size()));

    return bestConnectingFlightsWithScore;
}

vector<pair<long long, vector<pair<int, CLASS_CD>>>> getBestConnectingFlightsForJourney(int affectedJourneyID, vector<vector<int>> proposedFlights)
{

    vector<pair<long long, vector<pair<int, CLASS_CD>>>> allConnectingFlightsWithScore;

    Journey *curJounrey = journeyMap[affectedJourneyID];
    CLASS_CD curClassCD = curJounrey->classCD;

    for (auto curFlight : proposedFlights)
    {
        vector<pair<int, CLASS_CD>> curProposedFlight;
        function<void(int)> generateConnectingFlightWithClass = [&](int index)
        {
            if (index == curFlight.size())
            {
                allConnectingFlightsWithScore.push_back(
                    make_pair(
                        getFinalConnectingFlightScoreForJourney(affectedJourneyID,
                                                                curProposedFlight),
                        curProposedFlight));
            }
            else
            {
                for (int toClassCD = 0; toClassCD < 4; toClassCD++)
                {
                    if ((toClassCD < curClassCD) && (!CLASS_UPGRADE_ALLOWED))
                        continue;
                    if ((toClassCD > curClassCD) && (!CLASS_DOWNGRADE_ALLOWED))
                        continue;
                    curProposedFlight.push_back(make_pair(curFlight[index],
                                                          static_cast<CLASS_CD>(toClassCD)));
                    generateConnectingFlightWithClass(index + 1);
                    curProposedFlight.pop_back();
                }
            }
        };
        generateConnectingFlightWithClass(0);
    }

    auto bestConnectingFlightsWithScore = allConnectingFlightsWithScore;
    sort(bestConnectingFlightsWithScore.begin(), bestConnectingFlightsWithScore.end(), [&](auto a, auto b)
         { return (a.first * allowedPassengers(a.second)) > (b.first * allowedPassengers(b.second)); });
    bestConnectingFlightsWithScore.resize(
        min(
            (int)MAXIMUM_ALLOWED_CONNECTING_FLIGHTS_PER_JOURNEY,
            (int)bestConnectingFlightsWithScore.size()));

    return bestConnectingFlightsWithScore;
}

vector<pair<int, CLASS_CD>> multiSourceBFS(
    vector<vector<int>> &flightAdj,
    vector<int> flightFromSrc, vector<int> flightsToDest, 
    int curPaxCnt, CLASS_CD curClassCD){
        map<pair<int,CLASS_CD>,pair<int,CLASS_CD>> parent;
        queue<pair<int,CLASS_CD>> flights;
        set<pair<int,CLASS_CD>> srcGoal, destGoal;
        for(auto start: flightFromSrc){
            for(int toClassCD = 0; toClassCD<4; toClassCD++){
                if((toClassCD<curClassCD)&&(!CLASS_UPGRADE_ALLOWED))continue;
                if((toClassCD>curClassCD)&&(!CLASS_DOWNGRADE_ALLOWED))continue;
                CLASS_CD classCD = static_cast<CLASS_CD>(toClassCD);
                int maxPaxCnt = getPassengers(start, classCD);
                if( maxPaxCnt<curPaxCnt)
                    continue;
                
                parent[make_pair(start, classCD)] = make_pair(-1, FC);
                flights.push(make_pair(start, classCD));
                srcGoal.insert(make_pair(start, classCD));
            }
        }
        for(auto dest: flightsToDest){
            for(int toClassCD = 0; toClassCD<4; toClassCD++){
                if((toClassCD<curClassCD)&&(!CLASS_UPGRADE_ALLOWED))continue;
                if((toClassCD>curClassCD)&&(!CLASS_DOWNGRADE_ALLOWED))continue;
                CLASS_CD classCD = static_cast<CLASS_CD>(toClassCD);
                int maxPaxCnt = getPassengers(dest, classCD);
                if( maxPaxCnt<curPaxCnt)
                    continue;
                destGoal.insert(make_pair(dest, classCD));
            }
        }
        while(!flights.empty()){
            auto [flight, classCD] = flights.front();
            flights.pop();
            if(destGoal.find(make_pair(flight, classCD))!=destGoal.end()){
                pair<int, CLASS_CD> cur = make_pair(flight, classCD);
                vector<pair<int, CLASS_CD>> connectingFlight;
                while(cur.first!=-1){
                    connectingFlight.push_back(cur);
                    cur = parent[cur];
                }
                reverse(connectingFlight.begin(), connectingFlight.end());
                return connectingFlight;
            }
            for(int toFlight: flightAdj[flight]){
                for(int toClassCD = 0; toClassCD<4; toClassCD++){
                    if((toClassCD<curClassCD)&&(!CLASS_UPGRADE_ALLOWED))continue;
                    if((toClassCD>curClassCD)&&(!CLASS_DOWNGRADE_ALLOWED))continue;
                    CLASS_CD castClassCD = static_cast<CLASS_CD>(toClassCD);
                    if(parent.find(make_pair(toFlight, castClassCD))!=parent.end()) continue;
                    int maxPaxCnt = getPassengers(toFlight, castClassCD);
                    if(maxPaxCnt<curPaxCnt)
                        continue;
                    parent[make_pair(toFlight, castClassCD)] = make_pair(flight, classCD);
                    flights.push(make_pair(toFlight, castClassCD));
                }
            }
        }
        return vector<pair<int, CLASS_CD>> ();
}