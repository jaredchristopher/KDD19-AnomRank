# Fast and Accurate Anomaly Detection in Dynamic Graphs with a Two-Pronged Approach

AnomRank is an online algorithm for anomaly detection in dynamic graphs. It uses a two-pronged approach, defining two novel metrics for anomalousness. Each metric tracks the derivatives of its own version of a 'node score' (or node importance) function, allowing the detection of sudden changes in the importance of any node.

Details of AnomRank can be found in our original paper: [AnomRank Paper](https://minjiyoon.xyz/Paper/ANRank.pdf).

# Debugging Purposes

The debugging code included in this project is not the final version and may be less accurate than the production code. It was instrumental in monitoring the program's operations during each snapshot, injection, and detection phase. The debug statements log key information, helping us understand the internal state and track the execution flow of the program.

## Debugging Descriptions

### `main.cpp`
- **Purpose:** Orchestrate the overall execution of the anomaly detection process.
- **Debugging:** Logs the start and end of the program, along with key milestones such as the completion of data loading, anomaly injection, and detection phases. This helps in verifying the flow of execution and identifying any issues in the main control logic.

### `read_data.cpp`
- **Purpose:** Read, process, and prepare temporal graph data.
- **Debugging:** Ensures the file is opened correctly, and logs details of each line processed to help verify data parsing and processing steps.

### `pagerank.cpp`
- **Purpose:** Calculate PageRank values for nodes in the graph.
- **Debugging:** Logs the start and end of the PageRank computation, including the number of iterations and convergence status. This helps verify the correctness and performance of the PageRank implementation.

### `anomaly_inject.cpp`
- **Purpose:** Inject anomalies into the graph data.
- **Debugging:** Logs details of each injection operation, including the nodes and edges involved and the weights added. This helps trace the injection process and verify correct implementation.

### `anomaly_detect.cpp`
- **Purpose:** Normalize data and compute anomaly scores.
- **Debugging:**
  - **normalize:** Logs the start and completion of the normalization process, detailing the number of nodes and time steps.
  - **normalize_online:** Logs the start and completion of the online normalization process, including the current time step and number of nodes.
  - **compute_anomaly_score:** Logs the start of anomaly score computation, details of the derivatives calculated, and the final computed anomaly score. This helps trace the anomaly detection process and verify the computations.

### `accuracy.cpp`
- **Purpose:** Evaluate the accuracy of the anomaly detection algorithm.
- **Debugging:** Logs the results of the accuracy evaluation, including precision and recall values. This helps in assessing the performance of the algorithm and identifying areas for improvement.

# Code Review for all `.cpp` and `.hpp` Files

## 1. `read_data.cpp`

### Purpose
The code is designed to read, process, and prepare temporal graph data for further analysis.

### File Reading
- Opens a file specified by the `path` parameter.
- Reads the file line by line, tokenizes each line based on a specified delimiter, and converts the tokens to integers.
- Constructs `timeEdge` objects using the obtained tokens and populates vectors for edges, times, and nodes.

### Data Processing
- Sorts the vectors of edges, times, and nodes.
- Adjusts the time and node values of each edge based on the initial time and node.

### Calculations
- Determines the final values for `n` (number of nodes), `m` (number of edges), and `timeNum` (number of distinct time points).

### Snapshot Generation
- Determines snapshots based on the edges and a specified `stepSize`.
- Updates a vector (`snapshots`) with the determined snapshot values.

By adding these debugging statements and descriptions, we aim to ensure the correctness of the code during development and facilitate troubleshooting and verification of the program's behavior at each critical stage.