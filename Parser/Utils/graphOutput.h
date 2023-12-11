#pragma once
#include<vector>
#include<fstream>
#include "Graph/graph.h"
using namespace std;
template<typename T>int calEdges(ExtendableVector<vector<T>> &adj){
    int edges = 0;
    for(vector<T> cur:adj)
        edges+=cur.size();
    return edges;
}
void outputEdges(ExtendableVector<vector<pair<int,long long>>> adj,ofstream& fw){
    fw<<calEdges<pair<int,long long>>(adj)<<"\n";
    for(int u=0;u<adj.size();++u){
        for(pair<int,long long> e:adj[u]){
            fw<<u<<" "<<e.first<<" "<<e.second<<"\n";
        }
    }
}
void outputEdges(ExtendableVector<vector<int>> adj,ofstream& fw){
    fw<<calEdges<int>(adj)<<"\n";
    for(int u=0;u<adj.size();++u){
        for(int v:adj[u]){
            fw<<u<<" "<<v<<"\n";
        }
    }
}
void graphOutput(ofstream& fw){
    //Output graph

    int modU=uIndexGenerator.getSize();
    int modC=cIndexGenerator.getSize();
    int modV=vIndexGenerator.getSize();
    int modD=dIndexGenerator.getSize();
    int modW=wIndexGenerator.getSize();

    fw<<modU<<" "<<modC<<" "<<modV<<" "<<modD<<" "<<modW<<"\n";

    outputEdges(graphUC, fw);
    outputEdges(graphUV, fw);
    outputEdges(graphCV, fw);
    outputEdges(graphDV, fw);
    outputEdges(graphWD, fw);
    outputEdges(graphWU, fw);

    for(int u=0;u<modU;u++){
        int j_id = uIndexGenerator.getVal(u);
        int pax_cnt=pnrMap[journeyToPnrMap[j_id]]->paxCnt;
        fw<<pax_cnt<<" ";
    }
    fw<<"\n";

    for(int v=0;v<modV;v++){
        pair<int,CLASS_CD>  p = vIndexGenerator.getVal(v);
        int avl_inv;

        int inv_id=p.first;

        cout<<p.second<<"\n";
        cout<<inventoryMap[inv_id]->fcTotalInventory<<" "<<inventoryMap[inv_id]->fcBookedInventory<<endl;
        cout<<inventoryMap[inv_id]->bcTotalInventory<<" "<<inventoryMap[inv_id]->bcBookedInventory<<endl;
        cout<<inventoryMap[inv_id]->pcTotalInventory<<" "<<inventoryMap[inv_id]->pcBookedInventory<<endl;
        cout<<inventoryMap[inv_id]->ecTotalInventory<<" "<<inventoryMap[inv_id]->ecBookedInventory<<endl;

        if(p.second==FC) avl_inv = inventoryMap[inv_id]->fcTotalInventory - inventoryMap[inv_id]->fcBookedInventory;
        else if(p.second==BC) avl_inv = inventoryMap[inv_id]->bcTotalInventory - inventoryMap[inv_id]->bcBookedInventory;
        else if(p.second==PC) avl_inv = inventoryMap[inv_id]->pcTotalInventory - inventoryMap[inv_id]->pcBookedInventory;
        else avl_inv = inventoryMap[inv_id]->ecTotalInventory - inventoryMap[inv_id]->ecBookedInventory;

        fw<<avl_inv<<" ";
    }
    fw<<"\n";
}