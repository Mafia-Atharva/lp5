#include <omp.h>
#include <stdlib.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std;

// ---------- Merge Sort ----------
void merge(int *a, int *temp, int i1, int j1, int i2, int j2) {
    int i = i1, j = i2, k = 0;

    while (i <= j1 && j <= j2) {
        if (a[i] < a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }
    while (i <= j1) {
        temp[k++] = a[i++];
    }
    while (j <= j2) {
        temp[k++] = a[j++];
    }
    for (i = i1, j = 0; i <= j2; i++, j++) {
        a[i] = temp[j];
    }
}

void s_mergesort(int *a, int *temp, int i, int j) {
    int mid;
    if (i < j) {
        mid = (i + j) / 2;
        s_mergesort(a, temp, i, mid);
        s_mergesort(a, temp, mid + 1, j);
        merge(a, temp, i, mid, mid + 1, j);
    }
}

void p_mergesort(int *a, int *temp, int i, int j) {
    int mid;
    if (i < j) {
        if ((j - i) > 1000) { // Task creation threshold
            mid = (i + j) / 2;

#pragma omp task shared(a, temp)
            p_mergesort(a, temp, i, mid);
#pragma omp task shared(a, temp)
            p_mergesort(a, temp, mid + 1, j);

#pragma omp taskwait
            merge(a, temp, i, mid, mid + 1, j);
        } else {
            s_mergesort(a, temp, i, j);
        }
    }
}

void parallel_mergesort(int *a, int *temp, int i, int j) {
#pragma omp parallel num_threads(thread::hardware_concurrency())
    {
#pragma omp single
        p_mergesort(a, temp, i, j);
    }
}

// ---------- Helper Functions ----------
std::string bench_traverse(std::function<void()> traverse_fn) {
    auto start = high_resolution_clock::now();
    traverse_fn();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    return std::to_string(duration.count());
}

void print_array(int *a, int n) {
    int limit = min(n, 20);
    for (int i = 0; i < limit; i++) {
        cout << a[i] << ", ";
    }
    cout << "... (showing first " << limit << " elements)\n";
}

// ---------- Main ----------
int main() {
    srand(static_cast<unsigned>(time(0)));

    int *a, *b, *temp, n, rand_max;

    std::cout << "Enter the length of the array: ";
    std::cin >> n;
    std::cout << "Enter the maximum value for random elements: ";
    std::cin >> rand_max;

    if (n <= 0 || rand_max <= 0) {
        std::cout << "Array length and maximum random value must be positive integers.\n";
        return 1;
    }

    a = new int[n];
    b = new int[n];
    temp = new int[n]; // Create temp array once

    for (int i = 0; i < n; i++) {
        a[i] = rand() % rand_max;
    }
    copy(a, a + n, b);

    cout << "Generated random array of length " << n << " with elements between 0 to " << rand_max << "\n\n";

    std::cout << "Sequential Merge sort time: " << bench_traverse([&] { s_mergesort(a, temp, 0, n - 1); }) << " ms\n";
    cout << "Sorted array:\n";
    print_array(a, n);
    cout << "\n";

    std::cout << "Parallel Merge sort time: " << bench_traverse([&] { parallel_mergesort(b, temp, 0, n - 1); }) << " ms\n";
    cout << "Sorted array:\n";
    print_array(b, n);

    delete[] a;
    delete[] b;
    delete[] temp;
    return 0;
}

