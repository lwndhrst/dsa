#include <stdio.h>

int partition_lomuto(int *arr, int lo, int hi) {
    int p = arr[hi];

    int i = lo - 1;
    for (int j = lo; j < hi; j += 1) {
        if (arr[j] <= p) {
            i += 1;

            // swapperino
            int tmp = arr[i];
            arr[i]  = arr[j];
            arr[j]  = tmp;
        }
    }

    i += 1;
    int tmp = arr[i];
    arr[i]  = arr[hi];
    arr[hi] = tmp;
    return i;
}

void quicksort_lomuto_recursive(int *arr, int lo, int hi) {
    // base case
    if (lo >= hi || lo < 0)
        return;

    // parition
    int p = partition_lomuto(arr, lo, hi);

    // recurse
    quicksort_lomuto_recursive(arr, lo, p - 1); // left side
    quicksort_lomuto_recursive(arr, p + 1, hi); // right side
}

int partition_hoare(int *arr, int lo, int hi) {
    int p = arr[(hi - lo) / 2 + lo];

    int i = lo - 1;
    int j = hi + 1;

    for(;;) {
        do i += 1;
        while (arr[i] < p);

        do j -= 1;
        while (arr[j] > p);

        if (i >= j)
            return j;

        // swapperino
        int tmp = arr[i];
        arr[i]  = arr[j];
        arr[j] = tmp;
    }
}

void quicksort_hoare_recursive(int *arr, int lo, int hi) {
    // base case
    if (lo >= hi || lo < 0)
        return;

    // parition
    int p = partition_hoare(arr, lo, hi);

    // recurse
    quicksort_hoare_recursive(arr, lo, p);     // left side (including p)
    quicksort_hoare_recursive(arr, p + 1, hi); // right side
}

#define ARRAY_INPUT { 2, 12, 7, 13, 1, 5, 3, 1, 9, 6 }
#define ARRAY_LEN 10

int main(void) {
    printf("input array:\n");

    int input[] = ARRAY_INPUT;
    for (int i = 0; i < ARRAY_LEN; i += 1) {
        printf("%d ", input[i]);
    }
    printf("\n\n");

    
    //////////////////////////////////////////////////////////////


    printf("quicksort_lomuto_recursive:\n");

    int arr[] = ARRAY_INPUT;
    quicksort_lomuto_recursive(arr, 0, ARRAY_LEN - 1);
    for (int i = 0; i < ARRAY_LEN; i += 1) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    
    //////////////////////////////////////////////////////////////
    

    printf("quicksort_hoare_recursive:\n");

    int arr2[] = ARRAY_INPUT;
    quicksort_hoare_recursive(arr2, 0, ARRAY_LEN - 1);
    for (int i = 0; i < ARRAY_LEN; i += 1) {
        printf("%d ", arr2[i]);
    }
    printf("\n\n");

    return 0;
}
