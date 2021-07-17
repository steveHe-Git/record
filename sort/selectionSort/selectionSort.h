#ifndef SELECTIONSORT_H_
#define SELECTIONSORT_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]));

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void selectionSort(int a[], int len) {
    //printArr(a,len);
    int min_index, tmp;
    for (int i = 0; i<len - 1; i++) {
        min_index = i;
        for(int j = i + 1; j<len; j++) {
            if (a[j] < a[min_index]) {
                min_index = j;
            }
        }
        tmp = a[i];
        a[i] = a[min_index];
        a[min_index] = tmp;
    }
    //printArr(a,len);
}

#endif