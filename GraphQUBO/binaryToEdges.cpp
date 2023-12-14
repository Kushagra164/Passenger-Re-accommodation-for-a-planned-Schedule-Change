#include "graph.h"
#include <iostream>
using namespace std;

int main(int argc,char *argv[]){
    // command line arg
    ifstream graphMapping, quboOutput;
    graphMapping.open(argv[1]);
    quboOutput.open(argv[2]);
    ofstream selectedEdges;
    selectedEdges.open(argv[3]);

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
                    selectedUC.push_back(UC[r]);
                }
                else if(UV.find(r)!=UV.end()){
                    auto [u,v] = UV[r];
                    if(visitU[u]){
                        cout<<i<<" "<<visitU[u]<<" "<<(i+1)<<endl;
                    }
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

    printSelectedEdges(selectedEdges, selectedUC);
    printSelectedEdges(selectedEdges, selectedUV);
    printSelectedEdges(selectedEdges, selectedWD);

}