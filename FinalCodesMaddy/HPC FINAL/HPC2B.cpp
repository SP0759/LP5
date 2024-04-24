#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to merge two sorted subarrays into one sorted array
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    vector<int> L(n1), R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Function to perform sequential merge sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Function to perform parallel merge sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    // Generate random input data
    vector<int> arr(n);
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < n; ++i)
        arr[i] = rand() % 1000; // Generate random numbers between 0 and 999

    // Sequential merge sort
    clock_t start = clock();
    sequentialMergeSort(arr, 0, n - 1);
    clock_t stop = clock();
    double sequential_duration = double(stop - start) / CLOCKS_PER_SEC * 1000;

    cout << "Time taken for sequential merge sort: " << sequential_duration << " milliseconds" << endl;

    // Generate new random input data
    for (int i = 0; i < n; ++i)
        arr[i] = rand() % 1000; // Generate random numbers between 0 and 999

    // Parallel merge sort
    start = clock();
    parallelMergeSort(arr, 0, n - 1);
    stop = clock();
    double parallel_duration = double(stop - start) / CLOCKS_PER_SEC * 1000;

    cout << "Time taken for parallel merge sort: " << parallel_duration << " milliseconds" << endl;

    return 0;
}

