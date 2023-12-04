#include<bits/stdc++.h>

class graphIndexGenerator{
    map<int,int> forwardMap;
    map<int,int> revMap;
    public:
        int getIndex(int id) {
            if (forwardMap.find(id) != forwardMap.end()) return forwardMap[id];
            revMap[forwardMap.size()] = id;
            return forwardMap[id] = forwardMap.size();
        }

        int getID(int idx){
            assert(revMap.find(idx)!= revMap.end());
            return revMap[idx];
        }

        int getSize(){
            return revMap.size();
        }
};
graphIndexGenerator uIndexGenerator;
graphIndexGenerator wIndexGenerator;


class vGraphIndexGenerator{
    map<pair<int,ClassCDs>,int> forwardMap;
    map<int,pair<int,ClassCDs>> revMap;
public:
    int getIndex(pair<int,ClassCDs> id) {
        if (forwardMap.find(id) != forwardMap.end()) return forwardMap[id];
        revMap[forwardMap.size()] = id;
        return forwardMap[id] = forwardMap.size();
    }

    pair<int,ClassCDs> getID(int idx){
        if(revMap.find(idx)!= revMap.end())
            return revMap[idx];
        assert(false);
    }

    int getSize(){
        return revMap.size();
    }
};

vGraphIndexGenerator vIndexGenerator;