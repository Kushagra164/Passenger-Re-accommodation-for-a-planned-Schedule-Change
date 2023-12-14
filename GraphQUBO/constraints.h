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
    friend class eqConstraints;
};
class eqConstraints{
    vector<pair<vector<int>,long long> > q;
    public:
        void addByIndex(vector<int> indices,int totind = -1,int val = 0){
            vector<int> curEq;
            for(int ind: indices){
                if(curEq.size()<=ind){
                    curEq.resize(ind+1);
                }
                curEq[ind] = 1;
            }
            if(totind!=-1){
                if(curEq.size()<=totind)
                    curEq.resize(totind+1);
                curEq[totind] = -1;
            }
            q.push_back(mp(curEq,val));
        }
        void addInq(ineqConstraints& inq,qubo &Q){
            for(pair<vector<int>,long long> curInq: inq.q){
                curInq.F.resize(Q.size());
                for (int i = 1; i <= curInq.S; i <<= 1)
                {
                    Q.addVariable(0,(curInq.S&i));
                    curInq.F.push_back(i);
                }
                q.push_back(curInq);
            }
        }
        void adjustToQubo(qubo &Q,long long inf){
            long long inf2 = inf*2;
            long long negInf = -inf;
            for(pair<vector<int>,int> curEq: q){
                for(int i=0;i<curEq.F.size();++i){
                    Q.add(i,i,curEq.F[i]*inf2*curEq.S);
                    for(int j=0;j<curEq.F.size();++j){
                        Q.add(i,j,curEq.F[i]*negInf*curEq.F[j]);
                    }
                }
            }
        }
};