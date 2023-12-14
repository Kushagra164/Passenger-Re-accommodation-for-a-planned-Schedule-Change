#pragma once
#include "scoring.h"
#include "validity.h"
using namespace std;

vector<int> findAllRelevantFlightsFromSrc(int originalInventoryID){
    int originalScheduleID = inventoryToScheduleMap[originalInventoryID];
    Schedule* originalSchedule = scheduleMap[originalScheduleID];

    vector<int> result;

    auto srcCity = originalSchedule->srcCity;
    auto destCity = originalSchedule->destCity;

    for(auto [curInventoryID, curInventory]: inventoryMap){
        if (curInventoryID == originalInventoryID) continue;
        Schedule* curSchedule = scheduleMap[inventoryToScheduleMap[curInventoryID]];
        if ((CancelledFlights.find(curInventoryID)==CancelledFlights.end()) 
                && (curSchedule->srcCity == srcCity) 
                && (curSchedule->destCity != destCity)){
            
            Time ArrivalTimeDiff = getArrTimeDiff(curInventoryID, originalInventoryID);
            if (ArrivalTimeDiff <= MAXIMUM_ALLOWED_TIME_DIFF){
                result.push_back(curInventoryID);
            }
        }
    }

    return result;
}

vector<int> findAllRelevantFlightsToDest(int originalInventoryID){
    int originalScheduleID = inventoryToScheduleMap[originalInventoryID];
    Schedule* originalSchedule = scheduleMap[originalScheduleID];

    vector<int> result;

    auto srcCity = originalSchedule->srcCity;
    auto destCity = originalSchedule->destCity;

    for(auto [curInventoryID, curInventory]: inventoryMap){
        if (curInventoryID == originalInventoryID) continue;
        Schedule* curSchedule = scheduleMap[inventoryToScheduleMap[curInventoryID]];
        if ((CancelledFlights.find(curInventoryID)==CancelledFlights.end()) 
                && (curSchedule->srcCity != srcCity) 
                && (curSchedule->destCity == destCity)){

            Time DepartureTimeDiff = getDepTimeDiff(curInventoryID, originalInventoryID);
            if (DepartureTimeDiff <= MAXIMUM_ALLOWED_TIME_DIFF){
                result.push_back(curInventoryID);
            }
        }
    }

    return result;
}

// return relevant journey
vector<vector<int>> getConnectingFlights(vector<int> &fromSrc, vector<int> &toDest){
    vector<vector<int>> connectingFlights;

    for(int srcFlight: fromSrc){
        Schedule* srcSchedule = scheduleMap[inventoryToScheduleMap[srcFlight]];
        for(int destFlight: toDest){
            Schedule* destSchedule = scheduleMap[inventoryToScheduleMap[destFlight]];
            if((srcSchedule->destCity == destSchedule->srcCity) &&
                    (getArrDepTimeDiff(srcFlight, destFlight)) >= MINIMUM_CONNECTING_TIME){
                connectingFlights.push_back({srcFlight,destFlight});
            }
        }
    }
    return connectingFlights;
}

int allowedPassengers(vector<pair<int,CLASS_CD>> flight){
    auto bottleNeck = min_element(flight.begin(), flight.end(), [&](auto a, auto b){
        return getPassengers(a.first, a.second) < getPassengers(b.first, b.second);
    });
    int flightID = bottleNeck->first;
    CLASS_CD curClass = bottleNeck->second;
    return getPassengers(flightID, curClass);
}

vector<pair<long long,vector<pair<int,CLASS_CD>>>> getBestConnectingFlights(int cancelledInventoryID, vector<vector<int>> proposedFlights){

    vector<pair<long long, vector<pair<int, CLASS_CD>>>> allConnectingFlightsWithScore;

    for(auto curFlight: proposedFlights){
        vector<pair<int,CLASS_CD>> curProposedFlight; 
        function<void(int)> generateConnectingFlightWithClass = [&](int index){
            if(index == curFlight.size()){
                allConnectingFlightsWithScore.push_back(
                    make_pair(
                        getFinalConnectingFlightScore(cancelledInventoryID, curProposedFlight),
                        curProposedFlight
                    )
                );
            }
            else{
                for(int toClassCD = 0; toClassCD < 4; toClassCD++){
                    curProposedFlight.push_back(make_pair(curFlight[index],
                                                            static_cast <CLASS_CD> (toClassCD)));
                    generateConnectingFlightWithClass(index+1);
                    curProposedFlight.pop_back();
                }
            }
        };
        generateConnectingFlightWithClass(0);
    }

    auto bestConnectingFlightsWithScore = allConnectingFlightsWithScore;
    sort(bestConnectingFlightsWithScore.begin(), bestConnectingFlightsWithScore.end(), [&](auto a, auto b){
        return (a.first*allowedPassengers(a.second))>(b.first*allowedPassengers(b.second));
    }); 
    bestConnectingFlightsWithScore.resize(
        min(
            (int) MAXIMUM_ALLOWED_CONNECTING_FLIGHTS_PER_JOURNEY,
            (int) bestConnectingFlightsWithScore.size()
        )
    );

    return bestConnectingFlightsWithScore;
}