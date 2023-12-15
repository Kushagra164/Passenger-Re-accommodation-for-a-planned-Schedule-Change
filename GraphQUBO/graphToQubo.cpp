#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include "constraints.h"
#include "graph.h"
#define mp make_pair
#define F first
#define S second
#define ll long long
using namespace std;

void dfs(int u, vector<vector<pair<int, long long>>> &g, vector<bool> &visit, vector<int> &cur)
{
    cur.push_back(u);
    visit[u] = true;
    for (auto e : g[u])
    {
        if (!visit[e.F])
        {
            dfs(e.F, g, visit, cur);
        }
    }
}
int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);

    // command line parsing
    ofstream graphMapping, quboInput;
    graphMapping.open(argv[1]);
    quboInput.open(argv[2]);
    ifstream input;
    input.open(argv[3]);
    int N = (argc > 4) ? stoi(argv[4]) : 500000;
    int M = (argc > 5) ? stoi(argv[5]) : 50000;

    // input handling
    int U, C, V, D, W;
    input >> U >> C >> V >> D >> W;
    const int T = U + C + V + D + W;

    vector<
        vector<
            pair<int, long long>>>
        g(T);
    
    const int offsetU = 0;
    const int offsetC = U;
    const int offsetV = U + C;
    const int offsetD = offsetV + V;
    const int offsetW = offsetD + D;

    function<int(int,int,int)> get = [](int ind,int L,int R){
        return ((ind>=L)&&(ind<R))?(ind-L):-1; 
    };
    function<int(int)> getU = [&](int x){
        return get(x, offsetU, offsetC);
    };

    function<int(int)> getC = [&](int x){
        return get(x, offsetC, offsetV);
    };

    function<int(int)> getV = [&](int x){
        return get(x, offsetV, offsetD);
    };

    function<int(int)> getD = [&](int x){
        return get(x, offsetD, offsetW);
    };

    function<int(int)> getW = [&](int x){
        return get(x, offsetW, T);
    };

    weightedInput(offsetU, offsetC, g, input);          // input UC
    weightedInput(offsetU, offsetV, g, input);          // input UV
    unweightedInput(offsetC, offsetV, g, input);        // input CV
    unweightedInput(offsetD, offsetV, g, input);        // input DV
    unweightedInput(offsetW, offsetD, g, input);        // input WD
    unweightedInput(offsetW, offsetU, g, input);        // input WU

    vector<int> wU(U), wV(V);
    for (auto &x : wU)
        input >> x;
    for (auto &x : wV)
        input >> x;

    vector<bool> visit(T);
    vector<qubo> quboInstances;

    for (int i = 0; i < T; ++i)
    {
        if (!visit[i])
        {
            long long val = 0;
            vector<int> cur;

            dfs(i, g, visit, cur);
            // non relocatable node
            if(cur.size()==1){
                int x = cur[0];
                if(x<U)cout<<"U"<<endl;
                else if(x<offsetV)cout<<"C"<<endl;
                else if(x<offsetD)cout<<"V"<<endl;
                else if(x<offsetW)cout<<"D"<<endl;
                else if(x<T)cout<<"W"<<endl;
                else cout<<"error"<<endl;
                continue;
            }
            qubo Q;
            eqConstraints eq;
            ineqConstraints inq;
            map<pair<int, int>, int> mappingUC, mappingUV, mappingWD;

            // handling U
            for (int x : cur)
            {
                int u = getU(x);
                if (u!=-1)
                {
                    vector<int> curInd;
                    for (auto e : g[x])
                    {
                        //handling C
                        int c = getC(e.F);
                        if(c!=-1)
                        {
                            val += e.S;
                            curInd.push_back(mappingUC[mp(u, c)] = Q.addVariable(e.S));
                        }

                        //handling V
                        int v = getV(e.F);
                        if(v!=-1)
                        {
                            val += (e.S);
                            curInd.push_back(mappingUV[mp(u, v)] = Q.addVariable(e.S));
                        }
                    }
                    // contribution to rellocation in scoring
                    val += (N*1ll*(curInd.size()));
                    for(int ind:curInd)
                        Q.add(ind,ind,N);
                    // at max passenger can be seated once
                    eq.atMaxOneAdd(curInd);
                }
            }

            // handling V constraints
            for (int x : cur)
            {
                int v = getV(x);
                if (v!=-1)
                {
                    vector<int> curInq(Q.size());
                    for (auto e : g[x])
                    {
                        // handling c
                        int c = getC(e.F);
                        if(c!=-1){
                            for(auto f : g[e.F]){
                                int u = getU(f.F);
                                if(u!=-1){
                                    curInq[mappingUC[mp(u,c)]] = wU[u];
                                }
                            }
                        }

                        // handling u
                        int u = getU(e.F);
                        if(u!=-1){
                            curInq[mappingUV[mp(u,v)]] = wU[u];
                        }
                    }
                    inq.add(curInq,wV[v]);
                }
            }

            // handling W
            for (int x : cur)
            {
                int w = getW(x);
                if (w!=-1)
                {
                    //getting all U
                    vector<int> curU;
                    for (auto e : g[x])
                    {
                        int u = getU(e.F);
                        if (u!=-1)
                            curU.push_back(u);
                    }

                    vector<int> curVar;
                    for (auto e : g[x])
                    {
                        int d = getD(e.F);
                        if (d!=-1)
                        {
                            curVar.push_back(mappingWD[mp(w, d)] = Q.addVariable());
                            for(auto f : g[e.F]){
                                int v = getV(f.F);
                                if(v!=-1){
                                    for (int u : curU)
                                    {
                                        if (mappingUV.find(mp(u, v)) != mappingUV.end())
                                        {
                                            Q.add(mappingUV[mp(u, v)], curVar.back(), wU[u]*1ll*M);
                                            val += (wU[u]*1ll*M);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // at max flight can be reschedule to one other flight
                    eq.atMaxOneAdd(curVar);
                }
            }

            eq.addInq(inq, Q);
            long long inf = ((Q.size()+1000)/1000)*val*Q.size()*Q.size();
            eq.adjustToQubo(Q, inf);
            quboInstances.push_back(Q);

            cout<<"Inf: "<<inf<<" QuboSize: "<<Q.size()<<endl;

            mappingOutput(graphMapping, mappingUC);
            mappingOutput(graphMapping, mappingUV);
            mappingOutput(graphMapping, mappingWD);
        }
    }
    
    quboInput << quboInstances.size() << "\n";
    for (int i = 1; i <= quboInstances.size(); ++i)
    {
        quboInput << i << "\n";
        quboInput << quboInstances[i - 1];
    }
}