/**
 * Lab 6 - Algorithm Analysis
 * Exercise 2 & 3: Merge Sort + Stability Test
 */

#include <iostream>
#include <vector>
#include <string>

// ── Basic Merge Sort (integers) ──────────────────────────────
void merge(std::vector<int>& arr, int lo, int mid, int hi) {
    std::vector<int> left(arr.begin() + lo, arr.begin() + mid + 1);
    std::vector<int> right(arr.begin() + mid + 1, arr.begin() + hi + 1);

    int i = 0, j = 0, k = lo;
    while (i < (int)left.size() && j < (int)right.size()) {
        // strict < keeps equal elements from left subarray first → STABLE
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else                     arr[k++] = right[j++];
    }
    while (i < (int)left.size())  arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
}

void mergeSort(std::vector<int>& arr, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(arr, lo, mid);
    mergeSort(arr, mid + 1, hi);
    merge(arr, lo, mid, hi);
}

// ── Stability Test ───────────────────────────────────────────
// Each element carries its key value and original position.
struct Element {
    int value;
    int originalIndex;
    std::string label; // e.g. "A", "B" to tell duplicates apart
};

void mergeStable(std::vector<Element>& arr, int lo, int mid, int hi) {
    std::vector<Element> left(arr.begin() + lo, arr.begin() + mid + 1);
    std::vector<Element> right(arr.begin() + mid + 1, arr.begin() + hi + 1);

    int i = 0, j = 0, k = lo;
    while (i < (int)left.size() && j < (int)right.size()) {
        if (left[i].value <= right[j].value) arr[k++] = left[i++];
        else                                 arr[k++] = right[j++];
    }
    while (i < (int)left.size())  arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
}

void mergeSortStable(std::vector<Element>& arr, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortStable(arr, lo, mid);
    mergeSortStable(arr, mid + 1, hi);
    mergeStable(arr, lo, mid, hi);
}

void printElements(const std::vector<Element>& arr, const std::string& label) {
    std::cout << label << ":\n";
    for (const auto& e : arr)
        std::cout << "  [val=" << e.value
                  << ", id=" << e.label
                  << ", orig_pos=" << e.originalIndex << "]\n";
}

int main() {
    // ── Exercise 2: basic Merge Sort ───────────────────
    std::cout << "=== Exercise 2: Merge Sort ===\n";
    std::vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    std::cout << "Before: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    mergeSort(arr, 0, (int)arr.size() - 1);

    std::cout << "After:  ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n\n";

    // ── Exercise 3: stability test ───────────────────────────
    std::cout << "=== Exercise 3: Stability Test ===\n";
    // Two elements share value=5, two share value=2.
    // After sorting, duplicates must preserve original order.
    std::vector<Element> elems = {
        {5, 0, "5-A"},
        {3, 1, "3-A"},
        {2, 2, "2-A"},
        {5, 3, "5-B"},
        {1, 4, "1-A"},
        {2, 5, "2-B"}
    };

    printElements(elems, "Before sorting");
    mergeSortStable(elems, 0, (int)elems.size() - 1);
    printElements(elems, "\nAfter  sorting");

    // Verify stability automatically
    bool stable = true;
    for (int i = 0; i < (int)elems.size() - 1; ++i) {
        if (elems[i].value == elems[i+1].value &&
            elems[i].originalIndex > elems[i+1].originalIndex) {
            stable = false;
            break;
        }
    }
    std::cout << "\nStability check: "
              << (stable ? "STABLE ✓" : "NOT STABLE ✗") << "\n";
    std::cout << "  -> Equal elements kept their original relative order.\n";

    return 0;
}
