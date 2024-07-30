#include "edge.hpp"
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>

void inject(outEdge *A, int u, int v, int num) {
    std::ofstream debugFile("debug_inject.txt", std::ios_base::out); // Open a debug file for writing
    if (!debugFile.is_open()) {
        std::cerr << "Failed to open debug file." << std::endl;
        return;
    }

    debugFile << "Injecting edge from " << u << " to " << v << " with weight " << num << std::endl;

    bool inserted = false;
    for (int nb = 0; nb < A[u].out.size(); nb++) {
        if (A[u].out[nb] != v)
            continue;
        A[u].weight[nb] += num;
        A[u].total_w += num;
        inserted = true;
        break;
    }

    if (!inserted) {
        A[u].out.push_back(v);
        A[u].weight.push_back(num);
        A[u].total_w += num;
    }

    debugFile << "Injection complete for " << u << " -> " << v << std::endl; // Log completion of the injection
    debugFile.close();
}

double inject_anomaly(int scenario, outEdge *A, int n, int edgeNum) {
    std::ofstream debugFile("debug_inject_anomaly.txt", std::ios_base::out); // Open a debug file for writing
    if (!debugFile.is_open()) {
        std::cerr << "Failed to open debug file." << std::endl;
        return 0;
    }

    debugFile << "Injecting anomaly with scenario " << scenario << ", n = " << n << ", edgeNum = " << edgeNum << std::endl;

    switch (scenario) {
        case 1: // Add whole weights to one edge
        {
            int u;
            do {
                u = std::rand() % n;
            } while (A[u].out.size() == 0);

            int v_n = std::rand() % (A[u].out.size());
            int v = A[u].out[v_n];
            inject(A, u, v, edgeNum);
            debugFile << "Anomaly injected (scenario 1) for node " << u << " to " << v << std::endl; // Log specific scenario injection
            debugFile.close();
            return edgeNum;
        }
        case 2: // Distribute whole weights to out-edges
        {
            int u;
            do {
                u = std::rand() % n;
            } while (A[u].out.size() == 0);

            if (edgeNum < A[u].out.size()) {
                for (int i = 0; i < edgeNum; i++)
                    inject(A, u, A[u].out[i], 1);
            } else {
                int edgeOne = edgeNum / A[u].out.size();
                for (int i = 0; i < A[u].out.size(); i++)
                    inject(A, u, A[u].out[i], edgeOne);
                int edgeRest = edgeNum - edgeOne * A[u].out.size();
                inject(A, u, A[u].out[0], edgeRest);
            }
            debugFile << "Anomaly injected (scenario 2) for node " << u << std::endl; // Log specific scenario injection
            debugFile.close();
            return edgeNum;
        }
        case 3: // Distribute whole edges to unseen edges from one source
        {
            int u = std::rand() % n;
            int *v = new int[edgeNum];
            int added = 0;
            int tried = 0;
            while (added < edgeNum && tried < n) {
                int v_n = std::rand() % n;
                if (find(A[u].out.begin(), A[u].out.end(), v_n) == A[u].out.end()) {
                    v[added] = v_n;
                    added++;
                }
                tried++;
            }

            for (int i = 0; i < added; i++)
                inject(A, u, v[i], 1);

            debugFile << "Anomaly injected (scenario 3) for node " << u << " with " << added << " new edges" << std::endl; // Log specific scenario injection

            delete[] v;
            debugFile.close();
            return added;
        }
        case 4:
        {
            int *v = new int[edgeNum];
            int v_n = std::rand() % n;
            v[0] = v_n;
            int added = 1;
            int tried = 0;
            while (added < edgeNum && tried < n) {
                bool unseen = true;
                v_n = std::rand() % n;
                for (int i = 0; i < added; i++) {
                    if (v_n == v[i]) {
                        unseen = false;
                        break;
                    }
                    if (find(A[v[i]].out.begin(), A[v[i]].out.end(), v_n) != A[v[i]].out.end()) {
                        unseen = false;
                        break;
                    }
                }

                if (unseen) {
                    v[added] = v_n;
                    added++;
                }
                tried++;
            }

            for (int i = 0; i < added; i++) {
                for (int j = i + 1; j < added; j++)
                    inject(A, v[i], v[j], 1);
            }

            debugFile << "Anomaly injected (scenario 4) with " << added << " new edges among " << edgeNum << " nodes" << std::endl; // Log specific scenario injection

            delete[] v;
            debugFile.close();
            return added * (added - 1);
        }
        default:
            debugFile << "Unknown scenario: " << scenario << std::endl; // Log unknown scenario
            debugFile.close();
            return 0;
    }
}

void inject_snapshot(int injectNum, int initSS, int testNum, std::vector<int> &snapshots, std::vector<int> &injectSS) {
    std::ofstream debugFile("debug_inject_sp.txt", std::ios_base::out); // Open a debug file for writing
    if (!debugFile.is_open()) {
        std::cerr << "Failed to open debug file." << std::endl;
        return;
    }

    debugFile << "Injecting snapshots with injectNum = " << injectNum << ", initSS = " << initSS << ", testNum = " << testNum << std::endl;

    int injected = 300;
    for (int i = 0; i < injectNum; i++) {
        do {
            injected = initSS + rand() % testNum;
        } while (find(injectSS.begin(), injectSS.end(), injected) != injectSS.end());

        injectSS.push_back(injected);
        if (find(snapshots.begin(), snapshots.end(), injected) == snapshots.end())
            snapshots.push_back(injected);
    }
    sort(snapshots.begin(), snapshots.end());
    sort(injectSS.begin(), injectSS.end());

    debugFile << "Injected snapshots: "; // Log the list of injected snapshots
    for (const auto& ss : injectSS) {
        debugFile << ss << " ";
    }
    debugFile << std::endl;

    debugFile.close();
}
