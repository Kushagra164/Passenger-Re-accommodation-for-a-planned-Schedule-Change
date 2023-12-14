#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
class qubo{
    vector<vector<long long> > Q;
    vector<bool> initialValue;
    //upper triangulation
    void adjust(){
        for(int i=0;i<Q.size();++i){
            for(int j=0;j<i;++j){
                Q[j][i]+=Q[i][j];
                Q[i][j]=0;
            }
        }
    }
    public:
        qubo(int _n){
            Q.resize(_n);
            for(auto &row:Q){
                row.resize(_n);
            }
            initialValue.resize(_n);
        }
        qubo(){}
        int addVariable(long long val = 0, bool curValue = false){
            for(auto &row:Q){
                row.push_back(0);
            }
            vector<long long> temp(Q.size());
            temp.push_back(val);
            Q.push_back(temp);
            initialValue.push_back(curValue);
            return Q.size()-1;
        }
        void add(int i,int j,long long val){
            Q[i][j]+=val;
        }
        int size(){
            return Q.size();
        }
        friend ofstream& operator << (ofstream&,qubo& q);
};
ofstream& operator << (ofstream &out, qubo &q){
    q.adjust();
    //printing -Q
    for(bool val:q.initialValue){
        out<<val<<" ";
    }
    out<<"\n";
    out<<q.Q.size()<<"\n";
    for(auto &row: q.Q){
        for(auto &elem: row){
            out<<(-elem)<<" ";
        }
        out<<"\n";
    }
    return out;
}