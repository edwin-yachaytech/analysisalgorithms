/**

 * Lab 4 - Algorithm Analysis

 * Exercise 1: Insertion Sort for Alphabetic Characters

 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

// ── Insertion Sort for char arrays ──────────────────────────────────────────
// No algorithmic adjustment needed: C++ compares chars via ASCII order,
// which already respects alphabetical ordering for [a-z] and [A-Z].
void insertionSort(std::vector<char>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        char key = arr[i];
        int j = i - 1;
        // Shift elements greater than key one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// ── Print helper ─────────────────────────────────────────────────────────────
void printArray(const std::vector<char>& arr, const std::string& label) {
    std::cout << label << ": [ ";
    for (char c : arr) std::cout << c << ' ';
    std::cout << "]\n";
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int SIZE = 20;

    // Generate random lowercase alphabetic characters
    std::vector<char> arr(SIZE);
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = static_cast<char>('a' + std::rand() % 26);
    }

    printArray(arr, "Unsorted array");

    insertionSort(arr);

    printArray(arr, "  Sorted array");

    std::cout << "\nNote: No adjustments needed for char type.\n"
              << "C++ compares chars using ASCII values, which\n"
              << "preserves correct alphabetical ordering.\n";

    return 0;
}
