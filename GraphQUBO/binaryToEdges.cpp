#include "graph.h"
#include <iostream>
#include <functional>
using namespace std;

int main(int argc,char *argv[]){
    // command line arg
    ifstream graphMapping, quboOutput, input;
    graphMapping.open(argv[1]);
    quboOutput.open(argv[2]);
    input.open(argv[3]);
    ofstream selectedEdges;
    selectedEdges.open(argv[4]);
    

    // graph input handling
    int U, C, V, D, W;
    input >> U >> C >> V >> D >> W;
    const int gT = U + C + V + D + W;

    vector<
        vector<
            pair<int, long long>>>
        g(gT);
    
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
        return get(x, offsetW, gT);
    };

    weightedInput(offsetU, offsetC, g, input);          // input UC
    weightedInput(offsetU, offsetV, g, input);          // input UV
    unweightedInput(offsetC, offsetV, g, input);        // input CV
    unweightedInput(offsetD, offsetV, g, input);        // input DV
    unweightedInput(offsetW, offsetD, g, input);        // input WD
    unweightedInput(offsetW, offsetU, g, input);        // input WU

    vector<int> wU(U), wV(V),calWV(V);
    for (auto &x : wU)
        input >> x;
    for (auto &x : wV)
        input >> x;

    int T;
    quboOutput>>T;

    vector<pair<int,int>> selectedUC, selectedUV, selectedWD;
    vector<int> visitU(100000),visitW(100000);
    for(int i=0;i<T;++i){
        int N;
        quboOutput>>N;
        map<int,pair<int,int>> UC = mapInput(graphMapping);
        map<int,pair<int,int>> UV = mapInput(graphMapping);
        map<int,pair<int,int>> WD = mapInput(graphMapping);

        for(int r=0;r<N;++r){
            char b;
            quboOutput>>b;
            if(b=='1'){
                if(UC.find(r)!=UC.end()){
                    auto [u,c] = UC[r];
                    if(visitU[u]){
                        cout<<i<<" "<<visitU[u]<<" "<<(i+1)<<endl;
                    }
                    visitU[u] = (i+1);
                    for(auto e:g[c+U]){
                        int v = getV(e.first);
                        if(v!=-1){
                            calWV[v]+=wU[u];
                        }
                    }
                    selectedUC.push_back(UC[r]);
                }
                else if(UV.find(r)!=UV.end()){
                    auto [u,v] = UV[r];
                    if(visitU[u]){
                        cout<<i<<" "<<visitU[u]<<" "<<(i+1)<<endl;
                    }
                    calWV[v]+=wU[u];
                    visitU[u] = (i+1);
                    selectedUV.push_back(UV[r]);
                }
                else if(WD.find(r)!=WD.end()){
                    auto [w,d] = WD[r];
                    if(visitW[w]){
                        cout<<i<<" w "<<visitW[w]<<" "<<(i+1)<<endl;
                    }
                    visitW[w] = (i+1);
                    selectedWD.push_back(WD[r]);
                }
            }
        }
    }
    for(int i=0;i<V;++i){
        if(calWV[i]>wV[i]){
            cout<<"Capacity violated: "<<i<<endl;
        }
    }
    printSelectedEdges(selectedEdges, selectedUV);
    printSelectedEdges(selectedEdges, selectedUC);
    printSelectedEdges(selectedEdges, selectedWD);

}