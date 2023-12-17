#include "qubo.h"
#define F first 
#define S second 
#define mp make_pair
using namespace std;
class ineqConstraints{
    vector<pair<map<int,int>,long long> > q;
    public:
        void add(map<int,int> inq, long long val){
            q.push_back(mp(inq,val));
        }
        void atMaxOne(vector<int> indices){
            map<int,int> cur;
            for(auto x:indices)
                cur[x] = 1;
            q.push_back(make_pair(cur, 1));
        }
        int size(){
            return q.size();
        }
        friend ofstream& operator << (ofstream&, ineqConstraints& q);
};
ofstream& operator << (ofstream &out, ineqConstraints &inq){
    out<<inq.size()<<"\n";
    for(auto q:inq.q){
        out<<q.S<<"\n";
        out<<q.F.size()<<"\n";
        for(auto x:q.F){
            out<<x.F<<" "<<x.S<<"\n";
        }
    }
    return out;
}