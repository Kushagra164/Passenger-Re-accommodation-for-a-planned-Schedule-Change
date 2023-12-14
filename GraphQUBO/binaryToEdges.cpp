#include "graph.h"
using namespace std;

int main(int argc,char *argv[]){
    // command line arg
    ifstream graphMapping, quboOutput;
    graphMapping.open(argv[1]);
    quboOutput.open(argv[2]);
    ofstream selectedEdges;
    selectedEdges.open(argv[3]);

    int T,K;
    quboOutput>>T>>K;
    selectedEdges<<K<<"\n";

    vector<vector<pair<int,int>>> selectedUC(K), selectedUV(K),selectedWD(K);

    for(int i=0;i<T;++i){
        int N;
        quboOutput>>N;
        map<int,pair<int,int>> UC = mapInput(graphMapping);
        map<int,pair<int,int>> UV = mapInput(graphMapping);
        map<int,pair<int,int>> WD = mapInput(graphMapping);

        for(int j=0;j<K;++j){
            for(int r=0;r<N;++r){
                char b;
                quboOutput>>b;
                if(b=='1'){
                    if(UC.find(r)!=UC.end()){
                        selectedUC[j].push_back(UC[r]);
                    }
                    else if(UV.find(r)!=UV.end()){
                        selectedUV[j].push_back(UV[r]);
                    }
                    else if(WD.find(r)!=WD.end()){
                        selectedWD[j].push_back(WD[r]);
                    }
                }
            }
        }
    }
//[TODO: Change order of UC and UV]
    for(int i=0;i<K;++i){
        printSelectedEdges(selectedEdges, selectedUC[i]);
        printSelectedEdges(selectedEdges, selectedUV[i]);
        printSelectedEdges(selectedEdges, selectedWD[i]);
    }
}