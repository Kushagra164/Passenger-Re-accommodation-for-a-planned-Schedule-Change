#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include "DateTime/datetime.h"
#include "extendableVector.h"
#include "../DataModels/schedule.h"
#include "../DataModels/inventory.h"
#include "../DataModels/journey.h"
#include "../DataModels/pnr.h"
#include "../DataModels/passenger.h"
#include "../DataModels/flight.h"
#include "graphIndexGenerator.h"
#include "constants.h"
using namespace std;

map<char,string> cabinToClassMap;

// Given a set of cancelled flights and map of delayed flights,
// function to generate set of affected journeys and graphWU

set<int> CancelledFlights;                   //set<InventoryID>
map<int,Time> DelayedFlights;                //map<InventoryID,Time delay>

ExtendableVector <vector<int>> graphWU;                 //Unweighted graph from InventoryID(s) of Cancelled Flights to Affected JourneyID(s)
vector<int> AffectedJourneys;                //Nodes U

Time getArrDepTimeDiff(int c_inv_id,int n_inv_id){
    DateTime curr = DateTime(inventoryMap[c_inv_id]->departureDate,scheduleMap[inventoryToScheduleMap[c_inv_id]]->arrivalTime);
    DateTime next = DateTime(inventoryMap[n_inv_id]->departureDate,scheduleMap[inventoryToScheduleMap[n_inv_id]]->departureTime);

    return next-curr;
}

int graphWUGenerator(){
    int n_edges=0;

    for(auto x:journeyMap){
        Journey* J = x.second;
        int j_id = x.first;


        if((J->flights.size()==1) && (CancelledFlights.find(J->flights[0])!=CancelledFlights.end())){
            int w_idx = wIndexGenerator.getIndex(J->flights[0]);

            int u_idx = uIndexGenerator.getIndex(j_id);

            AffectedJourneys.push_back(j_id);

            graphWU[w_idx].push_back(u_idx);
            n_edges++;
        }
        else{
            for(int i=0;i<J->flights.size();i++){
                if(DelayedFlights.find(J->flights[i])!=DelayedFlights.end()){
                    bool cond1 = (DelayedFlights[J->flights[i]] > Time(72,0));

                    bool cond2 = false;
                    if((i+1)<J->flights.size()) cond2= (getArrDepTimeDiff(J->flights[i],J->flights[i+1]) < MINIMUM_CONNECTING_TIME);

                    if(cond1 || cond2){
                        int u_idx = uIndexGenerator.getIndex(j_id);
                        AffectedJourneys.push_back(j_id);
                        break;
                    }
                }
            }
        }
    }
    return n_edges;
}



// PNR Scoring Function

int pnrScore(int journeyId, CLASS_CD proposed = CLASS_CD::NIL5){
    Journey* journey = journeyMap[journeyId];
    Pnr* pnr = pnrMap[journeyToPnrMap[journeyId]];
    if (proposed == CLASS_CD::NIL5) proposed = journey->classCD;
    int pnr_score = 0;
    for (auto &passenger_id: pnr->passengers){
        Passenger* passenger = passengerMap[passenger_id];
        pnr_score += (passenger->ssrCodes.size()*SSR_SCORE);
        pnr_score += PAX_SCORE;
    }
    pnr_score += classScoresMap[proposed];

    return pnr_score;
}

//Flight Score function

Time getDepTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id]->departureDate,scheduleMap[inventoryToScheduleMap[o_inv_id]]->departureTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id]->departureDate,scheduleMap[inventoryToScheduleMap[p_inv_id]]->departureTime);

    return abs(original - proposed);
}

Time getArrTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id]->arrivalDate,scheduleMap[inventoryToScheduleMap[o_inv_id]]->arrivalTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id]->arrivalDate,scheduleMap[inventoryToScheduleMap[p_inv_id]]->arrivalTime);

    return abs(original - proposed);
}

bool validSolution(int originalId, int proposedId){
    return (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].srcCity ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].srcCity) &&
           (flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->flightNum].destCity ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->flightNum].destCity) &&
           (getArrTimeDiff(originalId, proposedId) + getDepTimeDiff(originalId, proposedId) <= MAXIMUM_ALLOWED_TIME_DIFF*2);
}

bool checkInventory(int pax_cnt ,int inv_id,int x){
    int avl_inv;
    if(x==1) avl_inv = inventoryMap[inv_id]->fcTotalInventory - inventoryMap[inv_id]->fcBookedInventory;
    else if(x==2) avl_inv = inventoryMap[inv_id]->bcTotalInventory - inventoryMap[inv_id]->bcBookedInventory;
    else if(x==3) avl_inv = inventoryMap[inv_id]->pcTotalInventory - inventoryMap[inv_id]->pcBookedInventory;
    else avl_inv = inventoryMap[inv_id]->ecTotalInventory - inventoryMap[inv_id]->ecBookedInventory;

    return pax_cnt <= avl_inv;
}

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

    score += CITYPAIR_SCORE;
    score += scheduleMap[inventoryToScheduleMap[originalinvId]]->equipmentNo ==
             scheduleMap[inventoryToScheduleMap[proposedinvId]]->equipmentNo ? EQUIPMENT_SCORE: 0;
    return score;
}



// Generation of graphUV and graphDV

ExtendableVector<vector<pair<int,long long>>> graphUV;     //Weighted graph from affected JourneyID(s) to possible flight solution InventoryID(s) cum classCD
ExtendableVector<vector<int>> graphDV;                     //Unweighted graph from flight Inventory ID(s) to their possible classes

pair<int,int> graphUVAndGraphDVGenerator(){
    int uv_edges=0;
    int dv_edges=0;

    for(int j_id:AffectedJourneys){
        int u_idx = uIndexGenerator.getIndex(j_id);
        int o_inv_id = journeyMap[j_id]->flights[0];

        for(auto x:inventoryMap){
            if((CancelledFlights.find(x.first)==CancelledFlights.end()) && validSolution(o_inv_id,x.first)){
                int pax_cnt = pnrMap[journeyToPnrMap[j_id]]->paxCnt;
                int cls_cd = journeyMap[j_id]->classCD;

                int orig_score = pnrScore(j_id,journeyMap[j_id]->classCD);

                if(checkInventory(pax_cnt,x.first,cls_cd)){
                    int v_idx = vIndexGenerator.getIndex(make_pair(x.first,journeyMap[j_id]->classCD));
                    long long score = 1ll*orig_score*orig_score*getFlightScore(o_inv_id,x.first);
                    graphUV[u_idx].push_back(make_pair(v_idx,score));
                    uv_edges++;

                    int d_idx=dIndexGenerator.getIndex(x.first);
                    if(find(graphDV[d_idx].begin(),graphDV[d_idx].end(),v_idx) == graphDV[d_idx].end()){
                        graphDV[d_idx].push_back(v_idx);
                        dv_edges++;
                    }
                }

                if(CLASS_UPGRADE_ALLOWED){
                    for(int i=1;i<cls_cd;i++){
                        if(checkInventory(pax_cnt,x.first,i)){
                            int v_idx = vIndexGenerator.getIndex(make_pair(x.first,static_cast <CLASS_CD> (i)));
                            long long score = 1ll*orig_score*pnrScore(j_id,static_cast <CLASS_CD> (i))*getFlightScore(o_inv_id,x.first);
                            graphUV[u_idx].push_back(make_pair(v_idx,score));
                            uv_edges++;

                            int d_idx=dIndexGenerator.getIndex(x.first);
                            if(find(graphDV[d_idx].begin(),graphDV[d_idx].end(),v_idx) == graphDV[d_idx].end()){
                                graphDV[d_idx].push_back(v_idx);
                                dv_edges++;
                            }

                        }
                    }
                }

                if(CLASS_DOWNGRADE_ALLOWED){
                    for(int i=cls_cd+1;i<=4;i++){
                        if(checkInventory(pax_cnt,x.first,i)){
                            int v_idx = vIndexGenerator.getIndex(make_pair(x.first,static_cast <CLASS_CD> (i)));
                            long long score = 1ll*orig_score* pnrScore(j_id,static_cast <CLASS_CD> (i));
                            graphUV[u_idx].push_back(make_pair(v_idx,score));
                            uv_edges++;

                            int d_idx=dIndexGenerator.getIndex(x.first);
                            if(find(graphDV[d_idx].begin(),graphDV[d_idx].end(),v_idx) == graphDV[d_idx].end()){
                                graphDV[d_idx].push_back(v_idx);
                                dv_edges++;
                            }
                        }
                    }
                }


            }
        }
    }
    return make_pair(uv_edges,dv_edges);
}

// Generation of graphUC and graphCV

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

long long getConnectingFlightScore(pair<int, int> proposedinvIds, int originalinvId){
    long long score = 0;
    auto [proposedinvId1, proposedinvId2] = proposedinvIds;
    Time ArrivaltimeDiff = getArrTimeDiff(originalinvId, proposedinvId2);
    if (ArrivaltimeDiff <= Time(6, 0))       score += ARRIVAL_DELAY_LT_6_SCORE;
    else if (ArrivaltimeDiff <= Time(12, 0)) score += ARRIVAL_DELAY_LT_12_SCORE;
    else if (ArrivaltimeDiff <= Time(24, 0)) score += ARRIVAL_DELAY_LT_24_SCORE;
    else if (ArrivaltimeDiff <= Time(48, 0)) score += ARRIVAL_DELAY_LT_48_SCORE;

    Time DeparturetimeDiff = getDepTimeDiff(originalinvId, proposedinvId1);
    if (DeparturetimeDiff <= Time(6, 0))       score += DEPARTURE_DELAY_LT_6_SCORE;
    else if (DeparturetimeDiff <= Time(12, 0)) score += DEPARTURE_DELAY_LT_12_SCORE;
    else if (DeparturetimeDiff <= Time(24, 0)) score += DEPARTURE_DELAY_LT_24_SCORE;
    else if (DeparturetimeDiff <= Time(48, 0)) score += DEPARTURE_DELAY_LT_48_SCORE;


    DateTime proposed1departure = DateTime(inventoryMap[proposedinvId1]->departureDate,scheduleMap[inventoryToScheduleMap[proposedinvId1]]->departureTime);
    DateTime proposed2departure = DateTime(inventoryMap[proposedinvId2]->departureDate,scheduleMap[inventoryToScheduleMap[proposedinvId2]]->departureTime);
    DateTime proposed1arrival = DateTime(inventoryMap[proposedinvId1]->arrivalDate,scheduleMap[inventoryToScheduleMap[proposedinvId1]]->arrivalTime);
    DateTime proposed2arrival = DateTime(inventoryMap[proposedinvId2]->arrivalDate,scheduleMap[inventoryToScheduleMap[proposedinvId2]]->arrivalTime);
    Time t1 = proposed1arrival - proposed1departure;
    Time t2 = proposed2arrival - proposed2departure;
    Time t3 = t1 + t2;


    score += CITYPAIR_SCORE;
    int score1 = scheduleMap[inventoryToScheduleMap[originalinvId]]->equipmentNo ==
                 scheduleMap[inventoryToScheduleMap[proposedinvId1]]->equipmentNo ? EQUIPMENT_SCORE: 0;
    int score2 = scheduleMap[inventoryToScheduleMap[originalinvId]]->equipmentNo ==
                 scheduleMap[inventoryToScheduleMap[proposedinvId2]]->equipmentNo ? EQUIPMENT_SCORE: 0;
    score += (t1.value() * score1 + t2.value() * score2) / t3.value();

    return score;
}

long long getFinalConnectingFlightScore(int journeyId, pair<int, CLASS_CD> proposed1, pair<int, CLASS_CD> proposed2){
    long long originalPnrScore = pnrScore(journeyId, journeyMap[journeyId]->classCD);
    auto [invid1, classcd1] = proposed1;
    auto [invid2, classcd2] = proposed2;
    long long connectingFlightScore = getConnectingFlightScore(make_pair(invid1, invid2), journeyMap[journeyId]->flights[0]);

    long long newPnrScore = (pnrScore(journeyId, classcd1) + pnrScore(journeyId, classcd2)) / 2;

    return originalPnrScore * newPnrScore * connectingFlightScore;
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


ExtendableVector <vector<pair<int,long long>>> graphUC;       //Weighted graph from affected JourneyID(s) to possible connection solutions
ExtendableVector <vector<int>> graphCV;       //Unweighted graph from connection solution to its solution flight Inventory ID(s)

pair<int,int> graphUCAndGraphCVGenerator(){
    int uc_edges=0;
    int cv_edges=0;

    for(int j_id:AffectedJourneys){
        vector<int> v1=findAllFlightsFromSrc(journeyMap[j_id]->flights[0]);
        vector<int> v2=findAllFlightsToDest(journeyMap[j_id]->flights[0]);
        vector<pair<int,int>> v3= makeConnections(v1,v2);


        vector<pair<long long,vector<pair<int,CLASS_CD>>>> v4=getBest(j_id,v3);

        int u_id=uIndexGenerator.getIndex(j_id);

        for(auto [wt,ids]:v4){
            int c_id=cIndexGenerator.getIndex(ids);

            graphUC[u_id].push_back(make_pair(c_id,wt));
            uc_edges++;

            for(auto x:ids){
                int v_id=vIndexGenerator.getIndex(x);
                if(find(graphCV[c_id].begin(),graphCV[c_id].end(),v_id) == graphCV[c_id].end()){
                    graphCV[c_id].push_back(v_id);
                    cv_edges++;
                }
            }
        }
    }
    return make_pair(uc_edges,cv_edges);
}


// Generation of graphWD
ExtendableVector<vector<int>> graphWD;   //Unweighted graph from cancelled flight InventoryID(s) to possible flight solution InventoryID(s)

int graphWDGenerator(){
    int n_edges=0;

    for (int c_inv_id: CancelledFlights){
        int w_idx = wIndexGenerator.getIndex(c_inv_id);
        for (auto p_inv: inventoryMap){
            if ((CancelledFlights.find(p_inv.first) == CancelledFlights.end()) && (validSolution(p_inv.first, c_inv_id))){
                int d_idx = dIndexGenerator.getIndex(p_inv.first);
                graphWD[w_idx].push_back(d_idx);
                n_edges++;
            }
        }
    }
    return n_edges;
}