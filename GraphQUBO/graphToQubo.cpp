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
    int N = (argc > 4) ? stoi(argv[4]) : 2000;
    int M = (argc > 5) ? stoi(argv[5]) : 100;

    // input handling
    int U, V, W;
    input >> U >> V >> W;
    int T = U + V + W;
    vector<
        vector<
            pair<int, long long>>>
        g(T);
    weightedInput(0, U, g, input);        // input UV
    weightedInput(U + V, U, g,  input);   // input WV
    unweightedInput(U + V, 0, g, input);  // input WU

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

            qubo Q;
            eqConstraints eq;
            ineqConstraints inq;
            map<pair<int, int>, int> mappingUV, mappingWV;

            // handling UV
            // [TODO: sarthak handle UVC]
            for (int x : cur)
            {
                if (x < U)
                {
                    vector<int> curInd;
                    for (auto e : g[x])
                    {
                        if ((e.F >= U) && (e.F < (U + V)))
                        {
                            val += e.S;
                            curInd.push_back(mappingUV[mp(x, e.F - U)] = Q.addVariable(e.S));
                        }
                    }

                    val += N;
                    eq.addByIndex(curInd,Q.addVariable(-N));;
                }
            }

            // handling VU
            //[TODO: sarthak handle VCU]
            for (int x : cur)
            {
                if ((x >= U) && (x < (U + V)))
                {
                    vector<int> curInq(Q.size());
                    for (auto e : g[x])
                    {
                        if (e.F < U)
                        {
                            curInq[mappingUV[mp(e.F, x - U)]] = wU[e.F];
                        }
                    }
                    inq.add(curInq,wV[x-U]);
                }
            }

            // handling WV
            for (int x : cur)
            {
                if (x >= (U + V))
                {
                    vector<int> curU;
                    for (auto e : g[x])
                    {
                        if (e.F < U)
                            curU.push_back(e.F);
                    }
                    vector<int> curVar;
                    for (auto e : g[x])
                    {
                        if (e.F >= U)
                        {
                            curVar.push_back(mappingWV[mp(x - U - V, e.F - U)] = Q.addVariable());
                            for (int u : curU)
                            {
                                if (mappingUV.find(mp(u, e.F - U)) != mappingUV.end())
                                {
                                    Q.add(mappingUV[mp(u, e.F - U)], curVar.back(), e.S);
                                    val += e.S;
                                }
                            }
                        }
                    }
                    eq.addByIndex(curVar,-1,1);
                }
            }

            eq.addInq(inq, Q);
            eq.adjustToQubo(Q,val+1);
            quboInstances.push_back(Q);

            mappingOutput(graphMapping, mappingUV);
            mappingOutput(graphMapping, mappingWV);
        }
    }
    quboInput << quboInstances.size() << "\n";
    for (int i = 1; i <= quboInstances.size(); ++i)
    {
        quboInput << i << "\n";
        quboInput << quboInstances[i - 1];
    }
}