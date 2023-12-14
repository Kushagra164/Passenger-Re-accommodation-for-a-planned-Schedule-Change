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
        int size(){
            return q.size();
        }
    friend class eqConstraints;
};
class eqConstraints{
    vector<pair<vector<int>,long long> > q;
    vector<vector<int> > atMaxOneCond;
    public:
        void atMaxOneAdd(vector<int> indices){
            atMaxOneCond.push_back(indices);
        }
        void addInq(ineqConstraints& inq,qubo &Q){
            //return;
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
            // less weightage to inequality constraints
            long long inf2 = (inf/100)*2;
            long long negInf = -(inf/100);
            for(pair<vector<int>,int> curEq: q){
                for(int i=0;i<curEq.F.size();++i){
                    Q.add(i,i,curEq.F[i]*inf2*curEq.S);
                    for(int j=0;j<curEq.F.size();++j){
                        Q.add(i,j,curEq.F[i]*negInf*curEq.F[j]);
                    }
                }
            }
            for(vector<int> indices: atMaxOneCond){
                for(int i=0;i<indices.size();++i){
                    for(int j=i+1;j<indices.size();++j){
                        Q.add(indices[i],indices[j],-inf);
                    }
                }
            }
        }
};