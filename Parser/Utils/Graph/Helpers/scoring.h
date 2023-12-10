#pragma once
#include "data.h"
#include "timeDiff.h"
using namespace std;

int pnrScore(int journeyId, CLASS_CD proposed){
    Journey* journey = journeyMap[journeyId];
    Pnr* pnr = pnrMap[journeyToPnrMap[journeyId]];
    int pnr_score = 0;
    for (auto &passenger_id: pnr->passengers){
        Passenger* passenger = passengerMap[passenger_id];
        pnr_score += (passenger->ssrCodes.size()*SSR_SCORE);
        pnr_score += PAX_SCORE;
    }
    pnr_score += classScoresMap[proposed];
    return pnr_score;
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