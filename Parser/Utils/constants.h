#pragma once
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;
#define CITY_CODE_LENGTH 3
#define RANDOM_INPUT_SIMULATION true
#define PROBABILITY_FLIGHT_CANCELLED 0.6
#define PROBABILITY_FLIGHT_AFFECTED 0.01
#define SSR_SCORE 1
#define PAX_SCORE 1
#define MAXIMUM_ALLOWED_TIME_DIFF Time(72, 0)
#define MINIMUM_CONNECTING_TIME Time(1, 0)
#define MAXIMUM_ALLOWED_TIME_DIFF_FOR_CONNECTING Time(12, 0)
#define ARRIVAL_DELAY_LT_6_SCORE 7
#define ARRIVAL_DELAY_LT_12_SCORE 5
#define ARRIVAL_DELAY_LT_24_SCORE 4
#define ARRIVAL_DELAY_LT_48_SCORE 3
#define EQUIPMENT_SCORE 1
#define CITYPAIR_SCORE 0
#define DEPARTURE_DELAY_LT_6_SCORE 7
#define DEPARTURE_DELAY_LT_12_SCORE 5
#define DEPARTURE_DELAY_LT_24_SCORE 4
#define DEPARTURE_DELAY_LT_48_SCORE 3
#define CLASS_UPGRADE_ALLOWED true
#define CLASS_DOWNGRADE_ALLOWED true
#define MAXIMUM_ALLOWED_FLIGHTS_PER_JOURNEY 5
#define MAXIMUM_ALLOWED_FLIGHTS_PER_CANCELLED_FLIGHT 5
#define MAXIMUM_ALLOWED_CONNECTING_FLIGHTS_PER_JOURNEY 5
#define CARRIER_CODE_LENGTH 2
#define MINIMUM_PROPOSED_FLIGHTS 2
#define RANDOM_SEED 790

enum ACTION_CD
{
    GN,
    HK,
    HL,
    XX
};
static ACTION_CD getActionCode(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if (str == "GN")
        return GN;
    if (str == "HK")
        return HK;
    if (str == "HL")
        return HL;
    if (str == "XX")
        return XX;
    cout << "Action Code not found:" << str << endl;
    exit(-1);
}

enum CLASS_CD
{
    FC,
    BC,
    PC,
    EC
};
static CLASS_CD getClassCode(string str)
{
    if (str == "FirstClass")
        return FC;
    if (str == "BusinessClass")
        return BC;
    if (str == "PremiumEconomyClass")
        return PC;
    if (str == "EconomyClass")
        return EC;
    cout << "Class Code not found: " << str << endl;
    exit(-1);
}

enum SPECIAL_NAME1
{
    SPECIAL_NAME1_NULL,
    INS,
    INF,
    CHD,
    ADT,
    S65,
    UNN
};
static SPECIAL_NAME1 getSpecialName1(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if (str == "INS")
        return INS;
    if (str == "INF")
        return INF;
    if (str == "CHD")
        return CHD;
    if (str == "ADT")
        return ADT;
    if (str == "UNN")
        return UNN;
    if (str == "S65")
        return S65;
    return SPECIAL_NAME1_NULL;
}

enum SPECIAL_NAME2
{
    SPECIAL_NAME2_NULL,
    NRPS,
    NRSA
};
static SPECIAL_NAME2 getSpecialName2(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if (str == "NRPS")
        return NRPS;
    if (str == "NRSA")
        return NRSA;
    return SPECIAL_NAME2_NULL;
}

enum SSR_CD
{
    SSR_CD_NULL,
    WCHR,
    WCHS,
    WCHC,
    LANG,
    MAAS,
    UNMR,
    BLND,
    DEAF,
    EXST,
    MEAL,
    NSST
};
static SSR_CD getSSR(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    if (str == "WCHR")
        return WCHR;
    if (str == "WCHS")
        return WCHS;
    if (str == "WCHC")
        return WCHC;
    if (str == "LANG")
        return LANG;
    if (str == "MAAS")
        return MAAS;
    if (str == "UNMR")
        return UNMR;
    if (str == "BLND")
        return BLND;
    if (str == "DEAF")
        return DEAF;
    if (str == "EXST")
        return EXST;
    if (str == "MEAL")
        return MEAL;
    if (str == "NSST")
        return NSST;
    return SSR_CD_NULL;
}

enum LOYALTY_CD
{
    LOYALTY_NULL,
    PLATINUM,
    GOLD,
    SILVER
};
static LOYALTY_CD getLoyalty(string str)
{
    if (str == "Platinum")
        return PLATINUM;
    if (str == "Gold")
        return GOLD;
    if (str == "Silver")
        return SILVER;
    return LOYALTY_NULL;
}

enum SCHEDULE_STATUS
{
    SCHEDULE_SCHEDULED,
    SCHEDULE_PLANNED,
    SCHEDULE_CANCELLED
};
static SCHEDULE_STATUS getScheduleStatus(string status)
{
    transform(status.begin(), status.end(), status.begin(), ::toupper);
    if (status == "SCHEDULED")
        return SCHEDULE_SCHEDULED;
    if (status == "PLANNING")
        return SCHEDULE_PLANNED;
    if (status == "CANCELLED")
        return SCHEDULE_CANCELLED;
    cout << "Schedule status not found:" << status << endl;
    exit(-1);
}

enum INVENTORY_STATUS
{
    INVENTORY_SCHEDULED,
    INVENTORY_PLANNED,
    INVENTORY_CANCELLED,
    INVENTORY_DELAYED
};
static INVENTORY_STATUS getInventoryStatus(string status)
{
    transform(status.begin(), status.end(), status.begin(), ::toupper);
    if (status == "SCHEDULED")
        return INVENTORY_SCHEDULED;
    if (status == "PLANNING")
        return INVENTORY_PLANNED;
    if (status == "CANCELLED")
        return INVENTORY_CANCELLED;
    if (status == "DELAYED")
        return INVENTORY_DELAYED;
    cout << "Inventory status not found:" << status << endl;
    exit(-1);
}

map<CLASS_CD, int> classScoresMap{{(CLASS_CD)FC, 25}, {(CLASS_CD)BC, 20}, {(CLASS_CD)PC, 15}, {(CLASS_CD)EC, 10}};
map<LOYALTY_CD, int> loyaltyScoresMap{{(LOYALTY_CD)PLATINUM, 9}, {(LOYALTY_CD)GOLD, 8}, {(LOYALTY_CD)SILVER, 7}, {(LOYALTY_CD)LOYALTY_NULL, 0}};