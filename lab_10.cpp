/**
 * Analysis of Algorithms -- Lab 10
 * Student : Edwin Fabricio Quizhpe Aguilar
 * Exercises:
 *   1. Fibonacci with only recursion (no DP)
 *   2. Fibonacci with memoization (debugged from Python snippet)
 *   3. 0-1 Knapsack Problem using dynamic programming

 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

// EXERCISE 1 -- Fibonacci with pure recursion
//
// Recurrence:  F(n) = F(n-1) + F(n-2),  F(1)=F(2)=1
//
// Time complexity: O(2^n)
//   Every call branches into two sub-calls.  The recursion tree
//   has ~2^n nodes, and many sub-problems are solved repeatedly.
//
// Space complexity: O(n)  (call stack depth)


long long fibRecursive(int n) {
    if (n <= 0) return 0;
    if (n == 1 || n == 2) return 1;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

void exercise1() {
    std::cout << "  EXERCISE 1 -- Fibonacci (Recursion)\n";

    std::cout << "  n     F(n)        Time (us)\n";
    std::cout << "  " << std::string(36, '-') << "\n";

    for (int n : {5, 10, 20, 30, 35, 40}) {
        auto t0 = std::chrono::high_resolution_clock::now();
        long long val = fibRecursive(n);
        auto t1 = std::chrono::high_resolution_clock::now();
        double us = std::chrono::duration<double, std::micro>(t1 - t0).count();

        std::cout << "  " << std::setw(3) << n
                  << "   " << std::setw(12) << val
                  << "   " << std::fixed << std::setprecision(1)
                  << std::setw(10) << us << "\n";
    }

    std::cout << "\n  Complexity: O(2^n) -- exponential growth.\n";
    std::cout << "  Each call recomputes the same sub-problems\n";
    std::cout << "  over and over (no caching).\n\n";
}

// EXERCISE 2 -- Fibonacci with memoization (debugged)
// The original Python snippet had these bugs when translated
// to C++:
//
//   BUG 1 -- sentinel value
//   BUG 2 -- array size
//   BUG 3 -- base-case index
//
// Time complexity after fix: O(n)  -- each value computed once.
// Space complexity:          O(n)  -- memo table + call stack.

long long fibMemo(int n, std::vector<long long>& memo) {
    // BUG 1 fixed: check for -1, not 0
    if (memo[n] != -1)
        return memo[n];

    long long result;
    // BUG 3 fixed: base cases for 1-indexed Fibonacci
    if (n == 1 || n == 2)
        result = 1;
    else
        // BUG 2 fixed: memo is passed by reference, not recreated
        result = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);

    memo[n] = result;
    return result;
}

void exercise2() {

    std::cout << "  EXERCISE 2 -- Fibonacci (Memoization, Debugged)\n";

    std::cout << "  Bugs fixed:\n";
    std::cout << "    1. Sentinel: memo initialized to -1, not 0\n";
    std::cout << "    2. Scope:    memo allocated once, passed by ref\n";
    std::cout << "    3. Index:    base cases n==1 and n==2 (1-indexed)\n\n";

    std::cout << "  n     F(n)        Time (us)\n";
    std::cout << "  " << std::string(36, '-') << "\n";

    for (int n : {5, 10, 20, 30, 35, 40, 50}) {
        // BUG 2 fixed: allocate memo once per call, size n+1
        std::vector<long long> memo(n + 1, -1);

        auto t0 = std::chrono::high_resolution_clock::now();
        long long val = fibMemo(n, memo);
        auto t1 = std::chrono::high_resolution_clock::now();
        double us = std::chrono::duration<double, std::micro>(t1 - t0).count();

        std::cout << "  " << std::setw(3) << n
                  << "   " << std::setw(12) << val
                  << "   " << std::fixed << std::setprecision(1)
                  << std::setw(10) << us << "\n";
    }

    std::cout << "\n  Complexity: O(n) -- each F(k) computed exactly once.\n\n";
}

// EXERCISE 3 -- 0-1 Knapsack Problem (Dynamic Programming)
// Given n items each with a weight w[i] and value v[i], and a
// knapsack of capacity W, find the maximum total value that fits
// without exceeding W.  Each item is either taken (1) or not (0).
//
// Time complexity:  O(n * W)
// Space complexity: O(n * W)  (table) -- reducible to O(W)

struct Item {
    std::string name;
    int weight;
    int value;
};

void knapsack(const std::vector<Item>& items, int W) {
    int n = items.size();

    // build DP table: dp[i][c]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(W + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int c = 0; c <= W; ++c) {
            // option 1: skip item i
            dp[i][c] = dp[i - 1][c];
            // option 2: take item i (if it fits)
            if (items[i - 1].weight <= c) {
                int withItem = items[i - 1].value
                             + dp[i - 1][c - items[i - 1].weight];
                if (withItem > dp[i][c])
                    dp[i][c] = withItem;
            }
        }
    }

    // ── print DP table ────────────────────────────────────────
    std::cout << "  DP table (rows = items, cols = capacity 0.." << W << "):\n\n";
    std::cout << "        ";
    for (int c = 0; c <= W; ++c)
        std::cout << std::setw(4) << c;
    std::cout << "\n  " << std::string(8 + 4 * (W + 1), '-') << "\n";

    for (int i = 0; i <= n; ++i) {
        if (i == 0)
            std::cout << "  none | ";
        else
            std::cout << "  " << std::setw(4) << items[i-1].name << "  | ";
        for (int c = 0; c <= W; ++c)
            std::cout << std::setw(4) << dp[i][c];
        std::cout << "\n";
    }

    // ── backtrack to find selected items ─────────────────────
    std::cout << "\n  Maximum value: " << dp[n][W] << "\n";
    std::cout << "  Items selected:\n";

    int c = W;
    int totalW = 0, totalV = 0;
    for (int i = n; i >= 1; --i) {
        if (dp[i][c] != dp[i - 1][c]) {
            std::cout << "    + " << items[i-1].name
                      << "  (weight=" << items[i-1].weight
                      << ", value="   << items[i-1].value << ")\n";
            totalW += items[i-1].weight;
            totalV += items[i-1].value;
            c -= items[i-1].weight;
        }
    }
    std::cout << "  Total weight: " << totalW
              << " / " << W << "\n";
    std::cout << "  Total value:  " << totalV << "\n\n";
}

void exercise3() {
    std::cout << "  EXERCISE 3 -- 0-1 Knapsack (DP)\n";

    // Classic textbook example -- easy to verify by hand
    std::vector<Item> items = {
        {"Book",   2,  6},
        {"Laptop", 3, 10},
        {"Camera", 4, 16},
        {"Phone",  5, 20},
        {"Tablet", 9, 25},
    };
    int W = 10;

    std::cout << "  Knapsack capacity: " << W << "\n\n";
    std::cout << "  Items available:\n";
    std::cout << "  " << std::left
              << std::setw(10) << "Name"
              << std::setw(10) << "Weight"
              << std::setw(10) << "Value" << "\n";
    std::cout << "  " << std::string(30, '-') << "\n";
    for (auto& it : items)
        std::cout << "  " << std::setw(10) << it.name
                  << std::setw(10) << it.weight
                  << std::setw(10) << it.value << "\n";
    std::cout << "\n";

    knapsack(items, W);

    std::cout << "  Complexity: O(n * W) = O("
              << items.size() << " * " << W << ") = O("
              << items.size() * W << ") operations\n\n";
}

// ─────────────────────────────────────────────────────────────
int main() {
    exercise1();
    exercise2();
    exercise3();
    return 0;
}
