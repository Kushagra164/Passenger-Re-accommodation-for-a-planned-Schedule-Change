#pragma once
#include "data.h"
#include "timeDiff.h"
using namespace std;

int pnrScore(int journeyId, CLASS_CD proposed){
    Pnr* pnr = pnrMap[journeyToPnrMap[journeyId]];
    int pnr_score = 0;
    for (auto &passenger_id: pnr->passengers){
        Passenger* passenger = passengerMap[passenger_id];
        if(passenger->ssrCode != SSR_CD_NULL) pnr_score += SSR_SCORE;
        pnr_score += loyaltyScoresMap[passenger->loyaltyCode];
        pnr_score += PAX_SCORE;
    }
    pnr_score += classScoresMap[proposed];
    return pnr_score;
}

long long getFlightScoreWithTimeDiff(Time ArrivalTimeDiff, Time DepartureTimeDiff){
    long long score = 0;

    if (ArrivalTimeDiff <= Time(6, 0))       score += ARRIVAL_DELAY_LT_6_SCORE;
    else if (ArrivalTimeDiff <= Time(12, 0)) score += ARRIVAL_DELAY_LT_12_SCORE;
    else if (ArrivalTimeDiff <= Time(24, 0)) score += ARRIVAL_DELAY_LT_24_SCORE;
    else if (ArrivalTimeDiff <= Time(48, 0)) score += ARRIVAL_DELAY_LT_48_SCORE;

    if (DepartureTimeDiff <= Time(6, 0))       score += DEPARTURE_DELAY_LT_6_SCORE;
    else if (DepartureTimeDiff <= Time(12, 0)) score += DEPARTURE_DELAY_LT_12_SCORE;
    else if (DepartureTimeDiff <= Time(24, 0)) score += DEPARTURE_DELAY_LT_24_SCORE;
    else if (DepartureTimeDiff <= Time(48, 0)) score += DEPARTURE_DELAY_LT_48_SCORE;

    return score;
}

long long getFlightScore(int curJourneyID, int nextInventoryID){
    long long score = 0;

    Time ArrivalTimeDiff = getArrTimeDiff(journeyMap[curJourneyID]->flights.back(), nextInventoryID);
    Time DepartureTimeDiff = getDepTimeDiff(journeyMap[curJourneyID]->flights[0], nextInventoryID);
    
    score += getFlightScoreWithTimeDiff(ArrivalTimeDiff, DepartureTimeDiff);

    score += CITYPAIR_SCORE;
    if(journeyMap[curJourneyID]->flights.size()==1){
        score += scheduleMap[inventoryToScheduleMap[journeyMap[curJourneyID]->flights[0]]]->equipmentNo ==
             scheduleMap[inventoryToScheduleMap[nextInventoryID]]->equipmentNo ? EQUIPMENT_SCORE: 0;
    }
    return score;
}

long long getConnectingFlightScore(int originalInventoryID, vector<pair<int, CLASS_CD>> connectingFlight){
    long long score = 0;

    Time ArrivalTimeDiff = getArrTimeDiff(connectingFlight[0].first, originalInventoryID);
    Time DepartureTimeDiff = getDepTimeDiff(connectingFlight.back().first, originalInventoryID);
    
    score += getFlightScoreWithTimeDiff(ArrivalTimeDiff, DepartureTimeDiff);

    score += CITYPAIR_SCORE;

    return score;
}

long long getConnectingFlightScoreForJourney(int affectedJourneyID, vector<pair<int, CLASS_CD>> connectingFlight){
    long long score = 0;
    Journey *curJourney = journeyMap[affectedJourneyID];

    Time ArrivalTimeDiff = getArrTimeDiff(connectingFlight[0].first, curJourney->flights.back());
    Time DepartureTimeDiff = getDepTimeDiff(connectingFlight.back().first, curJourney->flights[0]);
    
    score += getFlightScoreWithTimeDiff(ArrivalTimeDiff, DepartureTimeDiff);

    score += CITYPAIR_SCORE;

    return score;
}

long long getFinalConnectingFlightScore(int cancelInventoryID, vector<pair<int, CLASS_CD>> proposedFlight){

    long double avgPnrScore = 0;
    long double totalTime = 0;

    for(auto [curFlightID, curClassCD]: proposedFlight){
        long double curTime = getFlightDuration(curFlightID).value();
        totalTime += curTime;
        avgPnrScore += (curTime*classScoresMap[curClassCD]);
    }

    avgPnrScore /= totalTime;

    return getConnectingFlightScore(cancelInventoryID, proposedFlight)*avgPnrScore/proposedFlight.size();
}

long long getFinalConnectingFlightScoreForJourney(int affectedJourneyID, vector<pair<int, CLASS_CD>> proposedFlight){

    long double avgPnrScore = 0;
    long double totalTime = 0;

    for(auto [curFlightID, curClassCD]: proposedFlight){
        long double curTime = getFlightDuration(curFlightID).value();
        totalTime += curTime;
        avgPnrScore += (curTime*classScoresMap[curClassCD]);
    }

    avgPnrScore /= totalTime;

    return getConnectingFlightScoreForJourney(affectedJourneyID, proposedFlight)*avgPnrScore/proposedFlight.size();
}