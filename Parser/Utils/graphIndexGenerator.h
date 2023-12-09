template<typename T>
class graphIndexGenerator{
    map<T,int> forwardMap;
    map<int,T> revMap;
    public:
        int getIndex(T id) {
            if (forwardMap.find(id) != forwardMap.end()) return forwardMap[id];
            revMap[forwardMap.size()] = id;
            return forwardMap[id] = forwardMap.size();
        }

        T getVal(int idx){
            assert(revMap.find(idx)!= revMap.end());
            return revMap[idx];
        }

        int getSize(){
            return revMap.size();
        }
};

graphIndexGenerator<int> uIndexGenerator;
graphIndexGenerator<int> dIndexGenerator;
graphIndexGenerator<int> wIndexGenerator;
graphIndexGenerator<vector<pair<int,ClassCD>>> cIndexGenerator;
graphIndexGenerator<pair<int,ClassCD>> cIndexGenerator;