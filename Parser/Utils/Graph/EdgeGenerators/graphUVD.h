#pragma once
#include "../Helpers/helper.h"
using namespace std;

void graphUVAndGraphDVGenerator(){
    for(int curJourneyID:AffectedJourneys){
        int curUIdx = uIndexGenerator.getIndex(curJourneyID);
        int originalInventoryID = journeyMap[curJourneyID]->flights[0];

        int curPaxCnt = pnrMap[journeyToPnrMap[curJourneyID]]->paxCnt;
        CLASS_CD curClassCD = journeyMap[curJourneyID]->classCD;

        for(auto [curInventoryID, curInventory]:inventoryMap){
            if((CancelledFlights.find(curInventoryID)==CancelledFlights.end()) 
                    && isSameCityPair(originalInventoryID, curInventoryID)){
                
                int origScore = pnrScore(curJourneyID, journeyMap[curJourneyID]->classCD);
                
                for(int toClassCD = 0; toClassCD < 4; toClassCD++){
                    if((toClassCD > curClassCD) && (!CLASS_DOWNGRADE_ALLOWED)) continue;
                    if((toClassCD < curClassCD) && (!CLASS_UPGRADE_ALLOWED)) continue;
                    if(isSufficientInventoryAvailable(curPaxCnt, curInventoryID, static_cast <CLASS_CD> (toClassCD))){

                        int curVIdx = vIndexGenerator.getIndex(make_pair(curInventoryID, static_cast <CLASS_CD> (toClassCD)));
                        long long curScore = origScore*getFlightScore(originalInventoryID, curInventoryID)
                                                    *pnrScore(curJourneyID,static_cast <CLASS_CD> (toClassCD));

                        graphUV[curUIdx].push_back(make_pair(curVIdx, curScore));

                        int curDIdx=dIndexGenerator.getIndex(curInventoryID);

                        // DV should be inserted only once
                        if(find(graphDV[curDIdx].begin(), graphDV[curDIdx].end(), curVIdx) == graphDV[curDIdx].end()){
                            graphDV[curDIdx].push_back(curVIdx);
                        }

                    }
                }
            }
        }
    }
}