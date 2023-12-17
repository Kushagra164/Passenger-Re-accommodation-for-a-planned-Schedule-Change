#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#define F first
#define S second
using namespace std;
class qubo{
    int sz;
    map<int,map<int,long long>> Q;
    //upper triangulation
    public:
        qubo():sz(0){}
        int addVariable(long long val = 0){
            Q[sz][sz] += val;
            ++sz;
            return sz-1;
        }
        void add(int i,int j,long long val){
            Q[i][j]+=val;
        }
        int size(){
            return sz;
        }
        friend ofstream& operator << (ofstream&,qubo& q);
};
ofstream& operator << (ofstream &out, qubo &q){
    //printing -Q
    out<<q.Q.size()<<"\n";
    map<pair<int,int>,long long> res;
    for(auto [i,row]:q.Q){
        for(auto [j,w]: row){
            res[make_pair(min(i,j),max(i,j))] -= w;
        }
    }
    out<<res.size()<<"\n";
    for(auto x:res){
        out<<x.F.F<<" "<<x.F.S<<" "<<x.S<<"\n";
    }
    return out;
}