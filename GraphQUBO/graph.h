#include<fstream>
#include<vector>
#include<map>
#define F first 
#define S second
#define mp make_pair
using namespace std;
void weightedInput(int offsetU, int offsetV,vector<vector<pair<int,long long>>> &g,ifstream& input)
{
    int m;
    input >> m;
    for (int i = 0; i < m; ++i)
    {
        int u, v, w;
        input >> u >> v >> w;
        u += offsetU;
        v += offsetV;
        g[u].push_back(mp(v, w));
        g[v].push_back(mp(u, 1));
    }
}
void unweightedInput(int offsetU, int offsetV,vector<vector<pair<int,long long>>> &g,ifstream& input)
{
    int m;
    input >> m;
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        input >> u >> v;
        u += offsetU;
        v += offsetV;
        g[u].push_back(mp(v, 1));
        g[v].push_back(mp(u, 1));
    }
};
void mappingOutput(ofstream& output, map<pair<int,int>,int> m){
    output<<m.size()<<"\n";
    for(auto p:m){
        output<<p.F.F<<" "<<p.F.S<<" "<<p.S<<"\n";
    }
}
map<int,pair<int,int> > mapInput(ifstream& input){
    int m;
    input>>m;
    map<int,pair<int,int>> res;
    for(int i=0;i<m;++i){
        int u,v,ind;
        input>>u>>v>>ind;
        res[ind] = mp(u,v);
    }
    return res;
}
void printSelectedEdges(ofstream& output,vector<pair<int,int> >& edges){
    output<<edges.size()<<"\n";
    for(pair<int,int> &e: edges){
        output<<e.F<<" "<<e.S<<"\n";
    }
}