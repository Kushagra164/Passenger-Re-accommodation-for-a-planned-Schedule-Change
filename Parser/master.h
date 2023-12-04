#include "datetime.h"
#include "uuidgenerator.h"
#include "inventory.h"
#include "schedule.h"
#include "journey.h"
#include "pnr.h"
#include "passenger.h"
#include "graphindexgenerator.h"
#include "constraints.h"
#include <bits/stdc++.h>

using namespace std;

map<char,string> cabinToClassMap;


// Map to get the corresponding InventoryID for a pair of Flight Number and Departure DTMZ

map<pair<int,DateTime>,int> flightToInventoryMap;       //map< pair<FlightNum,DEP_DTMZ> , InventoryID >
bool precalc = true;

void _fn(){
    for (auto &x: inventoryMap){
        int inv_id = x.first;
        Inventory I = *x.second;
        int s_id = inventoryToScheduleMap[inv_id];
        Schedule *S = scheduleMap[s_id];

        DateTime DepDTMZ(I.DepartureDate,S->DepartureTime);

        flightToInventoryMap[make_pair(S->FlightNum,DepDTMZ)] = inv_id;
    }
}

int getFlight(int FLT_NUM,DateTime DepartureDTMZ){

    if (precalc){
        _fn();
        precalc = false;
    }
    if (! flightToInventoryMap[make_pair(FLT_NUM,DepartureDTMZ)])
        return -1;

    return flightToInventoryMap[make_pair(FLT_NUM,DepartureDTMZ)];
}


// Given a set of cancelled flights, function to generate set of affected journeys and graphWU

vector<int> CancelledFlights;

vector<vector<int>> graphWU;                 //Unweighted graph from InventoryID(s) of Cancelled Flights to Affected JourneyID(s)
vector<int> AffectedJourneys;                //Nodes U

int graphWUGenerator(){
    int n_edges=0;

    for(auto x:journeyMap){
        Journey J = x.second;

        bool flag=false;
        for(int inv_id:J.flights){
            if(CancelledFlights.find(inv_id)!=CancelledFlights.end()){
                int w_idx = wIndexGenerator.getIndex(inv_id);
                int j_id = x.first;

                int u_idx = uIndexGenerator.getIndex(j_id);

                graphWU[w_idx].push_back(u_idx);
                n_edges++;

                if(!flag) AffectedJourneys.push_back(j_id),flag=true;
            }
        }
    }
    return n_edges;
}



// PNR Scoring Function

int pnrScore(int journeyId, ClassCDs proposed = ClassCDs::NIL5){
    Journey* journey = journeyMap[journeyId];
    Pnr pnr = pnrMap[journeyToPnrMap[journeyId]];
    if (proposed == ClassCDs::NIL5) proposed = journey->ClassCD;
    int pnr_score = 0;
    for (auto &passenger_id: pnr.Passengers){
        Passenger* passenger = passengerMap[passenger_id];
        pnr_score += passenger->SSRCode ? SSR_SCORE: 0;
        pnr_score += PAX_SCORE;
    }
    pnr_score += classScoresMap[proposed];

    return pnr_score;
}


// Generation of graphUV

Time getDepTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id].DepartureDate,scheduleMap[inventoryToScheduleMap[o_inv_id]].DepartureTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id].DepartureDate,scheduleMap[inventoryToScheduleMap[p_inv_id]].DepartureTime);

    return abs(original - proposed);
}

Time getArrTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id].ArrivalDate,scheduleMap[inventoryToScheduleMap[o_inv_id]].ArrivalTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id].ArrivalDate,scheduleMap[inventoryToScheduleMap[p_inv_id]].ArrivalTime);

    return abs(original - proposed);
}

bool validSolution(int originalId, int proposedId){
    return flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->FlightNum] ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->FlightNum] and
           (getArrTimeDiff(originalId, proposedId) + getDepTimeDiff(originalId, proposedId) <= MAXIMUM_ALLOWED_TIME_DIFF);
}

bool checkInventory(int pax_cnt ,int inv_id,int x){
    int avl_inv;
    if(x==1) avl_inv = inventoryMap[inv_id].FCTotalInventory - inventoryMap[inv_id].FCBookedInventory;
    else if(x==2) avl_inv = inventoryMap[inv_id].BCTotalInventory - inventoryMap[inv_id].BCBookedInventory;
    else if(x==3) avl_inv = inventoryMap[inv_id].PCTotalInventory - inventoryMap[inv_id].PCBookedInventory;
    else avl_inv = inventoryMap[inv_id].ECTotalInventory - inventoryMap[inv_id].ECBookedInventory;

    if(pax_cnt<=avl_inv) return true;

    return false;
}

vector<vector<pair<int,long long>>> graphUV;     //Weighted graph from affected JourneyID(s) to possible flight solution InventoryID(s) cum ClassCD

int graphUVGenerator(){
    int n_edges=0;

    for(int j_id:AffectedJourneys){
        int u_idx = uIndexGenerator.getIndex(j_id);
        int o_inv_id = journeyMap[j_id].flights[0];

        for(auto x:inventoryMap){
            if((CancelledFlights.find(x.first)==CancelledFlights.end()) && validSolution(o_inv_id,x.first)){
                int pax_cnt = journeyMap[journeyToPnrMap[j_id]].PaxCnt;
                int cls_cd = journeyMap[j_id].ClassCD;

                int orig_score = pnrScore(j_id,journeyMap[j_id].ClassCD);

                if(checkInventory(pax_cnt,x.first,cls_cd)){
                    int v_idx = vIndexGenerator.getIndex(make_pair{x.first,journeyMap[j_id].ClassCD});
                    long long score = 1ll*orig_score*orig_score;
                    graphUV[u_idx].push_back(make_pair(v_idx,score));
                    n_edges++;
                }

                if(CLASS_UPGRADE_ALLOWED){
                    for(int i=1;i<cls_cd;i++){
                        if(checkInventory(pax_cnt,x.first,i)){
                            int v_idx = vIndexGenerator.getIndex(make_pair{x.first,static_cast <ClassCDs> (i)});
                            long long score = 1ll*orig_score* pnrScore(j_id,static_cast <ClassCDs> (i));
                            graphUV[u_idx].push_back(make_pair(v_idx,score));
                            n_edges++;
                        }
                    }
                }

                if(CLASS_DOWNGRADE_ALLOWED){
                    for(int i=cls_cd+1;i<=4;i++){
                        if(checkInventory(pax_cnt,x.first,i)){
                            int v_idx = vIndexGenerator.getIndex(make_pair{x.first,static_cast <ClassCDs> (i)});
                            long long score = 1ll*orig_score* pnrScore(j_id,static_cast <ClassCDs> (i));
                            graphUV[u_idx].push_back(make_pair(v_idx,score));
                            n_edges++;
                        }
                    }
                }

            }
        }
    }
    return n_edges;
}

//Flight Score function

long long getFlightScore(int originalinvId, int proposedinvId){
    long long score = 0;

    Time ArrivaltimeDiff = getArrTimeDiff(originalinvId, proposedinvId);
    if (ArrivaltimeDiff <= Time(6, 0))       score += ARRIVAL_DELAY_LT_6_SCORE;
    else if (ArrivaltimeDiff <= Time(12, 0)) score += ARRIVAL_DELAY_LT_12_SCORE;
    else if (ArrivaltimeDiff <= Time(24, 0)) score += ARRIVAL_DELAY_LT_24_SCORE;
    else if (ArrivaltimeDiff <= Time(48, 0)) score += ARRIVAL_DELAY_LT_48_SCORE;

    Time DeparturetimeDiff = getDepTimeDiff(originalinvId, proposedinvId);
    if (DeparturetimeDiff <= Time(6, 0))       score += DEPARTURE_DELAY_LT_6_SCORE;
    else if (DeparturetimeDiff <= Time(12, 0)) score += DEPARTURE_DELAY_LT_12_SCORE;
    else if (DeparturetimeDiff <= Time(24, 0)) score += DEPARTURE_DELAY_LT_24_SCORE;
    else if (DeparturetimeDiff <= Time(48, 0)) score += DEPARTURE_DELAY_LT_48_SCORE;

    score += CITYPAIRS_SCORE;
    score += scheduleMap[inventoryToScheduleMap[originalinvId]]->EquipmentNo ==
             scheduleMap[inventoryToScheduleMap[proposedinvId]]->EquipmentNo ? EQUIPMENT_SCORE: 0;
    return score;
}

// Generation of graphWV


vector<vector<pair<int, long long>>> graphWV;   //Weighted graph from cancelled flight InventoryID(s) to possible flight solution InventoryID(s) cum ClassCD

int graphWVGenerator(){
    int n_edges=0;

    for (int c_inv_id: CancelledFlights){
        int w_idx = wIndexGenerator.getIndex(c_inv_id);

        for (auto p_inv: inventoryMap){
            if ((CancelledFlights.find(p_inv.first) == CancelledFlights.end()) && (validSolution(p_inv.first, c_inv_id))){
                for(int i=1;i<=4;i++){
                    int v_idx = vIndexGenerator.getIndex(make_pair{p_inv.first,static_cast <ClassCDs> (i)});
                    long long score = 1ll*getFlightScore(c_inv_id,p_inv.first);
                    graphWV[w_idx] = make_pair(v_idx,score);
                    n_edges++;
                }
            }
        }
    }
    return n_edges;
}
