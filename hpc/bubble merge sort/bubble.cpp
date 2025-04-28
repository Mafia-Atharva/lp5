#include <omp.h>
#include <stdlib.h>
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std;

void s_bubble(int *, int);
void p_bubble(int *, int);
void swap(int &, int &);

void s_bubble(int *a, int n) {
    for (int i = 0; i < n; i++) {
        int first = i % 2;
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void p_bubble(int *a, int n) {
    for (int i = 0; i < n; i++) {
        int first = i % 2;
#pragma omp parallel for shared(a, first) num_threads(16)
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

std::string bench_traverse(std::function<void()> traverse_fn) {
    auto start = high_resolution_clock::now();
    traverse_fn();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    return std::to_string(duration.count());
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int *a, n, rand_max;

    std::cout << "Enter the length of the array: ";
    std::cin >> n;
    std::cout << "Enter the maximum value for random elements: ";
    std::cin >> rand_max;

    if (n <= 0 || rand_max <= 0) {
        std::cout << "Array length and maximum random value must be positive integers.\n";
        return 1;
    }

    a = new int[n];
    for (int i = 0; i < n; i++) {
        a[i] = rand() % rand_max;
    }

    int *b = new int[n];
    copy(a, a + n, b);

    cout << "Generated random array of length " << n << " with elements between 0 to " << rand_max << "\n\n";

    std::cout << "Sequential Bubble sort: " << bench_traverse([&] { s_bubble(a, n); }) << "ms\n";
    cout << "Sorted array is:\n";
    for (int i = 0; i < n; i++) {
        cout << a[i] << ", ";
    }
    cout << "\n\n";

    omp_set_num_threads(16);
    std::cout << "Parallel (16) Bubble sort: " << bench_traverse([&] { p_bubble(b, n); }) << "ms\n";
    cout << "Sorted array is:\n";
    for (int i = 0; i < n; i++) {
        cout << b[i] << ", ";
    }

    delete[] a;
    delete[] b;

    return 0;
}

