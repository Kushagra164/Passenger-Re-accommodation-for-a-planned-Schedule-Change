#pragma once
#include "scoring.h"
using namespace std;

vector<int> findAllFlightsFromSrc(int o_inv_id){
    int sch_id = inventoryToScheduleMap[o_inv_id];
    Schedule* o_sch = scheduleMap[sch_id];

    vector<int> ret;

    auto [src, dest] = flightNumberMap[o_sch->flightNum];
    DateTime cancelledDateTime = DateTime(inventoryMap[o_inv_id]->departureDate, o_sch->departureTime);
    for(auto [p_inv_id, Inv]: inventoryMap){
        if (p_inv_id == o_inv_id) continue;
        Schedule* p_sch = scheduleMap[inventoryToScheduleMap[p_inv_id]];
        if ((CancelledFlights.find(p_inv_id)==CancelledFlights.end()) && (flightNumberMap[p_sch->flightNum].srcCity == src) &&
            (flightNumberMap[p_sch->flightNum].destCity != dest)){
            if ((DateTime(Inv->departureDate,p_sch->departureTime) >= cancelledDateTime) &&
            ((DateTime(Inv->arrivalDate,p_sch->arrivalTime) - cancelledDateTime) <= MAXIMUM_ALLOWED_TIME_DIFF)){
                ret.push_back(p_inv_id);
            }
        }
    }
    return ret;
}

vector<int> findAllFlightsToDest(int o_inv_id){
    int sch_id = inventoryToScheduleMap[o_inv_id];
    Schedule* o_sch = scheduleMap[sch_id];

    vector<int> ret;

    auto [src, dest] = flightNumberMap[o_sch->flightNum];
    DateTime cancelledDateTime = DateTime(inventoryMap[o_inv_id]->departureDate, o_sch->departureTime);

    for(auto [p_inv_id, Inv]: inventoryMap){
        if (p_inv_id == o_inv_id) continue;
        Schedule* p_sch = scheduleMap[inventoryToScheduleMap[p_inv_id]];

        if ((CancelledFlights.find(p_inv_id)==CancelledFlights.end()) && (flightNumberMap[p_sch->flightNum].srcCity != src) &&
            (flightNumberMap[p_sch->flightNum].destCity == dest)){
            if ((DateTime(Inv->arrivalDate,p_sch->arrivalTime) >= cancelledDateTime) &&
            ((DateTime(Inv->arrivalDate,p_sch->arrivalTime) - cancelledDateTime) <= MAXIMUM_ALLOWED_TIME_DIFF)){
                ret.push_back(p_inv_id);
            }
        }
    }
    return ret;
}

vector<pair<int,int>> makeConnections(vector<int> &from_src, vector<int> &to_dest){
    vector<pair<int,int>> v;

    for(int inv_id_1:from_src){
        Schedule* sch1=scheduleMap[inventoryToScheduleMap[inv_id_1]];
        for(int inv_id_2:to_dest){
            Schedule* sch2=scheduleMap[inventoryToScheduleMap[inv_id_2]];
            if((flightNumberMap[sch1->flightNum].destCity==flightNumberMap[sch2->flightNum].srcCity) &&
                    (getArrDepTimeDiff(inv_id_1,inv_id_2))>=MINIMUM_CONNECTING_TIME){
                v.push_back(make_pair(inv_id_1,inv_id_2));
            }
        }
    }
    return v;
}

vector<pair<long long,vector<pair<int,CLASS_CD>>>> getBest(int journeyId, vector<pair<int, int>> vecproposed){

    vector<pair<pair<int,CLASS_CD>,pair<int,CLASS_CD>>> allCases;

    auto originalcls = journeyMap[journeyId]->classCD;

    int origcls = originalcls;

    for(auto [proposed1,proposed2]: vecproposed){
        for(int cls1=1;cls1<=4;cls1++){
            for(int cls2=1;cls2<=4;cls2++){
                if (CLASS_DOWNGRADE_ALLOWED==0 && CLASS_UPGRADE_ALLOWED==0)
                {
                    if(cls1!=origcls || cls2!=origcls)
                        continue;
                }
                else if(CLASS_DOWNGRADE_ALLOWED==0)
                {
                    if(cls1>origcls || cls2>origcls)
                        continue;
                }
                else if(CLASS_UPGRADE_ALLOWED==0)
                {
                    if(cls1<origcls || cls2<origcls)
                        continue;
                }

                allCases.push_back({{proposed1,static_cast<CLASS_CD>(cls1)},{proposed2,static_cast<CLASS_CD>(cls2)}});
            }
        }
    }
    sort(allCases.begin(), allCases.end(), [&](auto i, auto j){ return getFinalConnectingFlightScore(journeyId, i.first, i.second) > getFinalConnectingFlightScore(journeyId, j.first , j.second); });
    allCases.resize(min((int) MAXIMUM_ALLOWED_CONNECTIONS_PER_JOURNEY, (int) allCases.size()));


    vector<pair<long long,vector<pair<int,CLASS_CD>>>> finalCases;

    for(auto cur: allCases)
    {
        long long totalScore = getFinalConnectingFlightScore(journeyId, cur.first , cur.second);
        finalCases.push_back({totalScore,{cur.first,cur.second}});
    }

    return finalCases;
}