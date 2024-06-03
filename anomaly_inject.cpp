#include "edge.hpp"
#include <vector>
#include <algorithm>

void inject(outEdge* A, int u, int v, int num)
{
    bool inserted = false;
    // Iterates over the outgoing edges of node u
    for(int nb = 0; nb < A[u].out.size(); nb++)
    {
        // Check if an edge to node v already exists
        if(A[u].out[nb] != v)
            continue; // If not, continue to the next edge
        
        // If an edge v exists, update its weight
        A[u].weight[nb] += num;
        A[u].total_w += num;    // Update the total wieght of outgoing edges from u
        inserted = true;        // Mark that the anomaly has been inserted
        break;
    }

    if(inserted == false)
    {
        A[u].out.push_back(v); // Add the node to the matrix
        A[u].weight.push_back(num); // Add the edge weight to the list
        A[u].total_w += num; // Add the edge weight to the total weight
    }
}

/*
    outEdge contains a list of outgoing edges. The inject_anomaly function modifies the graph
    depending on the specific scenario it is given. Essenitally, the attack is the addition of
    edgeNum to the weight of the connecting edge(s) from node u to v.
*/

double inject_anomaly(int scenario, outEdge* A, int n, int edgeNum)
{
    switch(scenario){
        case 1: // add whole weights to one edge
        {
            int u;
            do{
                u = std::rand()%n;  // This will randomly select a node in matrix A to inject the anomaly
            }while(A[u].out.size() == 0);

            int v_n = std::rand()%(A[u].out.size()); // Select another random outgoing edge that is connected to u
            int v = A[u].out[v_n];
            inject(A, u, v, edgeNum);
            return edgeNum;
        }
        case 2: // distribute whole weights to out-edges
        {
            int u;
            do{
                u = std::rand()%n; // Randomly select node
            }while(A[u].out.size()==0);

            // If the edgeNum is less than an outEdge, we will inject the anomaly to that outEdge
            if(edgeNum < A[u].out.size())
            {
                for(int i = 0; i < edgeNum; i++)
                    inject(A, u, A[u].out[i], 1);
            }
            // If edgeNum is not less than any outEdge a fraction of the edgeNum will be added to every outEdge evenly
            else
            {
                int edgeOne = edgeNum/A[u].out.size();
                for(int i = 0; i < A[u].out.size(); i++)
                    inject(A, u, A[u].out[i], edgeOne);
                int edgeRest = edgeNum - edgeOne*A[u].out.size(); // Normalizes the injection
                inject(A, u, A[u].out[0], edgeRest);
            }
            return edgeNum;
        }
        case 3: // distribute whole edges to unseen edges from one source
        {
            int u = std::rand()%n;
            int* v = new int[edgeNum];
            int added = 0;
            int tried = 0;
            while(added < edgeNum && tried < n)
            {
                int v_n = std::rand()%n;
                if(find(A[u].out.begin(), A[u].out.end(), v_n) == A[u].out.end())
                {
                    v[added] = v_n;
                    added++;
                }
                tried++;
            }

            for(int i = 0; i < added; i++)
                inject(A, u, v[i], 1);

            delete [] v;
            return added;
        }
        case 4:
        {
            int* v = new int[edgeNum];
            int v_n = std::rand()%n;
            v[0] = v_n;
            int added = 1;
            int tried = 0;
            while(added < edgeNum && tried < n)
            {
                bool unseen = true;
                v_n = std::rand()%n;
                for(int i = 0; i < added; i++)
                {
                    if(v_n == v[i])
                    {
                        unseen = false;
                        break;
                    }
                    if(find(A[v[i]].out.begin(), A[v[i]].out.end(), v_n) != A[v[i]].out.end())
                    {
                        unseen = false;
                        break;
                    }
                }

                if(unseen)
                {
                    v[added] = v_n;
                    added++;
                }
                tried++;
            }

            for(int i = 0; i < added; i++)
            {
                for(int j = i+1; j < added; j++)
                    inject(A, v[i], v[j], 1);
            }

            delete [] v;
            return added*(added-1);
        }
    }
    return 0;
}

void inject_snapshot(int injectNum, int initSS, int testNum, std::vector<int>& snapshots, std::vector<int>& injectSS)
{
    int injected = 300;
    for(int i = 0; i < injectNum; i++)
    {
        do{
            injected = initSS + rand()%testNum;
        }while(find(injectSS.begin(), injectSS.end(), injected) != injectSS.end());

        injectSS.push_back(injected);
        if(find(snapshots.begin(), snapshots.end(), injected) == snapshots.end())
            snapshots.push_back(injected);
    }
    sort(snapshots.begin(), snapshots.end());
    sort(injectSS.begin(), injectSS.end());
}
