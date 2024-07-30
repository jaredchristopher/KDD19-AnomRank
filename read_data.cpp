#include <iostream>
#include <fstream>
#include "edge.hpp"

using namespace std;

struct compare_edge
{
    inline bool operator()(const timeEdge &struct1, const timeEdge &struct2)
    {
        return (struct1.t < struct2.t);
    }
};

void read_data(string path, string delimiter, int stepSize, vector<timeEdge> &edges, vector<int> &snapshots, int &n, int &m, int &timeNum)
{
    ofstream debugFile("debug_data.txt"); // Open a debug file for writing debug information
    if (!debugFile.is_open()) {
        cerr << "Failed to open debug file." << endl;
        return;
    }

    // Writing initial information about the data being read to the debug file
    debugFile << "Reading data from: " << path << endl;
    debugFile << "Using delimiter: " << delimiter << endl;
    debugFile << "Step size: " << stepSize << endl;
    debugFile << "Edges: " << &edges << endl;
    debugFile << "Snapshots: " << &snapshots << endl;
    debugFile << "n: " << n << endl;
    debugFile << "m: " << m << endl;
    debugFile << "timeNum: " << timeNum << endl;

    vector<int> nodes;
    vector<int> times;

    ifstream graphFile(path.c_str()); // Open the graph file for reading
    string line;
    int lineCount = 0;

    while (getline(graphFile, line))
    {
        lineCount++;
        size_t pos = 0;
        vector<int> tokens;

        try
        {
            while ((pos = line.find(delimiter)) != string::npos)
            {
                tokens.push_back(stoi(line.substr(0, pos)));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(stoi(line));

            edges.push_back(timeEdge(tokens));
            times.push_back(tokens[0]);
            nodes.push_back(tokens[1]);
            nodes.push_back(tokens[2]);

            // Writing processed line information to the debug file
            debugFile << "Processed line " << lineCount << ": ";
            for (const int &token : tokens)
                debugFile << token << " ";
            debugFile << endl;
        }
        catch (const std::invalid_argument &e)
        {
            cerr << "Error in line " << lineCount << ": Invalid argument - " << e.what() << endl;
            debugFile << "Error in line " << lineCount << ": Invalid argument - " << e.what() << endl;
            // Handle the error as needed, e.g., skip the current line or terminate the program
        }
    }

    graphFile.close();
    debugFile << "Finished reading file." << endl;

    sort(edges.begin(), edges.end(), compare_edge());
    sort(times.begin(), times.end());
    sort(nodes.begin(), nodes.end());

    int initial_time = times[0];
    int initial_node = nodes[0];
    for (int i = 0; i < edges.size(); i++)
    {
        edges[i].t -= initial_time;
        edges[i].src -= initial_node;
        edges[i].trg -= initial_node;
    }

    n = nodes[nodes.size() - 1] - initial_node + 1;
    m = edges.size();
    timeNum = times[times.size() - 1] - initial_time + 1;

    // Writing the total nodes, edges, and time units information to the debug file
    debugFile << "Total nodes (n): " << n << endl;
    debugFile << "Total edges (m): " << m << endl;
    debugFile << "Total time units (timeNum): " << timeNum << endl;

    int step = 1;
    for (int i = 0; i < edges.size(); i++)
    {
        if (edges[i].t > step * stepSize)
        {
            snapshots.push_back(step);
            step = edges[i].t / stepSize + 1;
        }
    }
    if (step != *(snapshots.end()))
        snapshots.push_back(step);

    // Writing the snapshot information to the debug file
    debugFile << "Snapshots: ";
    for (const int &snapshot : snapshots)
        debugFile << snapshot << " ";
    debugFile << endl;

    debugFile.close();
}
