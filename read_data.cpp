#include <iostream>
#include <fstream>
#include "edge.hpp"

using namespace std;

struct compare_edge
{
    // Define a functor to compare two timeEdge structures based on their timestamp
    inline bool operator() (const timeEdge& struct1, const timeEdge& struct2)
    {
        return (struct1.t < struct2.t);
    }
};

void read_data(string path, string delimiter, int stepSize, vector<timeEdge>& edges, vector<int>& snapshots, int& n, int& m, int& timeNum)
{
    vector<int> nodes; // Vector stores unique node IDs
    vector<int> times; // Vector to store all timestamps

    ifstream graphFile(path.c_str());
    string line;
    while(getline(graphFile, line)) // Reads the file line by line
    {
        size_t pos = 0;
        vector<int> tokens; // Vector to store tokens from the current line
        while ((pos = line.find(delimiter)) != string::npos) // finds the delimiter of the line
        {
            tokens.push_back(stoi(line.substr(0, pos))); // Convert substring to integer and store in tokens
            line.erase(0, pos + delimiter.length()); // Erase the processed part of the line
        }
        tokens.push_back(stoi(line));

        edges.push_back(timeEdge(tokens));
        times.push_back(tokens[0]); // Adds the timestamps for each time a token is grabbed
        nodes.push_back(tokens[1]); // Adds the source node to nodes
        nodes.push_back(tokens[2]); // Adds the destination node to nodes
    }

    graphFile.close();

    sort(edges.begin(), edges.end(), compare_edge()); // Sorts the edges based on timestamps using the compare_edge funciton
    sort(times.begin(), times.end()); // Sort times in ascending order
    sort(nodes.begin(), nodes.end()); // Sort nodes in ascending order

    int initial_time = times[0]; // Earliest timestamp
    int initial_node = nodes[0]; // Smallest node
    for(int i = 0; i < edges.size(); i++)
    {
        edges[i].t -= initial_time; // Normalize timestamps by subtracting the inital_time
        edges[i].src -= initial_node; // Normalize node Ids by subtracting initial_node
        edges[i].trg -= initial_node; // Normalize target node Ids by subtracting initial_node
    }

    n = nodes[nodes.size()-1] - initial_node + 1; // n is the number of unique nodes
    m = edges.size(); // m is the number of edges
    timeNum = times[times.size()-1] - initial_time + 1; // timeNum is the total time span

    int step = 1; // Step counter initialized to 1
    for(int i = 0; i < edges.size(); i++)
    {
        if(edges[i].t > step*stepSize) // Check if current edge's time exceeds the current step size
        {
            snapshots.push_back(step); // Record the current step as a snapshot
            step = edges[i].t/stepSize + 1; // Update step to the next interval based on stepSize

        }
    }
    if(step != *(snapshots.end())) // Makes sure the final snapshot is recorded
        snapshots.push_back(step); // If not, added it to the end
}
