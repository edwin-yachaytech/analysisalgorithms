#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <iomanip>

using namespace std;
using namespace chrono;

// Exhaustive search: try all 2^n subsets
int knapsack_exhaustive(int n, int capacity, vector<int>& weights, vector<int>& values) {
    int best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int total_weight = 0, total_value = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                total_weight += weights[i];
                total_value  += values[i];
            }
        }
        if (total_weight <= capacity && total_value > best)
            best = total_value;
    }
    return best;
}

int main() {
    srand(42); // fixed seed for reproducibility

    cout << fixed << setprecision(4);
    cout << left << setw(6)  << "n"
         << setw(18) << "Time (seconds)"
         << setw(14) << "Best Value"
         << "Capacity" << "\n";
    cout << string(50, '-') << "\n";

    for (int n = 4; n <= 28; n++) {
        int capacity = 50 + rand() % 51; // capacity in [50, 100]
        vector<int> weights(n), values(n);
        for (int i = 0; i < n; i++) {
            weights[i] = 1 + rand() % 20;
            values[i]  = 1 + rand() % 100;
        }

        auto start = high_resolution_clock::now();
        int result = knapsack_exhaustive(n, capacity, weights, values);
        auto end   = high_resolution_clock::now();

        double elapsed = duration<double>(end - start).count();

        cout << left << setw(6)  << n
             << setw(18) << elapsed
             << setw(14) << result
             << capacity << "\n";
        cout.flush();

        if (elapsed > 600.0) { // stop if over 10 minutes
            cout << "\nStopped at n=" << n << " (exceeded 10 min limit)\n";
            break;
        }
    }
    return 0;
}
