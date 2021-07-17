#ifndef RADIXSORT_REWRITE_H_
#define RADIXSORT_REWRITE_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr))/(sizeof(arr[0]))

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int getMax(int arr[], int len) {
    int max = arr[0];
    for(int i=0; i<len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

void countSort(int arr[], int len, int exp) {
    int output[len];
    int buckets[10] = {0};

    for (int i=0; i<len; i++) {
        buckets[(arr[i]/exp)%10]++;
    }

    for (int i=1; i< len; i++) {
        buckets[i] += buckets[i-1];
    }

    for(int i = len-1; i>=0; i--) {
        output[buckets[(arr[i]/exp)%10] - 1] = arr[i];
        buckets[(arr[i]/exp)%10]--;
    }

    for (int i=0; i< len; i++) {
        arr[i] = output[i];
    }
}

void radixSort(int arr[], int len) {
    printArr(arr,len);
    int max = getMax(arr, len);
    for (int exp =1; (max/exp) > 0; exp*=10) {
        countSort(arr, len, exp);
    }
    printArr(arr,len);
}

#endif
