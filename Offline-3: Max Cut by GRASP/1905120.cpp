#include <iostream>
#include <fstream>
#include<vector>
#include<set>
#include<math.h>

using namespace std;

#define MAX_WEIGHT 100

int numVertices, numEdges;
vector<pair<int, int>> adjList[(int)1e5 + 3];
set<int> X, Y, Z;


int calculateCutSize(set<int> X, set<int> Y) {

    int cutSize = 0;

    for(auto x: X) {
        for(auto u: adjList[x]) {
            if(Y.count(u.first)) cutSize += u.second;
        }
    }

    for(auto y: Y) {
        for(auto u: adjList[y]) {
            if(X.count(u.first)) cutSize += u.second;
        }
    }

    return cutSize;
}


int sigmaX(int v) {

    int sumWt = 0;

    for (auto x: X) {
        for(auto u:adjList[x]) {
            if(u.first == v) sumWt += u.second;
        }
    }

    for(auto u: adjList[v]) {
        if(X.count(u.first)) sumWt += u.second;
    }
    return sumWt;
}

int sigmaY(int v) {

    int sumWt = 0;

    for (auto y: Y) {
        for(auto u:adjList[y]) {
            if(u.first == v) sumWt += u.second;
        }
    }

    for(auto u: adjList[v]) {
        if(Y.count(u.first)) sumWt += u.second;
    }

    return sumWt;
}



void semiGreedy(double alpha) {

    if (alpha < 0) alpha = (rand() % 100)/100.0;

    int w_min = (int)INFINITY, w_max = -(int)INFINITY;

    for(int u = 1; u <= numVertices; u++) {
        for(auto v: adjList[u]) {
            w_min = min(v.second, w_min);
            w_max = max(v.second, w_max);
        }
    }

    int cut_off = w_min + alpha*(w_max - w_min);

    vector<pair<int, int>> RCLedge; 

    for(int u = 1; u <= numVertices; u++) {
        for(auto v: adjList[u]) {
            if(v.second >= cut_off) {
                RCLedge.push_back(make_pair(u, v.first));
            }
        }
    }

     // select random edge from rcle
    int idx = rand() % RCLedge.size();
    // remove from Z and insert into x and y
    X.insert(RCLedge[idx].first);
    Y.insert(RCLedge[idx].second);
    Z.erase(RCLedge[idx].first);
    Z.erase(RCLedge[idx].second);
  
    while (Z.size() != 0)
    {
        cout << "semi greedy: " << ((numVertices-Z.size())/(float)numVertices)*100 << "% done" << endl;
        
        int w_min = (int)INFINITY, w_max = -(int)INFINITY;


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

int localSearch() {
    bool change = true;

    int iteration = 0;

    while (change) 
    {
        iteration++;
        change = false;
        
        for(int v = 1; v <= numVertices && !change; v++) {

            int sigmaX_v = sigmaX(v);
            int sigmaY_v = sigmaY(v);
            
            int del_v = sigmaX_v - sigmaY_v;

            if(X.count(v) && (del_v > 0)) {
                X.erase(v);
                Y.insert(v);
                change = true;
            } else if (Y.count(v) && del_v < 0) {
                Y.erase(v);
                X.insert(v);
                change = true;
            }
        }
    }

    return iteration;    
}

int GRASP(int maxIteration, float alpha) {

    int iteration = 0;

    set<int> X_prev, Y_prev;

    for (int itr = 1; itr <= maxIteration; itr++) {

        X.clear();
        Y.clear();
        for (int i = 1; i <= numVertices; i++) Z.insert(i);
        
        semiGreedy(alpha);
        iteration += localSearch();

        if (itr == 1) { 
            X_prev = X; 
            Y_prev = Y; 
        } else {
            if (calculateCutSize(X, Y) > calculateCutSize(X_prev, Y_prev)) {
                X_prev = X; 
                Y_prev = Y; 
            }
        }
    }

    X = X_prev; 
    Y = Y_prev;

    return iteration;
}

int main() {

    freopen ("./set1/g49.rud", "r", stdin);
    ofstream ReportFile("report1.csv");


    cin>> numVertices >> numEdges;

    int v1, v2, Wt;

    for (int i = 0; i < numEdges; ++i) {
        cin >> v1 >> v2 >> Wt;
        adjList[v1].push_back(make_pair(v2, Wt));
    }

    for(int i = 1; i <= numVertices; i++) {
        Z.insert(i);
    }

    ReportFile << "SemiGreedy,LocalSearch\n";

    semiGreedy(0.2);

    // TODO: generate report

    ReportFile << calculateCutSize(X, Y) << "\n";


    ReportFile.close();
    
    return 0;
}
