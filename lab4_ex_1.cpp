/**
 * Lab 4 - Algorithm Analysis
 
 * Exercise 2: Binary Search vs Interpolation Search
 *             on 10,000,000 random integers
 */

#include <iostream>
#include <vector>
#include <algorithm>    // std::sort, std::generate
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>

using Clock     = std::chrono::high_resolution_clock;
using Nanosecs  = std::chrono::nanoseconds;

// ── Binary Search ─────────────────────────────────────────────────────────────
// Returns index of key in arr, or -1 if not found.
// Time complexity: O(log n)
long long binarySearch(const std::vector<int>& arr, int key) {
    long long lo = 0, hi = static_cast<long long>(arr.size()) - 1;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        if (arr[mid] == key)  return mid;
        if (arr[mid] <  key)  lo = mid + 1;
        else                  hi = mid - 1;
    }
    return -1;
}

// ── Interpolation Search ──────────────────────────────────────────────────────
// Returns index of key in arr, or -1 if not found.
// Uses probing position: pos = lo + (key - arr[lo]) * (hi - lo) / (arr[hi] - arr[lo])
// Time complexity: O(log log n) average for uniform data, O(n) worst case.
long long interpolationSearch(const std::vector<int>& arr, int key) {
    long long lo = 0, hi = static_cast<long long>(arr.size()) - 1;

    while (lo <= hi && key >= arr[lo] && key <= arr[hi]) {
        if (lo == hi) {
            if (arr[lo] == key) return lo;
            return -1;
        }

        // Estimate position via linear interpolation
        long long pos = lo + (static_cast<long long>(key - arr[lo]) *
                              (hi - lo)) /
                             (arr[hi] - arr[lo]);

        if (arr[pos] == key) return pos;
        if (arr[pos] <  key) lo = pos + 1;
        else                 hi = pos - 1;
    }
    return -1;
}

// ── Timing helper ─────────────────────────────────────────────────────────────
template<typename Func>
std::pair<long long, double> timeSearch(Func searchFn,
                                        const std::vector<int>& arr,
                                        int key) {
    auto start  = Clock::now();
    long long idx = searchFn(arr, key);
    auto end    = Clock::now();
    double ns   = static_cast<double>(
                      std::chrono::duration_cast<Nanosecs>(end - start).count());
    return {idx, ns};
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int N = 10'000'000;

    // ── 1. Generate 10 million random integers ────────────────────────────────
    std::cout << "Generating " << N << " random integers...\n";
    std::vector<int> arr(N);
    for (int i = 0; i < N; ++i)
        arr[i] = std::rand();   // range [0, RAND_MAX]

    // ── 2. Sort the array (required by both search algorithms) ────────────────
    std::cout << "Sorting array...\n";
    std::sort(arr.begin(), arr.end());

    // ── 3. Choose a key that EXISTS in the array (worst-case is more fair) ────
    // Pick an element from the middle third for a realistic scenario.
    int keyIdx = N / 3 + std::rand() % (N / 3);
    int key    = arr[keyIdx];

    std::cout << "Searching for key = " << key
              << " (known index: " << keyIdx << ")\n\n";

    // ── 4. Run and time Binary Search ─────────────────────────────────────────
    auto [bsIdx, bsNs] = timeSearch(binarySearch, arr, key);

    // ── 5. Run and time Interpolation Search ──────────────────────────────────
    auto [isIdx, isNs] = timeSearch(interpolationSearch, arr, key);

    // ── 6. Report results ─────────────────────────────────────────────────────
    const int W = 28;
    std::cout << std::string(50, '=') << "\n";
    std::cout << std::left << std::setw(W) << "Algorithm"
              << std::setw(12) << "Found at"
              << "Time (ns)\n";
    std::cout << std::string(50, '-') << "\n";

    std::cout << std::setw(W) << "Binary Search"
              << std::setw(12) << bsIdx
              << std::fixed << std::setprecision(0) << bsNs << " ns\n";

    std::cout << std::setw(W) << "Interpolation Search"
              << std::setw(12) << isIdx
              << std::fixed << std::setprecision(0) << isNs << " ns\n";

    std::cout << std::string(50, '=') << "\n\n";

    // ── 7. Complexity analysis ─────────────────────────────────────────────────
    std::cout << "Theoretical comparison for n = " << N << ":\n";
    std::cout << "  Binary Search       -> O(log n)      ~"
              << static_cast<int>(std::log2(N)) << " comparisons\n";
    std::cout << "  Interpolation Search-> O(log log n)  ~"
              << static_cast<int>(std::log2(std::log2(N))) << " comparisons (uniform data)\n\n";

    double speedup = bsNs / isNs;
    if (speedup > 1.0)
        std::cout << "Interpolation Search was ~" << std::fixed
                  << std::setprecision(1) << speedup << "x faster.\n";
    else
        std::cout << "Binary Search was ~" << std::fixed
                  << std::setprecision(1) << (1.0 / speedup) << "x faster.\n";

    return 0;
}
