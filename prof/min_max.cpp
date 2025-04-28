//to run code
//g++ -fopenmp min_max.cpp -o min_max
//./min_max 50 20

#include <limits.h>
#include <omp.h>
#include <stdlib.h>
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

auto start = std::chrono::high_resolution_clock::now();
auto stop = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

void s_avg(int arr[], int n) {
    long sum = 0L;
    for (int i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    cout<< sum / long(n);
}

void p_avg(int arr[], int n) {
    long sum = 0L;

    // Parallelize summing over the array using OpenMP reduction
    #pragma omp parallel for reduction(+ : sum) num_threads(16)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    // Compute average outside of parallel section to avoid redundant computation
    long avg = sum / long(n);
    cout << avg;
}

void s_sum(int arr[], int n) {
    long sum = 0L;
    for (int i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    cout << sum;
}

void p_sum(int arr[], int n) {
    long sum = 0L;
    #pragma omp parallel for reduction(+ : sum) num_threads(16)
    for (int i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    cout << sum;
}

void s_max(int arr[], int n) {
    int max_val = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    cout << max_val;
}

void p_max(int arr[], int n) {
    int max_val = INT_MIN;
    #pragma omp parallel for reduction(max : max_val) num_threads(16)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    cout << max_val;
}
void s_min(int arr[], int n) {
    int min_val = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    cout << min_val;
}

void p_min(int arr[], int n) {
    int min_val = INT_MAX;
    #pragma omp parallel for reduction(min : min_val) num_threads(16)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    cout << min_val;
}


std::string bench_traverse(std::function<void()> traverse_fn) {
    auto start = std::chrono::high_resolution_clock::now();
    traverse_fn();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return std::to_string(duration.count());
}

int main(int argc, const char **argv) {

   int n, rand_max;

    // Prompt the user for array length and maximum random value
    std::cout << "Enter array length: ";
    std::cin >> n;
    
    std::cout << "Enter maximum random value: ";
    std::cin >> rand_max;

    // Allocate memory for the array
    int* a = new int[n];

    // Fill the array with random values
    for (int i = 0; i < n; i++) {
        a[i] = rand() % rand_max;  // Random values between 0 and rand_max-1
    }

    // Create another array to demonstrate some processing, such as copying
    int* b = new int[n];
    std::copy(a, a + n, b);  // Copy elements from a to b

    std::cout << "Generated random array of length " << n << " with elements between 0 to " << rand_max << "\n\n";

    std::cout << "Sequential Min: " 
          << bench_traverse([&] { s_min(a, n); }) << "ms\n";

    std::cout << "Parallel (16) Min: " 
            << bench_traverse([&] { p_min(a, n); }) << "ms\n\n";

    std::cout << "Sequential Max: " 
            << bench_traverse([&] { s_max(a, n); }) << "ms)\n";

    std::cout << "Parallel (16) Max: " 
            << bench_traverse([&] { p_max(a, n); }) << "ms)\n\n";

    std::cout << "Sequential Sum: " 
            << bench_traverse([&] { s_sum(a, n); }) << "ms)\n";

    std::cout << "Parallel (16) Sum: " 
            << bench_traverse([&] { p_sum(a, n); }) << "ms)\n\n";
    std::cout << "Sequential Average: " 
            << bench_traverse([&] { s_avg(a, n); }) << "ms)\n";

    std::cout << "Parallel (16) Average: " 
            << bench_traverse([&] { p_avg(a, n); }) << "ms)\n";

    

    return 0;

}
