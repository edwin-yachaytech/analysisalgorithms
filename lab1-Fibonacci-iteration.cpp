#include <iostream>
using namespace std;

int main() {
    int n;
    long long t0 = 0, t1 = 1, next;

    cout << "Enter the number of terms: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of terms." << endl;
        return 0;
    }

    cout << "Fibonacci sequence:" << endl;

    if (n >= 1)
        cout << t0 << " ";
    if (n >= 2)
        cout << t1 << " ";

    for (int i = 2; i < n; i++) {
        next = t0 + t1;
        cout << next << " ";
        t0 = t1;
        t1 = next;
    }

    cout << endl;
    return 0;
}
