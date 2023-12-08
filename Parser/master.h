#include "datetime.h"
#include "uuidGenerator.h"
#include "inventory.h"
#include "schedule.h"
#include "journey.h"
#include "pnr.h"
#include "passenger.h"
#include "graphIndexGenerator.h"
#include "constants.h"

using namespace std;

map<char,string> cabinToClassMap;

template <typename T>
class default_vector : public vector<T> {
public:
    using vector<T>::vector;
    void set_default_value(const T &val) {
        default_value = val;
    }
    const T& operator[](int i) const {
        return index_out_of_bounds(i) ? default_value : vector<T>::operator[](i);
    }
    T& operator[](int i) {
        if (index_out_of_bounds(i)) resize(i + 1);
        return vector<T>::operator[](i);
    }
    const T& at(int i) const {
        return (*this)[i];
    }
    T& at(int i) {
        return (*this)[i];
    }
    T get_or_default(int i) const {
        return index_out_of_bounds(i) ? default_value : vector<T>::operator[](i);
    }
    T get_or_default(int i, T tmp_default_value) const {
        return index_out_of_bounds(i) ? tmp_default_value : vector<T>::operator[](i);
    }
    int size() {
        return vector<T>::size();
    }
    void resize(int n) {
        vector<T>::resize(n, default_value);
    }
private:
    T default_value;
    bool index_out_of_bounds(unsigned int i) const {
        return i >= vector<T>::size();
    }
};


// Map to get the corresponding InventoryID for a pair of Flight Number and Departure DTMZ
// Used while parsing passenger booking details and creating Journey

map<pair<int,pair<DateTime,DateTime>>,int> flightToInventoryMap;       //map< pair<FlightNum,pair<DEP_DTMZ,ARR_DTMZ>> , InventoryID >
bool precalc = true;

void _fn(){
    for (auto &x: inventoryMap){
        int inv_id = x.first;
        Inventory I = *x.second;
        int s_id = inventoryToScheduleMap[inv_id];
        Schedule *S = scheduleMap[s_id];

        DateTime DepDTMZ(I.DepartureDate,S->DepartureTime);
        DateTime ArrDTMZ(I.ArrivalDate,S->ArrivalTime);

        flightToInventoryMap[make_pair(S->FlightNum,make_pair(DepDTMZ,ArrDTMZ))] = inv_id;
    }
}

int getFlight(int FLT_NUM,DateTime DepartureDTMZ, DateTime ArrivalDTMZ){

    if (precalc){
        _fn();
        precalc = false;
    }
    for (auto &f: flightToInventoryMap){
        auto [a, b] = f.first;
    }
    if (! flightToInventoryMap.count(make_pair(FLT_NUM,make_pair(DepartureDTMZ,ArrivalDTMZ)))){
        cerr<<"Could not find inventory for flight:"
            <<FLT_NUM<<" "<<DepartureDTMZ.to_string()<<" "<<ArrivalDTMZ.to_string()<<endl;
        return -1;
    }

    return flightToInventoryMap[make_pair(FLT_NUM,make_pair(DepartureDTMZ,ArrivalDTMZ))];
}


// Given a set of cancelled flights and map of delayed flights,
// function to generate set of affected journeys and graphWU

set<int> CancelledFlights;                   //set<InventoryID>
map<int,Time> DelayedFlights;                //map<InventoryID,Time delay>

default_vector <vector<int>> graphWU;                 //Unweighted graph from InventoryID(s) of Cancelled Flights to Affected JourneyID(s)
vector<int> AffectedJourneys;                //Nodes U

Time getArrDepTimeDiff(int c_inv_id,int n_inv_id){
    DateTime curr = DateTime(inventoryMap[c_inv_id]->DepartureDate,scheduleMap[inventoryToScheduleMap[c_inv_id]]->ArrivalTime);
    DateTime next = DateTime(inventoryMap[n_inv_id]->DepartureDate,scheduleMap[inventoryToScheduleMap[n_inv_id]]->DepartureTime);

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

int pnrScore(int journeyId, ClassCDs proposed = ClassCDs::NIL5){
    Journey* journey = journeyMap[journeyId];
    Pnr* pnr = pnrMap[journeyToPnrMap[journeyId]];
    if (proposed == ClassCDs::NIL5) proposed = journey->ClassCD;
    int pnr_score = 0;
    for (auto &passenger_id: pnr->Passengers){
        Passenger* passenger = passengerMap[passenger_id];
        pnr_score += passenger->SSRCode ? SSR_SCORE: 0;
        pnr_score += PAX_SCORE;
    }
    pnr_score += classScoresMap[proposed];

    return pnr_score;
}

//Flight Score function

Time getDepTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id]->DepartureDate,scheduleMap[inventoryToScheduleMap[o_inv_id]]->DepartureTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id]->DepartureDate,scheduleMap[inventoryToScheduleMap[p_inv_id]]->DepartureTime);

    return abs(original - proposed);
}

Time getArrTimeDiff(int o_inv_id,int p_inv_id){
    DateTime original = DateTime(inventoryMap[o_inv_id]->ArrivalDate,scheduleMap[inventoryToScheduleMap[o_inv_id]]->ArrivalTime);
    DateTime proposed = DateTime(inventoryMap[p_inv_id]->ArrivalDate,scheduleMap[inventoryToScheduleMap[p_inv_id]]->ArrivalTime);

    return abs(original - proposed);
}

bool validSolution(int originalId, int proposedId){
    return flightNumberMap[scheduleMap[inventoryToScheduleMap[originalId]]->FlightNum] ==
           flightNumberMap[scheduleMap[inventoryToScheduleMap[proposedId]]->FlightNum] and
           (getArrTimeDiff(originalId, proposedId) + getDepTimeDiff(originalId, proposedId) <= MAXIMUM_ALLOWED_TIME_DIFF*2);
}

bool checkInventory(int pax_cnt ,int inv_id,int x){
    int avl_inv;
    if(x==1) avl_inv = inventoryMap[inv_id]->FCTotalInventory - inventoryMap[inv_id]->FCBookedInventory;
    else if(x==2) avl_inv = inventoryMap[inv_id]->BCTotalInventory - inventoryMap[inv_id]->BCBookedInventory;
    else if(x==3) avl_inv = inventoryMap[inv_id]->PCTotalInventory - inventoryMap[inv_id]->PCBookedInventory;
    else avl_inv = inventoryMap[inv_id]->ECTotalInventory - inventoryMap[inv_id]->ECBookedInventory;

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
    score += scheduleMap[inventoryToScheduleMap[originalinvId]]->EquipmentNo ==
             scheduleMap[inventoryToScheduleMap[proposedinvId]]->EquipmentNo ? EQUIPMENT_SCORE: 0;
    return score;
}



// Generation of graphUV and graphDV

default_vector<vector<pair<int,long long>>> graphUV;     //Weighted graph from affected JourneyID(s) to possible flight solution InventoryID(s) cum ClassCD
default_vector <vector<int>> graphDV;                     //Unweighted graph from flight Inventory ID(s) to their possible classes

pair<int,int> graphUVAndGraphDVGenerator(){
    int uv_edges=0;
    int dv_edges=0;

    for(int j_id:AffectedJourneys){
        int u_idx = uIndexGenerator.getIndex(j_id);
        int o_inv_id = journeyMap[j_id]->flights[0];

        for(auto x:inventoryMap){
            if((CancelledFlights.find(x.first)==CancelledFlights.end()) && validSolution(o_inv_id,x.first)){
                int pax_cnt = pnrMap[journeyToPnrMap[j_id]]->PaxCnt;
                int cls_cd = journeyMap[j_id]->ClassCD;

                int orig_score = pnrScore(j_id,journeyMap[j_id]->ClassCD);

                if(checkInventory(pax_cnt,x.first,cls_cd)){
                    int v_idx = vIndexGenerator.getIndex(make_pair(x.first,journeyMap[j_id]->ClassCD));
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
                            int v_idx = vIndexGenerator.getIndex(make_pair(x.first,static_cast <ClassCDs> (i)));
                            long long score = 1ll*orig_score*pnrScore(j_id,static_cast <ClassCDs> (i))*getFlightScore(o_inv_id,x.first);
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
                            int v_idx = vIndexGenerator.getIndex(make_pair(x.first,static_cast <ClassCDs> (i)));
                            long long score = 1ll*orig_score* pnrScore(j_id,static_cast <ClassCDs> (i));
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

    auto [src, dest] = flightNumberMap[o_sch->FlightNum];
    DateTime cancelledDateTime = DateTime(inventoryMap[o_inv_id]->DepartureDate, o_sch->DepartureTime);

    for(auto [p_inv_id, Inv]: inventoryMap){
        if (p_inv_id == o_inv_id) continue;
        Schedule* p_sch = scheduleMap[inventoryToScheduleMap[p_inv_id]];

        if ((CancelledFlights.find(p_inv_id)==CancelledFlights.end()) && (flightNumberMap[p_sch->FlightNum].first == src) &&
            (flightNumberMap[p_sch->FlightNum].second != dest)){
            if ((DateTime(Inv->DepartureDate,p_sch->DepartureTime) >= cancelledDateTime) &&
            ((DateTime(Inv->ArrivalDate,p_sch->ArrivalTime) - cancelledDateTime) <= MAXIMUM_ALLOWED_TIME_DIFF)){
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

    auto [src, dest] = flightNumberMap[o_sch->FlightNum];
    DateTime cancelledDateTime = DateTime(inventoryMap[o_inv_id]->DepartureDate, o_sch->DepartureTime);

    for(auto [p_inv_id, Inv]: inventoryMap){
        if (p_inv_id == o_inv_id) continue;
        Schedule* p_sch = scheduleMap[inventoryToScheduleMap[p_inv_id]];

        if ((CancelledFlights.find(p_inv_id)==CancelledFlights.end()) && (flightNumberMap[p_sch->FlightNum].first != src) &&
            (flightNumberMap[p_sch->FlightNum].second == dest)){
            if ((DateTime(Inv->ArrivalDate,p_sch->ArrivalTime) >= cancelledDateTime) &&
            ((DateTime(Inv->ArrivalDate,p_sch->ArrivalTime) - cancelledDateTime) <= MAXIMUM_ALLOWED_TIME_DIFF)){
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
            if((flightNumberMap[sch1->FlightNum].second==flightNumberMap[sch2->FlightNum].first) &&
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


    DateTime proposed1departure = DateTime(inventoryMap[proposedinvId1]->DepartureDate,scheduleMap[inventoryToScheduleMap[proposedinvId1]]->DepartureTime);
    DateTime proposed2departure = DateTime(inventoryMap[proposedinvId2]->DepartureDate,scheduleMap[inventoryToScheduleMap[proposedinvId2]]->DepartureTime);
    DateTime proposed1arrival = DateTime(inventoryMap[proposedinvId1]->ArrivalDate,scheduleMap[inventoryToScheduleMap[proposedinvId1]]->ArrivalTime);
    DateTime proposed2arrival = DateTime(inventoryMap[proposedinvId2]->ArrivalDate,scheduleMap[inventoryToScheduleMap[proposedinvId2]]->ArrivalTime);
    Time t1 = proposed1arrival - proposed1departure;
    Time t2 = proposed2arrival - proposed2departure;
    Time t3 = t1 + t2;


    score += CITYPAIR_SCORE;
    int score1 = scheduleMap[inventoryToScheduleMap[originalinvId]]->EquipmentNo ==
                 scheduleMap[inventoryToScheduleMap[proposedinvId1]]->EquipmentNo ? EQUIPMENT_SCORE: 0;
    int score2 = scheduleMap[inventoryToScheduleMap[originalinvId]]->EquipmentNo ==
                 scheduleMap[inventoryToScheduleMap[proposedinvId2]]->EquipmentNo ? EQUIPMENT_SCORE: 0;
    score += (t1.value() * score1 + t2.value() * score2) / t3.value();

    return score;
}

long long getFinalConnectingFlightScore(int journeyId, pair<int, ClassCDs> proposed1, pair<int, ClassCDs> proposed2){
    long long originalPnrScore = pnrScore(journeyId, journeyMap[journeyId]->ClassCD);
    auto [invid1, classcd1] = proposed1;
    auto [invid2, classcd2] = proposed2;
    long long connectingFlightScore = getConnectingFlightScore(make_pair(invid1, invid2), journeyMap[journeyId]->flights[0]);

    long long newPnrScore = (pnrScore(journeyId, classcd1) + pnrScore(journeyId, classcd2)) / 2;

    return originalPnrScore * newPnrScore * connectingFlightScore;
}

vector<pair<long long,vector<pair<int,ClassCDs>>>> getBest(int journeyId, vector<pair<int, int>> vecproposed){

    vector<pair<pair<int,ClassCDs>,pair<int,ClassCDs>>> allCases;

    auto originalcls = journeyMap[journeyId]->ClassCD;

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

                allCases.push_back({{proposed1,static_cast<ClassCDs>(cls1)},{proposed2,static_cast<ClassCDs>(cls2)}});
            }
        }
    }
    sort(allCases.begin(), allCases.end(), [&](auto i, auto j){ return getFinalConnectingFlightScore(journeyId, i.first, i.second) > getFinalConnectingFlightScore(journeyId, j.first , j.second); });
    allCases.resize(min((int) MAXIMUM_ALLOWED_CONNECTIONS_PER_JOURNEY, (int) allCases.size()));


    vector<pair<long long,vector<pair<int,ClassCDs>>>> finalCases;

    for(auto cur: allCases)
    {
        long long totalScore = getFinalConnectingFlightScore(journeyId, cur.first , cur.second);
        finalCases.push_back({totalScore,{cur.first,cur.second}});
    }

    return finalCases;
}


default_vector <vector<pair<int,long long>>> graphUC;       //Weighted graph from affected JourneyID(s) to possible connection solutions
default_vector <vector<int>> graphCV;       //Unweighted graph from connection solution to its solution flight Inventory ID(s)

pair<int,int> graphUCAndGraphCVGenerator(){
    int uc_edges=0;
    int cv_edges=0;

    for(int j_id:AffectedJourneys){
        vector<int> v1=findAllFlightsFromSrc(journeyMap[j_id]->flights[0]);
        vector<int> v2=findAllFlightsToDest(journeyMap[j_id]->flights[0]);
        vector<pair<int,int>> v3= makeConnections(v1,v2);

        cerr<<v1.size()<<endl;
        cerr<<v2.size()<<endl;
        cerr<<v3.size()<<endl;

        vector<pair<long long,vector<pair<int,ClassCDs>>>> v4=getBest(j_id,v3);

        int u_id=uIndexGenerator.getIndex(j_id);

        for(auto [wt,ids]:v4){
            int c_id=cIndexGenerator.getIndex(ids);

            graphUC[u_id].push_back(make_pair(wt,c_id));
            uc_edges++;

            for(auto x:ids){
                int v_id=vIndexGenerator.getIndex(x);
                graphCV[c_id].push_back(v_id);
                cv_edges++;
            }
        }
    }
    return make_pair(uc_edges,cv_edges);
}


// Generation of graphWD
default_vector <vector<int>> graphWD;   //Unweighted graph from cancelled flight InventoryID(s) to possible flight solution InventoryID(s)

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