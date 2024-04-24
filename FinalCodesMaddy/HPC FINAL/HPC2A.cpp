#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>


#include <omp.h> // OpenMP library for Windows
using namespace std;

// Function to generate an array with random values
void generateRandomArray(int arr[], int size) {
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000; // Generate random numbers between 0 and 999
    }
}

// Function to perform bubble sort in parallel
void parallelBubbleSort(int arr[], int size) {
    bool swapped;

    for (int i = 0; i < size - 1; ++i) {
        swapped = false;

        #pragma omp parallel for shared(arr, swapped)
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped in the inner loop, array is sorted
        if (!swapped)
            break;
    }
}

// Function to perform bubble sort sequentially
void sequentialBubbleSort(int arr[], int size) {
    bool swapped;

    for (int i = 0; i < size - 1; ++i) {
        swapped = false;

        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped in the inner loop, array is sorted
        if (!swapped)
            break;
    }
}

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    int* arr = new int[n];

    // Generate random input data
    generateRandomArray(arr, n);

    // Measure time for sequential bubble sort
    clock_t start = clock();
    sequentialBubbleSort(arr, n);
    clock_t stop = clock();
    double sequential_duration = double(stop - start) / CLOCKS_PER_SEC * 1000;

    cout << "Time taken for sequential bubble sort: " << sequential_duration << " milliseconds" << endl;

    // Generate new random input data
    generateRandomArray(arr, n);

    // Measure time for parallel bubble sort
    start = clock();
    parallelBubbleSort(arr, n);
    stop = clock();
    double parallel_duration = double(stop - start) / CLOCKS_PER_SEC * 1000;

    cout << "Time taken for parallel bubble sort: " << parallel_duration << " milliseconds" << endl;

    delete[] arr;

    return 0;
}

