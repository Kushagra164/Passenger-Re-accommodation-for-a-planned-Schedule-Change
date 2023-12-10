#pragma once
#include "calculation.h"
#include "data.h"
using namespace std;

void graphWUGenerator(){

    for(auto [curJourneyID, curJourney]:journeyMap){

        if((curJourney->flights.size()==1) && (CancelledFlights.find(curJourney->flights[0])!=CancelledFlights.end())){
            // cancelled single flight
            int curWIdx = wIndexGenerator.getIndex(curJourney->flights[0]);

            int curUIdx = uIndexGenerator.getIndex(curJourneyID);

            AffectedJourneys.push_back(curJourneyID);

            graphWU[curWIdx].push_back(curUIdx);
        }
        else{
            // [TODO: shreyansh remove hardcode 72 hrs, add a little descriptive comment for this section]
            for(int i=0;i<curJourney->flights.size();i++){
                if(DelayedFlights.find(curJourney->flights[i])!=DelayedFlights.end()){
                    bool cond1 = (DelayedFlights[curJourney->flights[i]] > Time(72,0));

                    bool cond2 = false;

                    if((i+1)<curJourney->flights.size()) 
                        cond2= (getArrDepTimeDiff(curJourney->flights[i],curJourney->flights[i+1]) < MINIMUM_CONNECTING_TIME);

                    if(cond1 || cond2){
                        uIndexGenerator.getIndex(curJourneyID);
                        AffectedJourneys.push_back(curJourneyID);
                        break;
                    }
                }
            }
        }
    }
}