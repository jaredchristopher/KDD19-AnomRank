#include "edge.hpp"
#include <cmath>
#include <vector>
#define EPSILON  0.001 // A small constant used to check for convergence, which refers to the process where the algorithm's successive iterations produce results that become increasingly close to a stable solution
#define c 0.5 // Defines the damping factor used in the PageRank algorithm, represents the probability that a random web surfer will continue clicking on links

double* pagerank(outEdge* A, double* b, int n, int m, int version)
{
    // Initializes the PageRank vector b based ont he verison specified
    if(version == 1)
    {
        for(int i = 0; i < n; i++)
            b[i] = c/n; // Uniform initialization if verison 1
    }
    else
    {
        for(int i = 0; i < n; i++)
            b[i] = c*A[i].total_w/m; // Weighted initialization if verison 2
    }

    double* nq = new double[n]; // Stores new PageRank values
    double* nq_prev = new double[n]; // Stores the previous PageRank values
    for(int i = 0; i < n; i++)
    {
        nq[i] = 0;  // Initializes the PageRank array nq with 0
        nq_prev[i] = b[i];  // Initialzes the nq_prev array with the values of b
    }

    double* new_nq; // Pointer to the current iteration of PageRank values
    double* old_nq; // Pointer to the old iteration of PageRank values
    int out_iter = 0; // Iteration counter
    double delta_score = 100; // Variable to track the chnage in PageRank values

    // Iterativley calculates PageRank until the values are less than EPSILON (0.001)
    while(delta_score > EPSILON)
    {
        /*If the counter is even, swap pointers KNOW AS DOUBLE BUFFERING
        This is used to avoid unnecessary copying of arrays during the iterative process 
        of calculating PageRank Values.
        By alternating between two arrays, the algorithm efficiently updates the PageRank values without needing
        to create new arrays for each iteration.
        */
        if(out_iter % 2 == 0)
        {
            new_nq = nq;
            old_nq = nq_prev;
        }
        else
        {
            new_nq = nq_prev;
            old_nq = nq;
        }

        for(int i = 0; i < n; i++)
            new_nq[i] = 0; // Resets new_nq to 0 for the current iteration

        for(int i = 0; i < n; i++)
        {
	        if(old_nq[i] == 0)
                continue; // Skip nodes with 0 PageRank in the previous iteration
            if(A[i].total_w == 0)
                continue; // Skip nodes with no outgoing edges

            if(version == 1)
            {
                double delta = (1-c)*old_nq[i]/A[i].out.size(); // Calculate contribution for version 1

                std::vector<int>::iterator j = A[i].out.begin();
                for(; j != A[i].out.end(); j++)
                {
                    new_nq[*j] += delta; // Distributing PageRank to outgoing nodes
                }
            }
            else
            {
                double delta = (1-c)*old_nq[i]/A[i].total_w;

                std::vector<int>::iterator j = A[i].out.begin();
                std::vector<int>::iterator k = A[i].weight.begin();
                for(; j != A[i].out.end(); j++, k++)
                {
                    new_nq[*j] += delta*(*k); // Distributing weighted PageRank to outgoing nodes
                }
            }
        }

        delta_score = 0; // Reset the delta score for the current iteration
        for(int i = 0; i < n; i++)
        {
            delta_score += std::abs(new_nq[i]); // Accumulate changes in Page Rank
            b[i] += new_nq[i]; // Update the PageRank values
        }
        out_iter++; // Increase the number of iterations
    }

    double sum = 0;
    for(int i = 0; i < n; i++)
        sum += b[i]; // Calculate the sum of the PageRank values for normalization
    for(int i = 0; i < n; i++)
        b[i] /= sum; // Normalize the PageRank values

    delete [] nq; // Free the dynamically allocate memory for nq
    delete [] nq_prev; // Free the dynamically allocated memory for nq_prev

    return b; // Return the normalized PageRank vector
}
