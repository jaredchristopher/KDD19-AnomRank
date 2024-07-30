#include <iostream>
#include <fstream>
#include <math.h>
#include <random>
#include <vector>
#include "time.h"

#include "accuracy.cpp"
#include "anomaly_detect.cpp"
#include "anomaly_inject.cpp"
#include "pagerank.cpp"
#include "read_data.cpp"

#define attackLimit 50

using namespace std;

int main(int argc, char *argv[])
{
    clock_t start_total = clock(); // Start the total timer

    clock_t start = clock();

    string path = argv[1];
    string delimeter = argv[2];
    int timeStep = atoi(argv[3]);
    int initSS = atoi(argv[4]);
    int injectScene = atoi(argv[5]);
    int injectNum = atoi(argv[6]);
    int injectSize = atoi(argv[7]);
    bool INJECT = (injectScene != 0);

    ofstream debugFile("debug_main.txt");
    if (!debugFile.is_open()) {
        cerr << "Failed to open debug file." << endl;
        return 1;
    }

    // READ DATA
    vector<timeEdge> edges;
    vector<int> snapshots;
    int n, m, timeNum;
    read_data(path, delimeter, timeStep, edges, snapshots, n, m, timeNum);
    int numSS = timeNum / timeStep + 1;
    if (n == 0) {
        cout << "n = 0. Cannot create array of out edges A." << endl;
        return 1;
    }
    outEdge *A = new outEdge[n];
    cout << "============GRAPH Info==============" << endl;
    debugFile << "============GRAPH Info==============" << endl;

    // Output the number of nodes, edges, and timestamps for verification
    cout << "#node: " << n << ", #edges: " << edges.size() << ", #timeStamp: " << timeNum << endl << endl;
    debugFile << "#node: " << n << ", #edges: " << edges.size() << ", #timeStamp: " << timeNum << endl << endl;

    // Custom greeting message for initial confirmation
    cout << "=========HELLO ANTHONY AND JARED!=======" << endl;
    debugFile << "=========HELLO ANTHONY AND JARED!=======" << endl;

    ///////////////////////////////////////////////////////////////
    // Print the first few edges for debugging to ensure data is read correctly
    debugFile << "First few edges:" << endl;
    for (int i = 0; i < min(5, (int)edges.size()); i++) {
        debugFile << "Edge " << i << ": source = " << edges[i].src << ", target = " << edges[i].trg << ", time = " << edges[i].t << ", attack = " << edges[i].atk << endl;
    }
    ///////////////////////////////////////////////////////////////

    // ANOMALY_SCORE
    // int testNum = 900;
    int testNum = numSS - initSS;
    cout << "testNum in Main.cpp: " << testNum << endl;
    bool *attack = new bool[testNum];
    double *anomScore = new double[testNum];
    for (int i = 0; i < testNum; i++)
    {
        anomScore[i] = 0;
        attack[i] = false;
    }

    // PAGERANK
    double **pagerank1 = new double *[3];
    double **pagerank2 = new double *[3];

    if (!pagerank1 || !pagerank2)
    {
        cerr << "Memory allocation for pagerank1 or pagerank2 failed." << endl;
        return 1;
    }

    for (int i = 0; i < 3; i++)
    {
        pagerank1[i] = new double[n];
        pagerank2[i] = new double[n];

        if (!pagerank1 || !pagerank2)
        {
            cerr << "Memory allocation for pagerank1[" << i << "] or pagerank2[" << i << "] failed." << endl;
            return 1;
        }

        for (int j = 0; j < n; j++)
            pagerank1[i][j] = pagerank2[i][j] = 0;
    }

    // MEAN AND VARIANCE
    double **mean = new double *[4];
    double **var = new double *[4];
    for (int i = 0; i < 4; i++)
    {
        mean[i] = new double[n];
        var[i] = new double[n];
        for (int j = 0; j < n; j++)
            mean[i][j] = var[i][j] = 0;
    }

    // INJECTED SNAPSHOT
    vector<int> injectSS;
    if (INJECT)
        inject_snapshot(injectNum, initSS, testNum, snapshots, injectSS);

    // Log the time taken for preprocessing
    cout << "Preprocess done: " << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
    debugFile << "Preprocess done: " << (double)(clock() - start) / CLOCKS_PER_SEC << endl;

    int eg = 0;
    int snapshot = 0;
    int attackNum = 0;
    int injected = 0;
    int current_m = 0;
    double previous_score = 100.0;

    start = clock();
    int print_e = 10;
    for (int ss = 0; ss < snapshots.size(); ss++)
    {
        while (edges[eg].t < snapshots[ss] * timeStep)
        {
            inject(A, edges[eg].src, edges[eg].trg, 1);
            current_m++;
            if (edges[eg].atk)
                attackNum++;
            eg++;
            ///////////////////////////////////////////////////////////////
            // Log the number of processed edges at exponential intervals
            if (eg == print_e)
            {
                debugFile << "Processed edges: " << eg << "," << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;
                print_e *= 10;
            }
            ///////////////////////////////////////////////////////////////
            if (eg == edges.size())
                break;
        }

        if (INJECT && injectSS[injected] == snapshots[ss])
        {
            current_m += inject_anomaly(injectScene, A, n, injectSize);
            attackNum += attackLimit;
            injected++;
            if (injected == injectSS.size())
                INJECT = false;
        }

        snapshot = snapshots[ss];
        int numSteps = 10;
        pagerank(A, pagerank1[snapshot % 3], n, current_m, 1, numSteps);
        pagerank(A, pagerank2[snapshot % 3], n, current_m, 2, numSteps);

        double score = compute_anomaly_score(snapshot, pagerank1, pagerank2, mean, var, n);
        ///////////////////////////////////////////////////////////////
        // Log the anomaly score for each snapshot
        debugFile << "Snapshot " << snapshot << ": anomaly score = " << score << endl;
        ///////////////////////////////////////////////////////////////

        if (snapshot >= initSS)
        {
            anomScore[snapshot - initSS] = score;
            attack[snapshot - initSS] = attackNum >= attackLimit;
            previous_score = score;
        }
        attackNum = 0;
    }

    string filePath = "Anomrank1.txt";

    ofstream writeFile;
    writeFile.open(filePath.c_str(), ofstream::out);
    for (int i = 0; i < testNum; i++)
        writeFile << anomScore[i] << " " << int(attack[i]) << endl;
    writeFile.close();

    // Compute accuracy at various thresholds and log the results
    for (int i = 1; i <= std::min(17, (testNum + 49) / 50); i++) // Adjusted to handle smaller testNum
    {
        compute_accuracy(anomScore, attack, testNum, 50 * i);
    }

    delete[] A;
    delete[] anomScore;

    for (int i = 0; i < 3; i++)
    {
        delete[] pagerank1[i];
        delete[] pagerank2[i];
    }
    delete[] pagerank1;
    delete[] pagerank2;

    for (int i = 0; i < 4; i++)
    {
        delete[] mean[i];
        delete[] var[i];
    }
    delete[] mean;
    delete[] var;

    // Log the total execution time
    cout << "Total time without I/O: " << (double)(clock() - start_total) / CLOCKS_PER_SEC << " seconds" << endl;
    debugFile << "Total time without I/O: " << (double)(clock() - start_total) / CLOCKS_PER_SEC << " seconds" << endl;

    debugFile.close();

    return 1;
}
