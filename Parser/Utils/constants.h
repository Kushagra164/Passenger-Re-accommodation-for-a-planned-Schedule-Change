#pragma once
#include<map>
#include<algorithm>
using namespace std;
#define CITY_CODE_LENGTH 3
#define SCHEDULE_LEVEL_CANCELLATION false
#define INVENTORY_LEVEL_CANCELLATION true
#define SSR_SCORE 1
#define PAX_SCORE 1
#define MAXIMUM_ALLOWED_TIME_DIFF Time(72, 0)
#define MINIMUM_CONNECTING_TIME Time(1,0)
#define ARRIVAL_DELAY_LT_6_SCORE 7
#define ARRIVAL_DELAY_LT_12_SCORE 5
#define ARRIVAL_DELAY_LT_24_SCORE 4
#define ARRIVAL_DELAY_LT_48_SCORE 3
#define EQUIPMENT_SCORE 5
#define CITYPAIR_SCORE 0
#define DEPARTURE_DELAY_LT_6_SCORE 7
#define DEPARTURE_DELAY_LT_12_SCORE 5
#define DEPARTURE_DELAY_LT_24_SCORE 4
#define DEPARTURE_DELAY_LT_48_SCORE 3
#define CLASS_UPGRADE_ALLOWED  true
#define CLASS_DOWNGRADE_ALLOWED  true
#define MAXIMUM_ALLOWED_FLIGHTS_PER_JOURNEY 5
#define MAXIMUM_ALLOWED_FLIGHTS_PER_CANCELLED_FLIGHT 5
#define MAXIMUM_ALLOWED_CONNECTIONS_PER_JOURNEY 2
#define CARRIER_CODE_LENGTH 2

enum ACTION_CD{HK,PD};
static ACTION_CD getActionCode(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if(str=="HK") return HK;
    if(str=="PD") return PD;
    cout<<"Action Code not found:"<<str<<endl;
}

enum CLASS_CD{FC,BC,PC,EC};
static CLASS_CD getClassCode(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if(str=="FC") return FC;
    if(str=="BC") return BC;
    if(str=="PC") return PC;
    if(str=="EC") return EC;
    cout<<"Class Code not found: "<<str<<endl;
}

enum SPECIAL_NAME1 {SPECIAL_NAME1_NULL,INS,INF,CHD,ADT,UNN,S65};
static SPECIAL_NAME1 getSpecialName1(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if(str=="INS") return INS;
    if(str=="INF") return INF;
    if(str=="CHD") return CHD;
    if(str=="ADT") return ADT;
    if(str=="UNN") return UNN;
    if(str=="S65") return S65;
    return SPECIAL_NAME1_NULL;
}

enum SPECIAL_NAME2 {SPECIAL_NAME2_NULL,NRPS,NRSA};
static SPECIAL_NAME2 getSpecialName2(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if(str=="NRPS") return NRPS;
    if(str=="NRSA") return NRSA;
    return SPECIAL_NAME2_NULL;
}

enum SSR_CD {SSR_CD_NULL,WCHR,WCHS,WCHC,LANG,MAAS,UNMR,BLND,DEAF,EXST,MEAL,NSST};
static SSR_CD getSSR(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if(str=="WCHR") return WCHR;
    if(str=="WCHS") return WCHS;
    if(str=="WCHC") return WCHC;
    if(str=="LANG") return LANG;
    if(str=="MAAS") return MAAS;
    if(str=="UNMR") return UNMR;
    if(str=="BLND") return BLND;
    if(str=="DEAF") return DEAF;
    if(str=="EXST") return EXST;
    if(str=="MEAL") return MEAL;
    if(str=="NSST") return NSST;
    return SSR_CD_NULL;
}

enum SCHEDULE_STATUS{SCHEDULE_SCHEDULED,SCHEDULE_PLANNED,SCHEDULE_CANCELLED};
static SCHEDULE_STATUS getScheduleStatus(string status){
    transform(status.begin(), status.end(), status.begin(), ::toupper);
    if(status=="SCHEDULED") return SCHEDULE_SCHEDULED;
    if(status=="PLANNING") return SCHEDULE_PLANNED;
    if(status=="CANCELLED") return SCHEDULE_CANCELLED;
    cout<<"Schedule status not found:"<<status<<endl;
}

enum INVENTORY_STATUS{INVENTORY_SCHEDULED,INVENTORY_PLANNED,INVENTORY_CANCELLED,INVENTORY_DELAYED};
static INVENTORY_STATUS getInventoryStatus(string status){
    transform(status.begin(), status.end(), status.begin(), ::toupper);
    if(status=="SCHEDULED") return INVENTORY_SCHEDULED;
    if(status=="PLANNING") return INVENTORY_PLANNED;
    if(status=="CANCELLED") return INVENTORY_CANCELLED;
    if(status=="DELAYED") return INVENTORY_DELAYED;
    cout<<"Inventory status not found:"<<status<<endl;
}

map<CLASS_CD,int> classScoresMap{{(CLASS_CD)FC,25},{(CLASS_CD)BC,20},{(CLASS_CD)PC,15},{(CLASS_CD)EC,10}};
