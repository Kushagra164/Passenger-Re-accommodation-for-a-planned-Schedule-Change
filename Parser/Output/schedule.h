#pragma once
#include<string>
#include<fstream>
#include "../DataModels/schedule.h"
#include "../DataModels/flight.h"
#include "../Utils/constants.h"
#include "../Utils/DateTime/dateTime.h"
#include "../Utils/uuidGenerator.h"
using namespace std;

void getScheduleOutput(ofstream& fw){
    for(auto x:scheduleMap){
        int curScheduleID=x.first;
        Schedule* curSchedule=x.second;

        fw<<scheduleUuidGenerator.getString(curScheduleID)<<" ";
        fw<<curSchedule->carrierCD<<" ";
        fw<<curSchedule->flightNum<<" ";
        fw<<curSchedule->equipmentNo.first<<" "<<curSchedule->equipmentNo.second<<" ";
        fw<<flightNumberMap[curSchedule->flightNum].srcCity<<" "<<flightNumberMap[curSchedule->flightNum].destCity<<" ";
        fw<<curSchedule->departureTime.to_string()<<" "<<curSchedule->arrivalTime.to_string()<<" ";
        fw<<curSchedule->startDate.to_string()<<" "<<curSchedule->endDate.to_string()<<" ";

        if(curSchedule->status == SCHEDULE_STATUS::SCHEDULE_SCHEDULED) fw<<"Scheduled"<<" ";
        else if(curSchedule->status == SCHEDULE_STATUS::SCHEDULE_CANCELLED) fw<<"Cancelled"<<" ";
        else if(curSchedule->status == SCHEDULE_STATUS::SCHEDULE_PLANNED) fw<<"Planned"<<" ";

        fw<<curSchedule->flightPattern<<endl;
    }
}