#include<bits/stdc++.h>
#define mp  make_pair
#define F   first
#define S   second
#define ll  long long
using namespace std;
int main(int argc,char *argv[]){
    ios_base::sync_with_stdio(false);
    ifstream graphMapping, quboOutput;
    graphMapping.open(argv[1]);
    quboOutput.open(argv[2]);
    ofstream selectedEdges;
    selectedEdges.open(argv[3]);
    int T,K;
    quboOutput>>T>>K;
    selectedEdges<<K<<"\n";
    function<map<int,pair<int,int>>()> mapInput = [&](){
        int m;
        graphMapping>>m;
        map<int,pair<int,int>> res;
        for(int i=0;i<m;++i){
            int u,v,ind;
            graphMapping>>u>>v>>ind;
            res[ind] = mp(u,v);
        }
        return res;
    };
    vector<vector<pair<int,int>>> selectedUV(K),selectedWV(K);
    for(int i=0;i<T;++i){
        int N;
        quboOutput>>N;
        map<int,pair<int,int>> UV = mapInput();
        map<int,pair<int,int>> WV = mapInput();
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
        selectedEdges<<selectedUV[i].size()<<"\n";
        for(auto p:selectedUV[i])
            selectedEdges<<p.F<<" "<<p.S<<"\n";
        selectedEdges<<selectedUV[i].size()<<"\n";
        for(auto p:selectedWV[i])
            selectedEdges<<p.F<<" "<<p.S<<"\n";
    }
}