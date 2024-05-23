#ifndef EDGE
#define EDGE
#include <vector>

/*
    Define a guard against multiple inclusions of the header file
    This ensures that the content of the header file is included only once during compilation
    If EDGE is not defined, proceed witht he content of the header file. Otherwise, skip it.
*/

// Edge stream is espressed using timeEdge
struct timeEdge
{
    // Constructor to initialize timeEdge object from a vector of integer tokens
    timeEdge(std::vector<int>& tokens)
    {
        t = tokens[0]; // Assign the timestamp from first token
        src = tokens[1]; // Assign the source node ID from the second token
        trg = tokens[2]; // Assign the target node ID from the third token

        // Checks if there is an additional token indicating whether it's an attack (1) or not (0)
        if(tokens.size() == 3)
            atk = false;    // If no additional token, set attack status to false
        else
            atk = tokens[3] == 1; // If the additional token is 1, set the atk flag to true; otherwise, false
    }

    int src;
    int trg;
    int t;
    bool atk;
};

// Sparse matrix is expressed using outEdge
// A sparese matrix is a matrix where most of the elements are 0
struct outEdge
{
    // Constructor initializes outEdge object
    outEdge()
    {
        total_w = 0; // Initialize the total weight of outgoing edges to 0
    }

    //Member variables to store the total weight of the outgoing edges,
    // the list of target nodes (out), and the weights of corresponding edges
    double total_w;
    std::vector<int> out;
    std::vector<int> weight;
};

#endif
