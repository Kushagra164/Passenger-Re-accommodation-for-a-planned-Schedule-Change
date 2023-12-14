#pragma once
#include "../Helpers/helper.h"
using namespace std;

void graphWUGenerator(){

    for(auto f:CancelledFlights){
        cout<<scheduleMap[inventoryToScheduleMap[f]]->srcCity<<" "<<scheduleMap[inventoryToScheduleMap[f]]->destCity<<endl;
        cout<<DateTime(inventoryMap[f]->departureDate,scheduleMap[inventoryToScheduleMap[f]]->departureTime).to_string()<<endl;
        cout<<DateTime(inventoryMap[f]->arrivalDate,scheduleMap[inventoryToScheduleMap[f]]->arrivalTime).to_string()<<endl;
    }
    int affectedByCancel = 0, affectedOthers = 0;
    for(auto [curJourneyID, curJourney]:journeyMap){

        if((curJourney->flights.size()==1) && (CancelledFlights.find(curJourney->flights[0])!=CancelledFlights.end())){
            // cancelled single flight
            ++affectedByCancel;
            int curWIdx = wIndexGenerator.getIndex(curJourney->flights[0]);

            int curUIdx = uIndexGenerator.getIndex(curJourneyID);

            AffectedJourneys.push_back(curJourneyID);

            graphWU[curWIdx].push_back(curUIdx);
        }
        else{
            //Single journey has multiple flights
            int temp = affectedOthers;
            string e = pnrUuidGenerator.getString(journeyToPnrMap[curJourneyID]);
            for(int i=0;i<curJourney->flights.size();i++){
                bool cond1 = CancelledFlights.find(curJourney->flights[i])!=CancelledFlights.end();
                if(cond1){
                    if(e[0]=='T')
                        cout<<"checking "<<e<<endl;
                }
                if(DelayedFlights.find(curJourney->flights[i])!=DelayedFlights.end()) 
                    cond1 |= (DelayedFlights[curJourney->flights[i]] > MAXIMUM_ALLOWED_TIME_DIFF);
                // if flight delay is more than 72 hours
                if(cond1){
                    if(e[0]=='T')
                        cout<<"checking "<<e<<endl;
                }

                bool cond2 = false;

                if((i+1)<curJourney->flights.size())
                    cond2 = (getArrDepTimeDiff(curJourney->flights[i],curJourney->flights[i+1]) < MINIMUM_CONNECTING_TIME);
                //if time difference between 2 consecutive flights after delay taken into consideration is less than 1hr
                if(cond2){
                    if(e[0]=='T'){
                        cout<<"checking "<<e<<" "<<getArrDepTimeDiff(curJourney->flights[i],curJourney->flights[i+1]).days<<" "
                            <<getArrDepTimeDiff(curJourney->flights[i],curJourney->flights[i+1]).to_string()<<endl;
                        cout<<MINIMUM_CONNECTING_TIME.to_string()<<endl;
                    } 
                }
                if(cond1 || cond2){
                    ++affectedOthers;
                    uIndexGenerator.getIndex(curJourneyID);
                    AffectedJourneys.push_back(curJourneyID);

                    for(auto flightID:curJourney->flights){
                        Inventory *curFlight = inventoryMap[flightID];
                        CLASS_CD classCD = curJourney->classCD;
                        curFlight->bookedInventory--;
                        if(classCD == FC) curFlight->fcBookedInventory--;
                        if(classCD == BC) curFlight->bcBookedInventory--;
                        if(classCD == PC) curFlight->pcBookedInventory--;
                        if(classCD == EC) curFlight->ecBookedInventory--;
                    }
                    break;
                }

            }
            if(temp!=affectedOthers){
                for(int i=0;i<curJourney->flights.size();++i){
                    int curSchedule = inventoryToScheduleMap[curJourney->flights[i]];
                    cout<<pnrUuidGenerator.getString(journeyToPnrMap[curJourneyID])<<" "<<scheduleMap[curSchedule]->srcCity<<" "<<scheduleMap[curSchedule]->destCity<<endl; 
                }
            }
            assert(affectedOthers<=(temp+1));
        }
    }
    cout<<__LINE__<<" "<<affectedByCancel<<" "<<affectedOthers<<endl;
}