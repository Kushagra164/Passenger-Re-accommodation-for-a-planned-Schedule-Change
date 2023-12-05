#include "graph.h"
#define mp  make_pair
#define F   first
#define S   second
#define ll  long long
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

    vector<vector<pair<int,int>>> selectedUV(K),selectedWV(K);

    for(int i=0;i<T;++i){
        int N;
        quboOutput>>N;
        map<int,pair<int,int>> UV = mapInput(graphMapping);
        map<int,pair<int,int>> WV = mapInput(graphMapping);

        for(int j=0;j<K;++j){
            for(int r=0;r<N;++r){
                int b;
                quboOutput>>b;
                if(b){
                    if(UV.find(r)!=UV.end()){
                        selectedUV[j].push_back(UV[r]);
                    }
                    else if(WV.find(r)!=WV.end()){
                        selectedWV[j].push_back(WV[r]);
                    }
                }
            }
        }
    }

    for(int i=0;i<K;++i){
        printSelectedEdges(selectedEdges,selectedUV[i]);
        printSelectedEdges(selectedEdges,selectedWV[i]);
    }
}