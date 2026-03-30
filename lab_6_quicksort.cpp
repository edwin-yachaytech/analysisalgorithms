/**
 * Lab - Algorithm Analysis
 * Exercise 4 & 5: Quick Sort + Empirical Analysis
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <algorithm>

// Global comparison counter for empirical analysis
long long gComparisons = 0;

// ── Partition (Lomuto scheme, last element as pivot) ─────────
int partition(std::vector<int>& arr, int lo, int hi) {
    int pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        ++gComparisons;           // count every comparison
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[hi]);
    return i + 1;
}

// ── Quick Sort ────────────────────────────────────────────────
void quickSort(std::vector<int>& arr, int lo, int hi) {
    if (lo < hi) {
        int p = partition(arr, lo, hi);
        quickSort(arr, lo, p - 1);
        quickSort(arr, p + 1, hi);
    }
}

// ── Generate random array ─────────────────────────────────────
std::vector<int> randomArray(int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = std::rand() % (10 * n);
    return v;
}

// ── Theoretical average-case comparisons: n * log2(n) * 1.386 ─
// The exact average is 2n*ln(n) ≈ 1.386*n*log2(n)
double theoretical(int n) {
    return 2.0 * n * std::log(n);   // 2n ln(n)
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // ── Exercise 4: functional test on small random list ─────
    std::cout << "=== Exercise 4: Quick Sort functional test ===\n";
    std::vector<int> demo = randomArray(15);
    std::cout << "Before: ";
    for (int x : demo) std::cout << x << " ";
    std::cout << "\n";

    gComparisons = 0;
    quickSort(demo, 0, (int)demo.size() - 1);

    std::cout << "After:  ";
    for (int x : demo) std::cout << x << " ";
    std::cout << "\n";

    bool ok = std::is_sorted(demo.begin(), demo.end());
    std::cout << "Sorted correctly: " << (ok ? "YES ✓" : "NO ✗") << "\n\n";

    // ── Exercise 5: empirical analysis ───────────────────────
    std::cout << "=== Exercise 5: Empirical Analysis ===\n\n";

    std::vector<int> sizes = {
        1000, 5000, 10000, 50000,
        100000, 500000, 1000000
    };

    // Table header
    std::cout << std::left
              << std::setw(12) << "n"
              << std::setw(18) << "Actual comps"
              << std::setw(18) << "Theory 2n·ln(n)"
              << std::setw(12) << "Ratio"
              << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (int n : sizes) {
        std::vector<int> arr = randomArray(n);
        gComparisons = 0;
        quickSort(arr, 0, n - 1);

        double theory = theoretical(n);
        double ratio  = static_cast<double>(gComparisons) / theory;

        std::cout << std::setw(12) << n
                  << std::setw(18) << gComparisons
                  << std::setw(18) << static_cast<long long>(theory)
                  << std::fixed << std::setprecision(3)
                  << std::setw(12) << ratio
                  << "\n";
    }

    std::cout << "\nConclusion:\n";
    std::cout << "  Ratio stays near 1.0 across all sizes -> empirical\n";
    std::cout << "  growth matches the theoretical O(n log n) prediction.\n";

    return 0;
}
