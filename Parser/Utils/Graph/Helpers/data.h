#pragma once
#include<map>
#include<set>
#include "../../extendableVector.h"
#include "../../DateTime/dateTime.h"
#include "../../../DataModels/inventory.h"
#include "../../../DataModels/schedule.h"
#include "../../../DataModels/journey.h"
#include "../../../DataModels/passenger.h"
#include "../../../DataModels/pnr.h"
#include "../../../DataModels/flight.h"
using namespace std;

set<int> CancelledFlights;                                  //set<InventoryID>
map<int,Time> DelayedFlights;                               //map<InventoryID,Time delay>

ExtendableVector <vector<int>> graphWU;                     //Unweighted graph from InventoryID(s) of Cancelled Flights to Affected JourneyID(s)
vector<int> AffectedJourneys;                               //Nodes U

ExtendableVector<vector<int>> graphWD;                      //Unweighted graph from cancelled flight InventoryID(s) to possible flight solution InventoryID(s)

ExtendableVector<vector<pair<int,long long>>> graphUV;      //Weighted graph from affected JourneyID(s) to possible flight solution InventoryID(s) cum classCD
ExtendableVector<vector<int>> graphDV;                      //Unweighted graph from flight Inventory ID(s) to their possible classes

map<char,string> cabinToClassMap;

ExtendableVector <vector<pair<int,long long>>> graphUC;     //Weighted graph from affected JourneyID(s) to possible connection solutions
ExtendableVector <vector<int>> graphCV;                     //Unweighted graph from connection solution to its solution flight Inventory ID(s)