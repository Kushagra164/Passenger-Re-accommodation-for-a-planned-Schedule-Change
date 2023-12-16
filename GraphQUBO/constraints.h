#include "qubo.h"
#define F first 
#define S second 
#define mp make_pair
using namespace std;
class ineqConstraints{
    vector<pair<vector<int>,long long> > q;
    public:
        void add(vector<int> inq, long long val){
            q.push_back(mp(inq,val));
        }
        void atMaxOne(vector<int> indices){
            vector<int> inq;
            for(int ind: indices){
                if(inq.size()<=ind){
                    inq.resize(ind+1);
                }
                inq[ind] = 1;
            }
            q.push_back(mp(inq,1));
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
        for(auto x:q.F){
            out<<x<<" ";
        }
        out<<"\n";
    }
    return out;
}