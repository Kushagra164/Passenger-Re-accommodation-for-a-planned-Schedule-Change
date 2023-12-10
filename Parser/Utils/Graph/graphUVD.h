#pragma once
#include "calculation.h"
#include "data.h"
#include "../getFlight.h"
using namespace std;



void graphUVAndGraphDVGenerator(){
    int uv_edges=0;
    int dv_edges=0;

    for(int j_id:AffectedJourneys){
        int u_idx = uIndexGenerator.getIndex(j_id);
        int o_inv_id = journeyMap[j_id]->flights[0];

        for(auto x:inventoryMap){
            if((CancelledFlights.find(x.first)==CancelledFlights.end()) && isSameCityPair(o_inv_id,x.first)){
                int pax_cnt = pnrMap[journeyToPnrMap[j_id]]->paxCnt;
                int cls_cd = journeyMap[j_id]->classCD;

                int orig_score = pnrScore(j_id,journeyMap[j_id]->classCD);

                if(isSufficientInventoryAvailable(pax_cnt,x.first,cls_cd)){
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
                        if(isSufficientInventoryAvailable(pax_cnt,x.first,i)){
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
                        if(isSufficientInventoryAvailable(pax_cnt,x.first,i)){
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
}