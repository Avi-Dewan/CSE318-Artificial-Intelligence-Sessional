#include<bits/stdc++.h>
#include<fstream>

using namespace std;

#define MAX_NODES (int)1e5
#define MAX_WEIGHT 100

int N, M;
vector<pair<int, int>> adj[MAX_NODES+1];
set<int> X, XX, Y, YY, Z;

int getWeight(int, int);
int getWeight(set<int>, set<int>);
int getSigma(int, set<int>);

void semiGreedyMaxCut(float);
int localSearchMaxCut();
int GRASP(int, float);

int main() {

    ofstream ReportFile("report.csv");
    ReportFile << "Name,|V|,|E|,Constructive Algorithm,LS-iteration,LS-best value,GRASP-iteration,GRASP-best value\n";
    
    for (int filecount = 1; filecount <= 1; filecount++) {
        string filename = "./set1/g" + to_string(filecount) + ".rud";
        
        ifstream input;
        input.open(filename);

        input >> N >> M;
        ReportFile << "G" << filecount << "," << N << "," << M << ",";

        // BUILD GRAPH
        for (int i = 0; i < M; i++) {
            int u, v, w;
            input >> u >> v >> w;
            adj[u].push_back({v, w}); 
            // adj[v].push_back({u, w}); 
        }

        ////// RANDOM
        // LOCAL SEARCH (ALPHA = 0)
        int randomCount = 3; 
        int sum = 0;
        int itrCount = 0;
        ReportFile << "random (avg of 3),";
        for (int i = 0; i < randomCount; i++) {
            itrCount += GRASP(1, 0);
            sum += getWeight(X, Y);
        }
        ReportFile << itrCount/randomCount << "," << sum/randomCount << ",";

        cout << filename << " done local search with alpha = 1\n";

        // GRASP (ALPHA = 0)
        sum = 0;
        itrCount = 0;
        ReportFile << "random (avg of 5),";
        for (int i = 0; i < randomCount; i++) {
            itrCount += GRASP(3, 0);
            sum += getWeight(X, Y);
        }
        ReportFile << itrCount/randomCount << "," << sum/randomCount << ",";

        cout << filename << " done grasp with alpha = 1\n";

        ////// GREEDY
        // LOCAL SEARCH (ALPHA = 1)
        ReportFile << GRASP(1, 1) << ",";
        ReportFile << getWeight(X, Y) << ",";

        cout << filename << " done local search with alpha = 0\n";

        // GRASP (ALPHA = 1)
        ReportFile << GRASP(3, 1) << ",";
        ReportFile << getWeight(X, Y) << ",";

        cout << filename << " done grasp with alpha = 0\n";

        ////// SEMIGREEDY
        // LOCAL SEARCH (ALPHA = 0.8)
        ReportFile << GRASP(1, 0.8) << ",";
        ReportFile << getWeight(X, Y) << ",";

        cout << filename << " done local search with alpha = 0.8\n";

        // GRASP (ALPHA = 1)
        ReportFile << GRASP(3, 0.8) << ",";
        ReportFile << getWeight(X, Y) << endl;

        cout << filename << " done grasp with alpha = 0.8\n";

        fclose(stdin);
    }
    

    ReportFile.close();
    return 0;
}

int getWeight(int u, int v) {
    for (auto x: adj[u]) {
        if (x.first == v) return x.second;
    }

    for (auto x: adj[v]) {
        if (x.first == u) return x.second;
    }

    return -MAX_WEIGHT-3;
}

int getWeight(set<int> A, set<int> B) {
    int weight = 0;

    for (auto a: A) {
        for (auto b: B) {
            int w = getWeight(a, b);
            if (w > -MAX_WEIGHT) weight += w;
        }
    }

    return weight;
}

int getSigma(int x, set<int> Y) {
    int sigma = 0;

    for (auto y: Y) {
        int w = getWeight(x, y);
        if (w > -MAX_WEIGHT)
            sigma += w;
    }

    return sigma;
} 

void semiGreedyMaxCut(float alpha) { 
    // srand(1935);
    if (alpha < 0) alpha = (rand() % 100)/100.0;

    int wMin = (int)INFINITY, wMax = -(int)INFINITY;
    for (int i = 1; i <= N; i++) {
        for (auto x: adj[i]) {
            wMin = min(wMin, x.second);
            wMax = max(wMax, x.second);
        }
    }

    // generate rcle
    int threshold = wMin + alpha*(wMax - wMin);
    vector<pair<int, int>> rclEdgeList;
    for (int i = 1; i <= N; i++) {
        for (int j = i+1; j <= N; j++) {
            if (getWeight(i, j) >= -MAX_WEIGHT && getWeight(i, j) >= threshold) {
                rclEdgeList.push_back({i, j});
            }
        }
    }

    // select random edge from rcle
    int edgeIndex = rand() % rclEdgeList.size();
    X.insert(rclEdgeList[edgeIndex].first);
    Y.insert(rclEdgeList[edgeIndex].second);
    Z.erase(rclEdgeList[edgeIndex].first);
    Z.erase(rclEdgeList[edgeIndex].second);

    while (Z.size()) {
        float x = ((N-Z.size())/(float)N)*100;
        if (x - (int)x == 0) {
            // cout << "semi greedy: " << x << "% done" << endl;
        }
        wMin = (int)INFINITY, wMax = -(int)INFINITY;
        for (auto z: Z) {
            int sigmaX = getSigma(z, Y);
            int sigmaY = getSigma(z, X);
            int mi = min(sigmaX, sigmaY);
            int ma = max(sigmaX, sigmaY);
            wMin = min(wMin, mi);
            wMax = max(wMax, ma);
        }

        // generate rclv
        threshold = wMin + alpha*(wMax - wMin);
        vector<int> rclVertexList;
        for (auto z: Z) {
            int sigmaX = getSigma(z, Y);
            int sigmaY = getSigma(z, X);

            if (max(sigmaX, sigmaY) >= threshold) {
                rclVertexList.push_back(z);
            }
        }

        // select random vertex from rclv
        int vertexIndex = rand() % rclVertexList.size();
        int selectedVertex = rclVertexList[vertexIndex];
        if (getSigma(selectedVertex, X) > getSigma(selectedVertex, Y)) Y.insert(selectedVertex);
        else X.insert(selectedVertex);
        Z.erase(selectedVertex);
    }
}

int localSearchMaxCut() {
    bool change = true;
    int itr = 0;

    while (change) {
        itr++;
        change = false;

        // cout << "local searching...\n";

        for (int i = 1; i <= N && !change; i++) {
            int sigmaX = getSigma(i, Y);
            int sigmaY = getSigma(i, X);
            
            if (X.count(i) && sigmaY > sigmaX) {
                X.erase(i);
                Y.insert(i);
                change = true;
            } else if (Y.count(i) && sigmaX > sigmaY) {
                Y.erase(i);
                X.insert(i);
                change = true;
            }
        }
    }

    return itr;
}

int GRASP(int maxIteration, float alpha) {

    int itrCount = 0;

    for (int i = 0; i < maxIteration; i++) {
        
        for (int j = 1; j <= N; j++) Z.insert(j);
        X.clear();
        Y.clear();

        semiGreedyMaxCut(alpha);
        itrCount += localSearchMaxCut();

        if (i == 0) { XX = X; YY = Y; }
        else {
            if (getWeight(X, Y) > getWeight(XX, YY)) {
                XX = X; YY = Y;
            }
        }
    }

    X = XX; Y = YY;

    return itrCount;
}


int main() {

    ofstream ReportFile("reportc.csv");
    ReportFile << "Name,|V|,|E|,Constructive Algorithm,LS-iteration,LS-best value,GRASP-iteration,GRASP-best value\n";
    
    for (int filecount = 1; filecount <= 1; filecount++) {
        string filename = "./set1/g" + to_string(filecount) + ".rud";
        
        ifstream input;
        input.open(filename);

        input >> numVertices >> numEdges;
        ReportFile << "G" << filecount << "," << numVertices << "," << numEdges << ",";

        // BUILD GRAPH
        for (int i = 0; i < numEdges; i++) {
            int u, v, w;
            input >> u >> v >> w;
            adjList[u].push_back({v, w}); 
            // adj[v].push_back({u, w}); 
        }

        ////// RANDOM
        // LOCAL SEARCH (ALPHA = 0)
        int randomCount = 3; 
        int sum = 0;
        int itrCount = 0;
        ReportFile << "random (avg of 3),";
        for (int i = 0; i < randomCount; i++) {
            itrCount += GRASP(1, 0);
            sum += calculateCutSize(X, Y);
        }
        ReportFile << itrCount/randomCount << "," << sum/randomCount << ",";

        cout << filename << " done local search with alpha = 1\n";

        // GRASP (ALPHA = 0)
        sum = 0;
        itrCount = 0;
        ReportFile << "random (avg of 5),";
        for (int i = 0; i < randomCount; i++) {
            itrCount += GRASP(3, 0);
            sum += calculateCutSize(X, Y);
        }
        ReportFile << itrCount/randomCount << "," << sum/randomCount << ",";

        cout << filename << " done grasp with alpha = 1\n";

        ////// GREEDY
        // LOCAL SEARCH (ALPHA = 1)
        ReportFile << GRASP(1, 1) << ",";
        ReportFile << calculateCutSize(X, Y) << ",";

        cout << filename << " done local search with alpha = 0\n";

        // GRASP (ALPHA = 1)
        ReportFile << GRASP(3, 1) << ",";
        ReportFile << calculateCutSize(X, Y) << ",";

        cout << filename << " done grasp with alpha = 0\n";

        ////// SEMIGREEDY
        // LOCAL SEARCH (ALPHA = 0.8)
        ReportFile << GRASP(1, 0.8) << ",";
        ReportFile << calculateCutSize(X, Y) << ",";

        cout << filename << " done local search with alpha = 0.8\n";

        // GRASP (ALPHA = 1)
        ReportFile << GRASP(3, 0.8) << ",";
        ReportFile << calculateCutSize(X, Y) << endl;

        cout << filename << " done grasp with alpha = 0.8\n";

        fclose(stdin);
    }
    

    ReportFile.close();
    return 0;
}
