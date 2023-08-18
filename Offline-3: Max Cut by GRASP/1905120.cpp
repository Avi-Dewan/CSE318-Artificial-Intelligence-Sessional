#include <iostream>
#include <fstream>
#include<vector>
#include<set>


using namespace std;

#define MAX_WEIGHT 100

int numVertices, numEdges;
vector<pair<int, int>> adjList[(int)1e5 + 3];
set<int> X, Y, Z;

int sigmaX(int v) {
    int sumWt = 0;
    for(auto u: adjList[v]) {
        if(X.count(u.first)) sumWt += u.second;
    }
    return sumWt;
}

int sigmaY(int v) {
    int sumWt = 0;
    for(auto u: adjList[v]) {
        if(Y.count(u.first)) sumWt += u.second;
    }
    return sumWt;
}

int semiGreedy(double alpha) {

    int w_min = MAX_WEIGHT+3, w_max = -MAX_WEIGHT-3;

    for(int u = 1; u < numVertices; u++) {
        for(auto v: adjList[u]) {
            w_min = min(v.second, w_min);
            w_max = max(v.second, w_max);
        }
    }

    int cut_off = w_min + alpha*(w_max - w_min);

    vector<pair<int, int>> RCLedge; 

    for(int u = 1; u < numVertices; u++) {
        for(auto v: adjList[u]) {
            if(v.second >= cut_off) {
                RCLedge.push_back(make_pair(u, v.first));
            }
        }
    }

     // select random edge from rcle
    int idx = rand() % RCLedge.size();
    // remove from Z and insert into x and y
    Z.erase(RCLedge[idx].first);
    X.insert(RCLedge[idx].first);
    Z.erase(RCLedge[idx].second);
    Y.insert(RCLedge[idx].second);
  
    while (Z.size() != 0)
    {
        cout << "semi greedy: " << ((numVertices-Z.size())/(float)numVertices)*100 << "% done" << endl;
        w_min = MAX_WEIGHT*numEdges;
        w_max = -MAX_WEIGHT*numEdges;

        for(auto v: Z) {
            int sigmaX_v = sigmaX(v);
            int sigmaY_v = sigmaY(v);
            w_min = min(w_min, min(sigmaX_v, sigmaY_v));
            w_max = max(w_max, max(sigmaX_v, sigmaY_v));
        }

        // generate rclv
        int cut_off = w_min + alpha*(w_max - w_min);

        vector<int> RCLvertex;

        for (auto v: Z) {
            int sigmaX_v = sigmaX(v);
            int sigmaY_v = sigmaY(v);

            if (max(sigmaX_v, sigmaY_v) >= cut_off) {
                RCLvertex.push_back(v);
            }
        }

        // select random vertex from RCLV
        int idx = rand() % RCLvertex.size();
        int v = RCLvertex[idx];

        // remove from Z and insert into x and y
        
        if(sigmaX(v) > sigmaY(v)) Y.insert(v);
        else X.insert(v);

        Z.erase(v);
    }
    
}

int main() {

    ifstream inputFile("input.txt");


    inputFile >> numVertices >> numEdges;

    int v1, v2, Wt;

    for (int i = 0; i < numEdges; ++i) {
        inputFile >> v1 >> v2 >> Wt;
        adjList[v1].push_back(make_pair(v2, Wt));
    }

    for(int i = 1; i < numVertices; i++) {
        Z.insert(i);
    }
    inputFile.close();
    return 0;
}