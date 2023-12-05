#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<functional>
#include "qubo.h"
#define mp  make_pair
#define F   first
#define S   second
#define ll  long long
using namespace std;
void dfs(int u,vector<vector<pair<int,long long> > > &g,vector<bool> &visit,vector<int> &cur){
    cur.push_back(u);
    visit[u] = true;
    for(auto e:g[u]){
        if(!visit[e.F]){
            dfs(e.F,g,visit,cur);
        }
    }
}
int main(int argc,char *argv[]){
    ios_base::sync_with_stdio(false);
    ofstream graphMapping, quboInput;
    graphMapping.open(argv[1]);
    quboInput.open(argv[2]);
    ifstream input;
    input.open(argv[3]);
    int N = (argc>4)?stoi(argv[4]):2000;
    int M = (argc>5)?stoi(argv[5]):100;
    int U,V,W;
    input>>U>>V>>W;
    int T = U+V+W;
    vector<
        vector<
            pair<int,long long> 
        >
    > g(T);
    function<void(int,int)> weightedInput = [&](int offsetU,int offsetV){
        int m;
        input>>m;
        for(int i=0;i<m;++i){
            int u,v,w;
            input>>u>>v>>w;
            u+=offsetU;
            v+=offsetV;
            g[u].push_back(mp(v,w));
            g[v].push_back(mp(u,-1));
        }
    };
    function<void(int,int)> unweightedInput = [&](int offsetU,int offsetV){
        int m;
        input>>m;
        for(int i=0;i<m;++i){
            int u,v;
            input>>u>>v;
            u+=offsetU;
            v+=offsetV;
            g[u].push_back(mp(v,1));
            g[v].push_back(mp(u,-1));
        }
    };
    weightedInput(0,U); // input UV
    weightedInput(U+V,U); // input WV
    unweightedInput(U+V,0); // input WU

    vector<int> wU(U),wV(V);
    for(auto &x:wU)
        input>>x;
    for(auto &x:wV)
        input>>x;
    vector<bool> visit(T);
    vector<qubo> quboInstances;
    for(int i=0;i<T;++i){
        if(!visit[i]){
            long long val = 0;
            vector<int> cur;
            dfs(i,g,visit,cur);
            // handling UV
            qubo Q;
            vector<pair<vector<int>,int> > eq,inq;
            map<pair<int,int>,int> mappingUV,mappingWV;
            for(int x:cur){
                if(x<U){
                    vector<int> curInd;
                    for(auto e:g[x]){
                        if((e.F>=U)&&(e.F<(U+V))){
                            val+=e.S;
                            curInd.push_back(mappingUV[mp(x,e.F-U)] = Q.addVariable(e.S));
                        }
                    }
                    int e = Q.addVariable(-N);
                    val+=N;
                    vector<int> curEq(e+1);
                    curEq[e] = -1;
                    for(int ind:curInd)
                        curEq[ind] = 1;
                    eq.push_back(mp(curEq,0));
                }
            }
            // handling VU
            for(int x:cur){
                if((x>=U)&&(x<(U+V))){
                    vector<int> curInq(Q.size());
                    for(auto e:g[x]){
                        if(e.F<U){
                            curInq[mappingUV[mp(e.F,x-U)]] = wU[e.F];
                        }
                    }
                    inq.push_back(mp(curInq,wV[x-U]));
                }
            }
            //handling WV
            for(int x:cur){
                if(x>=(U+V)){
                    vector<int> curU;
                    for(auto e:g[x]){
                        if(e.F<U)
                            curU.push_back(e.F);
                    }
                    vector<int> curVar;
                    for(auto e:g[x]){
                        if(e.F>=U){
                            curVar.push_back(mappingWV[mp(x-U-V,e.F-U)] = Q.addVariable());
                            for(int u:curU){
                                if(mappingUV.find(mp(u,e.F-U))!=mappingUV.end()){
                                    Q.add(mappingUV[mp(u,e.F-U)],curVar.back(),e.S);
                                    val+=e.S;
                                }
                            }
                        }
                    }
                    vector<int> curEq(Q.size());
                    for(int ind:curVar)
                        curEq[ind] = 1;
                    eq.push_back(mp(curEq,1));
                }
            }
            //handling inequalities
            for(auto curInq:inq){
                curInq.F.resize(Q.size());
                for(int i=1;i<=curInq.S;i<<=1){
                    curInq.F.push_back(Q.addVariable(i));
                }
                eq.push_back(curInq);
            }
            //handling equalities;
            long long inf = val+1;
            long long negInf = -inf;
            long long inf2 = inf*2;
            for(auto curEq:eq){
                for(int i=0;i<curEq.F.size();++i){
                    Q.add(i,i,curEq.F[i]*inf2*curEq.S);
                    for(int j=0;j<curEq.F.size();++j){
                        Q.add(i,j,curEq.F[i]*negInf*curEq.F[j]);
                    }
                }
            }
            Q.adjust();
            quboInstances.push_back(Q);
            graphMapping<<mappingUV.size()<<"\n";
            for(auto e:mappingUV){
                graphMapping<<e.F.F<<" "<<e.F.S<<" "<<e.S<<"\n";
            }
            graphMapping<<mappingWV.size()<<"\n";
            for(auto e:mappingWV){
                graphMapping<<e.F.F<<" "<<e.F.S<<" "<<e.S<<"\n";
            }
        }
    }
    quboInput<<quboInstances.size()<<"\n";
    for(int i=1;i<=quboInstances.size();++i){
        quboInput<<i<<"\n";
        quboInput<<quboInstances[i-1];
    }
}