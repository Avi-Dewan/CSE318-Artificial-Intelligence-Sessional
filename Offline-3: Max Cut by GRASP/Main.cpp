#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

struct Edge {
    int v1, v2, weight;
};

int calculateCutSize(const vector<Edge>& edges, const vector<int>& partition) {
    int cutSize = 0;
    for (const Edge& edge : edges) {
        if (partition[edge.v1] != partition[edge.v2]) {
            cutSize += edge.weight;
        }
    }
    return cutSize;
}

vector<int> randomInitialPartition(const vector<Edge>& edges, int numVertices) {
    vector<int> partition(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        partition[i] = rand() % 2; // Random initial partition
    }
    return partition;
}

vector<int> randomizedGreedyInitialSolution(const vector<Edge>& edges, int numVertices) {
    vector<int> partition(numVertices, -1); // Initialize with no partition
    vector<int> vertices(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertices[i] = i; // Create a list of vertices
    }
    random_shuffle(vertices.begin(), vertices.end()); // Randomly shuffle the order

    for (int vertex : vertices) {
        // Choose the partition with the least total weight of connected edges
        int partition1Weight = 0, partition2Weight = 0;
        for (const Edge& edge : edges) {
            if (edge.v1 == vertex) {
                if (partition[edge.v2] == 0) {
                    partition1Weight += edge.weight;
                } else if (partition[edge.v2] == 1) {
                    partition2Weight += edge.weight;
                }
            } else if (edge.v2 == vertex) {
                if (partition[edge.v1] == 0) {
                    partition1Weight += edge.weight;
                } else if (partition[edge.v1] == 1) {
                    partition2Weight += edge.weight;
                }
            }
        }

        // Assign the vertex to the partition with less total edge weight
        if (partition1Weight <= partition2Weight) {
            partition[vertex] = 0;
        } else {
            partition[vertex] = 1;
        }
    }

    return partition;
}


vector<int> localSearch(const vector<Edge>& edges, vector<int>& initialPartition, int maxIterations) {

    vector<int> currentPartition = initialPartition;
    vector<int> bestPartition = initialPartition;

    int bestCutSize = calculateCutSize(edges, bestPartition);

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        int nodeToSwap = -1;
        int maxCutIncrease = 0;

        for (int node = 0; node < initialPartition.size(); ++node) {
            vector<int> newPartition = currentPartition;
            newPartition[node] = 1 - newPartition[node];
            int newCutSize = calculateCutSize(edges, newPartition);
            int cutIncrease = newCutSize - bestCutSize;

            if (cutIncrease > maxCutIncrease) {
                maxCutIncrease = cutIncrease;
                nodeToSwap = node;
            }
        }

        if (nodeToSwap != -1) {
            currentPartition[nodeToSwap] = 1 - currentPartition[nodeToSwap];
            bestCutSize += maxCutIncrease;
            bestPartition = currentPartition;
        }
    }

    return bestPartition;
}

vector<int> semiGreedylocalSearch(const vector<Edge>& edges, vector<int>& initialPartition, int maxIterations, int topN) {
    vector<int> currentPartition = initialPartition;
    vector<int> bestPartition = initialPartition;
    vector<vector<int>> topPartitions(topN); // Store top N solutions
    vector<int> topCutSizes(topN, 0); // Store cut sizes corresponding to top solutions

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        int nodeToSwap = -1;
        int maxCutIncrease = 0;

        for (int node = 0; node < initialPartition.size(); ++node) {
            vector<int> newPartition = currentPartition;
            newPartition[node] = 1 - newPartition[node];
            int newCutSize = calculateCutSize(edges, newPartition);
            int cutIncrease = newCutSize - topCutSizes[topN - 1]; // Consider worst of the top N

            if (cutIncrease > maxCutIncrease) {
                maxCutIncrease = cutIncrease;
                nodeToSwap = node;
            }
        }

        if (nodeToSwap != -1) {
            currentPartition[nodeToSwap] = 1 - currentPartition[nodeToSwap];
            int newCutSize = calculateCutSize(edges, currentPartition);

            if (newCutSize > topCutSizes[0]) { // Update top solutions
                for (int i = 0; i < topN - 1; ++i) {
                    topCutSizes[i + 1] = topCutSizes[i];
                    topPartitions[i + 1] = topPartitions[i];
                }
                topCutSizes[0] = newCutSize;
                topPartitions[0] = currentPartition;
            } else {
                for (int i = 1; i < topN; ++i) {
                    if (newCutSize > topCutSizes[i]) {
                        for (int j = i; j < topN - 1; ++j) {
                            topCutSizes[j + 1] = topCutSizes[j];
                            topPartitions[j + 1] = topPartitions[j];
                        }
                        topCutSizes[i] = newCutSize;
                        topPartitions[i] = currentPartition;
                        break;
                    }
                }
            }
        }
    }

    // Randomly select one of the top N solutions
    int selectedSolution = rand() % topN;
    bestPartition = topPartitions[selectedSolution];

    return bestPartition;
}


vector<int> localSearch3(const vector<Edge>& edges, int numVertices, int maxIterations) {
    vector<int> bestPartition(numVertices, 0); // Initialize with all vertices in one partition
    int bestCutSize = calculateCutSize(edges, bestPartition);

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        vector<int> currentPartition(numVertices, 0); // Start with all vertices in one partition
        int currentCutSize = 0;

        for (int vertex = 0; vertex < numVertices; ++vertex) {
            currentPartition[vertex] = 1; // Move vertex to the other partition
            int newCutSize = calculateCutSize(edges, currentPartition);

            if (newCutSize > currentCutSize) {
                currentCutSize = newCutSize;
                if (currentCutSize > bestCutSize) {
                    bestCutSize = currentCutSize;
                    bestPartition = currentPartition;
                }
            } else {
                currentPartition[vertex] = 0; // Move vertex back to the original partition
            }
        }
    }

    return bestPartition;
}



vector<int> graspMaxCut(const vector<Edge>& edges, int numVertices, int maxIterations, int localSearchIterations) {
    vector<int> bestPartition;
    int bestCutSize = -1;

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        vector<int> initialPartition = randomInitialPartition(edges, numVertices);
        vector<int> localBestPartition = semiGreedylocalSearch(edges, initialPartition, localSearchIterations, 3);
        int localBestCutSize = calculateCutSize(edges, localBestPartition);

        if (localBestCutSize > bestCutSize) {
            bestCutSize = localBestCutSize;
            bestPartition = localBestPartition;
        }
    }

    return bestPartition;
}

int main() {
    srand(time(0));

    ifstream inputFile("input.txt");
    int numVertices, numEdges;
    inputFile >> numVertices >> numEdges;

    vector<Edge> edges(numEdges);

    for (int i = 0; i < numEdges; ++i) {
        inputFile >> edges[i].v1 >> edges[i].v2 >> edges[i].weight;
    }
    inputFile.close();

    int maxIterations = 10;
    int localSearchIterations = 100;
    
    vector<int> bestPartition = graspMaxCut(edges, numVertices, maxIterations, localSearchIterations);
    int bestCutSize = calculateCutSize(edges, bestPartition);

    cout << "Best Cut Size: " << bestCutSize << endl;
    cout << "Best Partition: ";
    for (int partition : bestPartition) {
        cout << partition << " ";
    }
    cout << endl;

    return 0;
}
