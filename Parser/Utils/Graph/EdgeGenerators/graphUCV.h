#pragma once
#include "../Helpers/helper.h"
using namespace std;

void graphUCAndGraphCVGenerator(){
    for(int j_id:AffectedJourneys){
        vector<int> v1=findAllFlightsFromSrc(journeyMap[j_id]->flights[0]);
        vector<int> v2=findAllFlightsToDest(journeyMap[j_id]->flights[0]);
        vector<pair<int,int>> v3= makeConnections(v1,v2);


        vector<pair<long long,vector<pair<int,CLASS_CD>>>> v4=getBest(j_id,v3);

        int u_id=uIndexGenerator.getIndex(j_id);

        for(auto [wt,ids]:v4){
            int c_id=cIndexGenerator.getIndex(ids);

            graphUC[u_id].push_back(make_pair(c_id,wt));

            for(auto x:ids){
                int v_id=vIndexGenerator.getIndex(x);
                if(find(graphCV[c_id].begin(),graphCV[c_id].end(),v_id) == graphCV[c_id].end()){
                    graphCV[c_id].push_back(v_id);
                }
            }
        }
    }
}