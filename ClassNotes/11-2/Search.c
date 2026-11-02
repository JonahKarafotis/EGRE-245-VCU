#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <time.h>

#define SIZE 2000000

int seqSearch(int[], int, int);
int binSearch(int[], int, int);

int main(void) {
    // Static storage avoids putting this large array on the stack.
    static int list[SIZE];
    int key = (int)(SIZE * .93172);

    printf("Number of items: %d\n", SIZE);
    for (int i = 0; i < SIZE; i++) {
        list[i] = i;
    }

    printf("key = %d\n", key);

    struct timespec start, end;

    // Time only the search, excluding array setup and printing.
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime");
        return 1;
    }
    int seqCompares = seqSearch(list, SIZE, key);
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        perror("clock_gettime");
        return 1;
    }
    double seqSeconds = (double)(end.tv_sec - start.tv_sec)
                      + (end.tv_nsec - start.tv_nsec) / 1e9;

    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime");
        return 1;
    }
    int binCompares = binSearch(list, SIZE, key);
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        perror("clock_gettime");
        return 1;
    }
    double binSeconds = (double)(end.tv_sec - start.tv_sec)
                      + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("seq. search compares to find key = %d\n", seqCompares);
    printf("seq. search time = %.9f seconds (%.3f microseconds)\n",
           seqSeconds, seqSeconds * 1e6);
    printf("bin. search compares to find key = %d\n", binCompares);
    printf("bin. search time = %.9f seconds (%.3f microseconds)\n",
           binSeconds, binSeconds * 1e6);
    // Single-search timings vary and include timer overhead, especially
    // for binary search, which may finish near the timer's resolution.

    return 0;
}

int seqSearch(int a[], int n, int key) {
    int compares = 0;

    for (int i = 0; i < n; i++) {
        compares++;
        if (a[i] == key) {
            return compares;
        }
    }
    return -1;
}

int binSearch(int a[], int n, int key) {
    int compares = 0, mid = 0;
    int low = 0, high = n - 1;

    while (low <= high) {
        mid = (high + low) / 2;
        ++compares;
        if (a[mid] == key) {
            return compares;
        } else {
            ++compares;
            if (a[mid] < key) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return -1;
}
