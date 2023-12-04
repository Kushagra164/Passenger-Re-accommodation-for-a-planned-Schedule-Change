#include<bits/stdc++.h>

class uuidGenerator{
    map<string,int> forwardMap;
    map<int,string> revMap;
public:
    int getID(string s) {
        if (forwardMap.find(s) != forwardMap.end())
            return forwardMap[s];
        revMap[forwardMap.size()] = s;
        return forwardMap[s] = forwardMap.size();
    }
    string getString(int uuid){
        if(revMap.find(uuid)!= revMap.end())
            return revMap[uuid];
        assert(false);
    }
};
uuidGenerator scheduleUuidGenerator;
uuidGenerator inventoryUuidGenerator;
uuidGenerator pnrUuidGenerator;
