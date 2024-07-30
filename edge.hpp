#ifndef EDGE
#define EDGE
#include <vector>

// Edge stream is expressed using timeEdge
struct timeEdge
{
    timeEdge(std::vector<int> &tokens)
    {
        t = tokens[0];
        src = tokens[1];
        trg = tokens[2];

        if (tokens.size() == 3)
            atk = false;
        else
            atk = tokens[3] == 1;
    }

    int src;
    int trg;
    int t;
    bool atk;
};

// Sparse matrix is expressed using outEdge
// Sparse matrix is expressed using outEdge
struct outEdge
{
    outEdge()
    {
        total_w = 0;
    }

    double total_w;
    int timestamp;
    std::vector<int> out;
    std::vector<int> weight;
};
/* 
struct outEdge
{
    int timestamp;           // Add timestamp for each node
    std::vector<int> out;    // List of outgoing nodes
    std::vector<int> weight; // Corresponding weights
    double total_w;          // Total weight for the node

    // Constructor to initialize total_w
    outEdge() : total_w(0) {}
}; */

#endif