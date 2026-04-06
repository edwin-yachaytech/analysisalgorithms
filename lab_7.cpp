/**
 * Analysis of Algorithms - Lab 7
 * Student : Edwin Quizhpe 
 * Exercises:
 *   1. Forward elimination
 *   2. Back substitution 
 *   3. Array -> Max-Heap 
 *   4. LU decomposition  
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <stdexcept>

using Matrix = std::vector<std::vector<double>>;

// ── helpers ──────────────────────────────────────────────────
void printMatrix(const Matrix& M, const std::string& label,
                 int w = 9, int p = 4) {
    std::cout << label << "\n";
    for (auto& row : M) {
        for (double v : row)
            std::cout << std::setw(w) << std::fixed
                      << std::setprecision(p) << v;
        std::cout << "\n";
    }
    std::cout << "\n";
}

void printVec(const std::vector<double>& v, const std::string& label) {
    std::cout << label << ":  ";
    for (double x : v)
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << x;
    std::cout << "\n\n";
}

// EXERCISE 1 -- Forward Elimination:
// Transforms augmented matrix [A|b] into upper-triangular form.
// Uses partial pivoting to improve numerical stability.
//
// Time complexity: O(n^3)
//   For each of the n-1 pivot columns  --> O(n)
//     For each of the remaining rows   --> O(n)
//       For each column entry          --> O(n)
//   Total: O(n * n * n) = O(n^3)
//
// Returns the augmented upper-triangular matrix.

Matrix forwardElimination(Matrix aug) {
    int n = aug.size();

    for (int col = 0; col < n; ++col) {

        // -- partial pivoting: find row with largest |value| in column
        int pivotRow = col;
        for (int r = col + 1; r < n; ++r)
            if (std::abs(aug[r][col]) > std::abs(aug[pivotRow][col]))
                pivotRow = r;

        if (std::abs(aug[pivotRow][col]) < 1e-12)
            throw std::runtime_error("Singular or near-singular matrix.");

        std::swap(aug[col], aug[pivotRow]);   // swap rows

        // -- eliminate entries below pivot
        for (int row = col + 1; row < n; ++row) {
            double factor = aug[row][col] / aug[col][col];
            for (int k = col; k <= n; ++k)
                aug[row][k] -= factor * aug[col][k];
        }
    }
    return aug;
}

// EXERCISE 2 -- Back Substitution:
// Solves upper-triangular system U*x = b' after elimination.
// Time complexity: O(n^2)
//   x[n-1] computed in 1 operation.
//   x[i]   computed in (n-1-i) multiplications + 1 division.
//   Total multiplications: 0+1+2+...+(n-1) = n(n-1)/2 = O(n^2)
// The running time is therefore Theta(n^2).

std::vector<double> backSubstitution(const Matrix& aug) {
    int n = aug.size();
    std::vector<double> x(n, 0.0);

    long long ops = 0;   // operation counter (reported for demonstration)

    for (int i = n - 1; i >= 0; --i) {
        x[i] = aug[i][n];               // start with RHS value
        for (int j = i + 1; j < n; ++j) {
            x[i] -= aug[i][j] * x[j];  // subtract known terms
            ++ops;
        }
        x[i] /= aug[i][i];             // divide by diagonal
        ++ops;
    }

    std::cout << "  Back-substitution operation count: " << ops
              << "  (theory: n*(n+1)/2 = "
              << (long long)n * (n + 1) / 2 << ")\n\n";
    return x;
}

// EXERCISE 3 -- Array -> Max-Heap (heapify)
// Builds a max-heap in-place using Floyd's bottom-up method.
//
// siftDown: restores heap property at node i.
// buildHeap: calls siftDown for all internal nodes,
//            bottom-up from floor(n/2)-1 down to 0.
//
// Time complexity: O(n)

void siftDown(std::vector<int>& arr, int i, int n) {
    while (true) {
        int largest = i;
        int left    = 2 * i + 1;
        int right   = 2 * i + 2;

        if (left  < n && arr[left]  > arr[largest]) largest = left;
        if (right < n && arr[right] > arr[largest]) largest = right;

        if (largest == i) break;

        std::swap(arr[i], arr[largest]);
        i = largest;
    }
}

void buildMaxHeap(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i)
        siftDown(arr, i, n);
}

bool verifyMaxHeap(const std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n / 2; ++i) {
        if (2*i+1 < n && arr[i] < arr[2*i+1]) return false;
        if (2*i+2 < n && arr[i] < arr[2*i+2]) return false;
    }
    return true;
}

// EXERCISE 4 -- LU Decomposition 
// Decomposes A into L*U where:
//   L = lower-triangular with 1s on diagonal
//   U = upper-triangular
// The factorization is derived directly from Gaussian
// elimination: the multipliers used during forward elimination
// become the entries of L; U is the resulting upper-triangular
// matrix.
// Solving A*x = b:
//   Step 1: Forward substitution  L*y = b  --> O(n^2)
//   Step 2: Back    substitution  U*x = y  --> O(n^2)
//   Total: O(n^2) per right-hand side after O(n^3) factorization.

struct LU {
    Matrix L, U;
    std::vector<int> perm;  // permutation from partial pivoting
    int n;
};

LU luDecompose(Matrix A) {
    int n = A.size();
    LU lu;
    lu.n = n;
    lu.L.assign(n, std::vector<double>(n, 0.0));
    lu.U.assign(n, std::vector<double>(n, 0.0));
    lu.perm.resize(n);
    for (int i = 0; i < n; ++i) lu.perm[i] = i;

    // L gets 1s on diagonal
    for (int i = 0; i < n; ++i) lu.L[i][i] = 1.0;

    for (int col = 0; col < n; ++col) {

        // -- partial pivoting
        int pivotRow = col;
        for (int r = col + 1; r < n; ++r)
            if (std::abs(A[r][col]) > std::abs(A[pivotRow][col]))
                pivotRow = r;

        if (std::abs(A[pivotRow][col]) < 1e-12)
            throw std::runtime_error("Singular matrix in LU.");

        if (pivotRow != col) {
            std::swap(A[col], A[pivotRow]);
            std::swap(lu.perm[col], lu.perm[pivotRow]);
            // swap already-computed L entries left of diagonal
            for (int k = 0; k < col; ++k)
                std::swap(lu.L[col][k], lu.L[pivotRow][k]);
        }

        // -- fill U row and L column
        for (int k = col; k < n; ++k)
            lu.U[col][k] = A[col][k];

        for (int row = col + 1; row < n; ++row) {
            double m = A[row][col] / A[col][col];
            lu.L[row][col] = m;           // multiplier -> L
            for (int k = col; k < n; ++k)
                A[row][k] -= m * A[col][k];
        }
    }
    return lu;
}

// Forward substitution: L*y = Pb
std::vector<double> forwardSub(const LU& lu,
                                const std::vector<double>& b) {
    int n = lu.n;
    std::vector<double> y(n, 0.0);
    for (int i = 0; i < n; ++i) {
        y[i] = b[lu.perm[i]];
        for (int j = 0; j < i; ++j)
            y[i] -= lu.L[i][j] * y[j];
        // L[i][i] == 1, no division needed
    }
    return y;
}

// Back substitution: U*x = y
std::vector<double> backSub(const LU& lu,
                             const std::vector<double>& y) {
    int n = lu.n;
    std::vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = y[i];
        for (int j = i + 1; j < n; ++j)
            x[i] -= lu.U[i][j] * x[j];
        x[i] /= lu.U[i][i];
    }
    return x;
}

std::vector<double> luSolve(Matrix A, const std::vector<double>& b) {
    LU lu = luDecompose(A);
    std::cout << "  -- L matrix --\n";
    printMatrix(lu.L, "");
    std::cout << "  -- U matrix --\n";
    printMatrix(lu.U, "");
    std::vector<double> y = forwardSub(lu, b);
    printVec(y, "  y  (L*y = Pb)");
    std::vector<double> x = backSub(lu, y);
    return x;
}


// MAIN

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // ── 4x4 system used for Ex 1, 2, and 4 ─────────────────
    // A*x = b  with known solution x = [1, 2, 3, 4]
    //
    //   2x1 +  1x2 -  1x3 +  3x4 = 16
    //   4x1 +  2x2 +  3x3 +  1x4 = 18
    //  -1x1 +  3x2 +  2x3 +  2x4 = 18
    //   3x1 -  1x2 +  2x3 -  1x4 =  3
    //
    Matrix A = {
        { 2,  1, -1,  3},
        { 4,  2,  3,  1},
        {-1,  3,  2,  2},
        { 3, -1,  2, -1}
    };
    std::vector<double> b = {13, 21, 19, 3};

    // augmented matrix [A|b]
    int n = A.size();
    Matrix aug(n, std::vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) aug[i][j] = A[i][j];
        aug[i][n] = b[i];
    }

    // ── Exercise 1 ───────────────────────────────────────────
    std::cout << "  EXERCISE 1 - Forward Elimination\n";
    printMatrix(aug, "Augmented matrix [A|b] (initial):");
    Matrix upper = forwardElimination(aug);
    printMatrix(upper, "Upper-triangular form after elimination:");

    // ── Exercise 2 ───────────────────────────────────────────
    std::cout << "  EXERCISE 2 - Back Substitution\n";
    std::vector<double> xGauss = backSubstitution(upper);
    printVec(xGauss, "Solution x (Gaussian + back-sub)");
    std::cout << "  Expected: x = [1, 2, 3, 4]\n\n";

    // ── Exercise 3 ───────────────────────────────────────────
    std::cout << "  EXERCISE 3 -- Array to Max-Heap\n";
    std::vector<int> arr = {3, 9, 2, 1, 4, 5, 10, 7, 6, 8};
    std::cout << "Input array:  ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    buildMaxHeap(arr);

    std::cout << "Max-Heap:     ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Heap valid:   "
              << (verifyMaxHeap(arr) ? "YES" : "NO") << "\n";
    std::cout << "\nHeap property: arr[i] >= arr[2i+1] and arr[i] >= arr[2i+2]\n";
    std::cout << "  Root (max element): " << arr[0] << "\n\n";

    // ── Exercise 4 ───────────────────────────────────────────
    std::cout << "  EXERCISE 4 -- LU Decomposition Solver\n";
    printMatrix(A, "Matrix A:");
    printVec(b, "Vector b");

    std::vector<double> xLU = luSolve(A, b);
    printVec(xLU, "Solution x (LU decomposition)");
    std::cout << "  Expected: x = [1, 2, 3, 4]\n\n";

    // verification: compute A*x and compare with b
    std::cout << "  Verification (A*x should equal b):\n";
    for (int i = 0; i < n; ++i) {
        double val = 0.0;
        for (int j = 0; j < n; ++j) val += A[i][j] * xLU[j];
        std::cout << "    row " << i+1 << ":  computed = "
                  << std::fixed << std::setprecision(6) << val
                  << "   b[" << i+1 << "] = " << b[i] << "\n";
    }

    return 0;
}
