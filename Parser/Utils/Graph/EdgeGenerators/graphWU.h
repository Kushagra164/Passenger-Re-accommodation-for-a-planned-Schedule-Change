#pragma once
#include "../Helpers/helper.h"
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
            //Single journey has multiple flights
            for(int i=0;i<curJourney->flights.size();i++){
                bool cond1 = false;
                if(DelayedFlights.find(curJourney->flights[i])!=DelayedFlights.end()) cond1 = (DelayedFlights[curJourney->flights[i]] > MAXIMUM_ALLOWED_TIME_DIFF);
                // if flight delay is more than 72 hours


                bool cond2 = false;

                if((i+1)<curJourney->flights.size())
                    cond2 = (getArrDepTimeDiff(curJourney->flights[i],curJourney->flights[i+1]) < MINIMUM_CONNECTING_TIME);
                //if time difference between 2 consecutive flights after delay taken into consideration is less than 1hr

                if(cond1 || cond2){
                    uIndexGenerator.getIndex(curJourneyID);
                    AffectedJourneys.push_back(curJourneyID);
                    break;
                }

            }
        }
    }
}