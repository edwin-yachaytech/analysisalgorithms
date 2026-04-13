/**
 * Analysis of Algorithms -- Lab 7
 * Student : Edwin Quizhpe A.
 * Exercises:
 *   1. Swap without extra storage (XOR trick)
 *   2. Comparison Counting Sort
 *   3. Distribution Counting Sort
 *   4. Hash Table with closed hashing (linear probing)
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>

// ── print helpers ─────────────────────────────────────────────
void printArr(const std::vector<int>& a, const std::string& lbl) {
    std::cout << lbl << ": ";
    for (int x : a) std::cout << std::setw(4) << x;
    std::cout << "\n";
}

// EXERCISE 1 -- Swap without extra storage
// Three classic methods that need zero temporary variables:
//
//   XOR swap  : u ^= v; v ^= u; u ^= v;
//   Add/Sub   : u = u+v; v = u-v; u = u-v;
//   Multiply  : only works for non-zero integers
//
// The XOR method is the most general (works for any integers,
// including negatives and zeros, as long as u != v in memory).
// Using u == v causes XOR to zero out the variable, so we
// guard against that case.

void swapXOR(int& u, int& v) {
    if (&u == &v) return;   // same memory location -> no-op
    u ^= v;
    v ^= u;
    u ^= v;
}

void swapAddSub(int& u, int& v) {
    u = u + v;
    v = u - v;
    u = u - v;
}

void exercise1() {
    std::cout << "========================================\n";
    std::cout << "  EXERCISE 1 -- Swap Without Extra Storage\n";
    std::cout << "========================================\n\n";

    // XOR swap
    int a = 42, b = 17;
    std::cout << "  XOR swap:\n";
    std::cout << "    Before: u = " << a << ", v = " << b << "\n";
    swapXOR(a, b);
    std::cout << "    After:  u = " << a << ", v = " << b << "\n\n";

    // Add/Subtract swap
    int c = 100, d = -55;
    std::cout << "  Add/Subtract swap:\n";
    std::cout << "    Before: u = " << c << ", v = " << d << "\n";
    swapAddSub(c, d);
    std::cout << "    After:  u = " << c << ", v = " << d << "\n\n";

    // Edge case: same value, different variables
    int e = 7, f = 7;
    std::cout << "  Edge case (same value, different variables):\n";
    std::cout << "    Before: u = " << e << ", v = " << f << "\n";
    swapXOR(e, f);
    std::cout << "    After:  u = " << e << ", v = " << f << "\n\n";

    // How XOR works step by step
    std::cout << "  XOR step-by-step trace (u=5, v=9):\n";
    int u = 5, v = 9;
    std::cout << "    Initial:   u=" << u << " (" << "0101"
              << ")  v=" << v << " (1001)\n";
    u ^= v;
    std::cout << "    u ^= v  -> u=" << u << " (1100)  v=" << v << " (1001)\n";
    v ^= u;
    std::cout << "    v ^= u  -> u=" << u << " (1100)  v=" << v << " (0101)\n";
    u ^= v;
    std::cout << "    u ^= v  -> u=" << u << " (1001)  v=" << v << " (0101)\n\n";
}

// EXERCISE 2 -- Comparison Counting Sort
// For each element A[i], count how many other elements are
// strictly smaller. That count is the final position of A[i]
// in the sorted output.
//
// Time complexity:
//   Two nested loops over n elements -> O(n^2)
//   No comparisons saved over naive sorting.
//
// Space complexity: O(n) for the count array and output array.


std::vector<int> comparisonCountingSort(const std::vector<int>& A) {
    int n = A.size();
    std::vector<int> count(n, 0);   // count[i] = rank of A[i]
    std::vector<int> S(n);          // sorted output

    // count how many elements are smaller than A[i]
    for (int i = 0; i < n - 1; ++i)
        for (int j = i + 1; j < n; ++j) {
            if (A[i] < A[j]) ++count[j];
            else              ++count[i];
        }

    // place each element at its counted position
    for (int i = 0; i < n; ++i)
        S[count[i]] = A[i];

    return S;
}

void exercise2() {
    std::cout << "========================================\n";
    std::cout << "  EXERCISE 2 -- Comparison Counting Sort\n";
    std::cout << "========================================\n\n";

    std::vector<int> A = {62, 31, 84, 96, 19, 47, 55, 73};
    printArr(A, "  Input ");
    std::vector<int> sorted = comparisonCountingSort(A);
    printArr(sorted, "  Sorted");
    std::cout << "\n  Complexity: O(n^2) -- two nested loops over n elements.\n\n";
}


// EXERCISE 3 -- Distribution Counting Sort
// Works only for integers in a known range [lo, hi].
//
// Step 1 (count):   D[v-lo]++ for each value v in A.
// Step 2 (prefix):  D[i] += D[i-1]  -- D[i] now holds the
//                   position AFTER the last occurrence of
//                   value i+lo in the output.
// Step 3 (place):   scan A right-to-left, decrement D[v-lo],
//                   place A[j] at position D[v-lo].
//
// Time complexity:  O(n + r)  where r = hi - lo + 1
//   Step 1: O(n)
//   Step 2: O(r)
//   Step 3: O(n)
//   Total:  O(n + r)  -- linear when r = O(n)
//
// Space complexity: O(n + r)


std::vector<int> distributionCountingSort(const std::vector<int>& A,
                                          int lo, int hi) {
    int n = A.size();
    int r = hi - lo + 1;
    std::vector<int> D(r, 0);   // frequency array
    std::vector<int> S(n);      // sorted output

    // step 1: count frequencies
    for (int x : A) ++D[x - lo];

    // step 2: prefix sums -> D[i] = number of elements <= i+lo
    for (int i = 1; i < r; ++i) D[i] += D[i - 1];

    // step 3: place elements (right-to-left for stability)
    for (int j = n - 1; j >= 0; --j) {
        S[--D[A[j] - lo]] = A[j];
    }

    return S;
}

void exercise3() {
    std::cout << "========================================\n";
    std::cout << "  EXERCISE 3 -- Distribution Counting Sort\n";
    std::cout << "========================================\n\n";

    std::vector<int> A = {62, 31, 84, 96, 19, 47, 55, 73};
    int lo = 19, hi = 96;
    printArr(A, "  Input ");
    std::vector<int> sorted = distributionCountingSort(A, lo, hi);
    printArr(sorted, "  Sorted");

    std::cout << "\n";
    std::cout << "  Complexity: O(n + r)  where r = hi - lo + 1 = "
              << (hi - lo + 1) << "\n";
    std::cout << "  For this input: n = " << A.size()
              << ", r = " << (hi - lo + 1) << "\n\n";

    // ── comparison table ──────────────────────────────────────
    std::cout << "  --- Comparison vs Comparison Counting Sort ---\n\n";
    std::cout << "  " << std::left
              << std::setw(28) << "Property"
              << std::setw(28) << "ComparisonCounting"
              << std::setw(28) << "DistributionCounting"
              << "\n";
    std::cout << "  " << std::string(84, '-') << "\n";

    auto row = [](const std::string& p,
                  const std::string& c,
                  const std::string& d){
        std::cout << "  " << std::left
                  << std::setw(28) << p
                  << std::setw(28) << c
                  << std::setw(28) << d << "\n";
    };
    row("Time complexity",    "O(n^2)",        "O(n + r)");
    row("Space complexity",   "O(n)",           "O(n + r)");
    row("Input requirement",  "Any integers",   "Integers in [lo, hi]");
    row("Stable?",            "Yes",            "Yes (right-to-left)");
    row("Uses comparisons?",  "Yes",            "No");
    row("Practical for",      "Small n",        "Large n, small range");
    std::cout << "\n";
}


// EXERCISE 4 -- Hash Table with Closed Hashing (Linear Probing)
// Stores key-value pairs in a fixed array.
// Hash function:  h(key) = key % TABLE_SIZE
// Collision resolution: linear probing
//   h(key, i) = (h(key) + i) % TABLE_SIZE
//
// Operations:
//   insert(key, value) : O(1) average, O(n) worst
//   search(key)        : O(1) average, O(n) worst
//   remove(key)        : O(1) average -- marks slot DELETED
//                        (tombstone) so probing chains stay intact
//
// Load factor alpha = n / TABLE_SIZE.
// Average probes on successful search: ~(1 + 1/(1-alpha)) / 2
// Performance degrades as alpha -> 1; keep alpha < 0.7.


struct HashEntry {
    int  key;
    std::string value;
    enum class State { EMPTY, OCCUPIED, DELETED } state;

    HashEntry() : key(0), value(""), state(State::EMPTY) {}
};

class HashTable {
    static const int TABLE_SIZE = 13;   // prime -> better distribution
    HashEntry table[TABLE_SIZE];
    int count;

    int hash(int key) const {
        return ((key % TABLE_SIZE) + TABLE_SIZE) % TABLE_SIZE;
    }

public:
    HashTable() : count(0) {}

    // ── insert ────────────────────────────────────────────────
    bool insert(int key, const std::string& value) {
        if (count >= TABLE_SIZE) {
            std::cout << "  [HashTable] Table is full.\n";
            return false;
        }
        int idx = hash(key);
        int firstDeleted = -1;

        for (int i = 0; i < TABLE_SIZE; ++i) {
            int probe = (idx + i) % TABLE_SIZE;

            if (table[probe].state == HashEntry::State::EMPTY) {
                int target = (firstDeleted != -1) ? firstDeleted : probe;
                table[target].key = key; table[target].value = value; table[target].state = HashEntry::State::OCCUPIED;
                ++count;
                return true;
            }
            if (table[probe].state == HashEntry::State::DELETED) {
                if (firstDeleted == -1) firstDeleted = probe;
            } else if (table[probe].key == key) {
                table[probe].value = value;  // update existing key
                return true;
            }
        }
        // all slots probed, use first deleted slot found
        if (firstDeleted != -1) {
            table[firstDeleted].key = key; table[firstDeleted].value = value; table[firstDeleted].state = HashEntry::State::OCCUPIED;
            ++count;
            return true;
        }
        return false;
    }

    // ── search ────────────────────────────────────────────────
    std::string* search(int key) {
        int idx = hash(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int probe = (idx + i) % TABLE_SIZE;
            if (table[probe].state == HashEntry::State::EMPTY)
                return nullptr;
            if (table[probe].state == HashEntry::State::OCCUPIED &&
                table[probe].key == key)
                return &table[probe].value;
        }
        return nullptr;
    }

    // ── remove ────────────────────────────────────────────────
    bool remove(int key) {
        int idx = hash(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int probe = (idx + i) % TABLE_SIZE;
            if (table[probe].state == HashEntry::State::EMPTY)
                return false;
            if (table[probe].state == HashEntry::State::OCCUPIED &&
                table[probe].key == key) {
                table[probe].state = HashEntry::State::DELETED;
                --count;
                return true;
            }
        }
        return false;
    }

    // ── print table ───────────────────────────────────────────
    void print() const {
        std::cout << "\n  Slot  State      Key    Value\n";
        std::cout << "  " << std::string(40, '-') << "\n";
        for (int i = 0; i < TABLE_SIZE; ++i) {
            std::cout << "  [" << std::setw(2) << i << "]  ";
            if (table[i].state == HashEntry::State::EMPTY)
                std::cout << std::left << std::setw(10) << "EMPTY"
                          << "-\n";
            else if (table[i].state == HashEntry::State::DELETED)
                std::cout << std::left << std::setw(10) << "DELETED"
                          << "-\n";
            else
                std::cout << std::left << std::setw(10) << "OCCUPIED"
                          << std::setw(6) << table[i].key
                          << table[i].value << "\n";
        }
        std::cout << "\n  Load factor: " << count
                  << "/" << TABLE_SIZE << " = "
                  << std::fixed << std::setprecision(2)
                  << (double)count / TABLE_SIZE << "\n\n";
    }
};

void exercise4() {
    std::cout << "========================================\n";
    std::cout << "  EXERCISE 4 -- Hash Table (Closed Hashing)\n";
    std::cout << "========================================\n\n";
    std::cout << "  Table size: 13 (prime)  |  Collision: linear probing\n";
    std::cout << "  Hash function: h(key) = key % 13\n\n";

    HashTable ht;

    // insert some entries -- keys 26 and 39 both hash to slot 0 -> collision
    std::cout << "  Inserting entries...\n";
    ht.insert(10,  "Alice");
    ht.insert(23,  "Bob");
    ht.insert(36,  "Carol");    // 36 % 13 = 10 -> collision with 10
    ht.insert(5,   "Dave");
    ht.insert(18,  "Eve");      // 18 % 13 = 5  -> collision with 5
    ht.insert(99,  "Frank");
    ht.insert(0,   "Grace");
    ht.insert(13,  "Heidi");    // 13 % 13 = 0  -> collision with 0

    ht.print();

    // search
    std::cout << "  Search key=36 : ";
    auto* r1 = ht.search(36);
    std::cout << (r1 ? *r1 : "NOT FOUND") << "\n";

    std::cout << "  Search key=18 : ";
    auto* r2 = ht.search(18);
    std::cout << (r2 ? *r2 : "NOT FOUND") << "\n";

    std::cout << "  Search key=77 : ";
    auto* r3 = ht.search(77);
    std::cout << (r3 ? *r3 : "NOT FOUND") << "\n\n";

    // remove and re-search
    std::cout << "  Removing key=5 (Dave)...\n";
    ht.remove(5);
    std::cout << "  Search key=18 after removing key=5: ";
    auto* r4 = ht.search(18);
    std::cout << (r4 ? *r4 : "NOT FOUND")
              << "  (tombstone keeps probe chain intact)\n\n";

    ht.print();
}

// =============================================================
int main() {
    exercise1();
    exercise2();
    exercise3();
    exercise4();
    return 0;
}
