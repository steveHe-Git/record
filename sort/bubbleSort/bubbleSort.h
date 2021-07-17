#ifndef BUBBLESORT_H_
#define BUBBLESORT_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]));

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void bubbleSort(int a[], int len) {
    int tmp = 0;
    for (int i = 0; i< len; i++) {
        for(int j = i; j<len; j++) {
            if (a[i] > a[j]) {
                tmp = a[j];
                a[j] = a[i];
                a[i] = tmp;
            }
        }
    }
}

#endif