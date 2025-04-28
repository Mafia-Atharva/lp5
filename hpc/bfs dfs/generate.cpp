#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_set>

using namespace std;

int main() {
    const int N = 1000;  // Number of nodes
    const string OUTPUT_FILE = "input.txt";

    // Seed the random number generator
    srand(time(0));

    // Initialize adjacency matrix with all zeros
    vector<vector<int>> adj_matrix(N, vector<int>(N, 0));

    // Generate random edges for each node
    for (int i = 0; i < N; ++i) {
        // Determine number of neighbors (5-10 inclusive)
        int k = 5 + (rand() % 6);
        unordered_set<int> neighbors;

        // Collect unique neighbors
        while (neighbors.size() < k) {
            int j = rand() % N;
            neighbors.insert(j);
        }

        // Update adjacency matrix for both directions (undirected)
        for (int j : neighbors) {
            adj_matrix[i][j] = 1;
            adj_matrix[j][i] = 1;
        }
    }

    // Write the adjacency matrix to the output file
    ofstream outFile(OUTPUT_FILE);
    if (!outFile.is_open()) {
        cerr << "Error opening file " << OUTPUT_FILE << endl;
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (j > 0) {
                outFile << " ";
            }
            outFile << adj_matrix[i][j];
        }
        outFile << "\n";
    }

    cout << "Generated " << OUTPUT_FILE << " with " << N << " nodes." << endl;

    return 0;
}