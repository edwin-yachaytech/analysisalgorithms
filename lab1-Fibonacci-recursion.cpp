#include <iostream>
using namespace std;

// Recursive function to compute Fibonacci number
long long fibonacci(int n) {
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n;

    cout << "Enter the number of terms: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of terms." << endl;
        return 0;
    }

    cout << "Fibonacci sequence:" << endl;

    for (int i = 0; i < n; i++) {
        cout << fibonacci(i) << " ";
    }

    cout << endl;
    return 0;
}
