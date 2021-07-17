#ifndef SHELLSORT_H_
#define SHELLSORT_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]));

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap(int *a, int* b) {
    int tmp = *b;
    *b = *a;
    *a = tmp;
}

void shellSort(int arr[], int len) {
    for (int gap = (len >>1); gap > 0; gap = gap>>1) {
        for (int i = gap; i<len; i++) {
            int j = i;
            while (arr[j] < arr[j-gap] && j-gap >=0) {
                swap(&arr[j], &arr[j - gap]);

                //继续与前面的相隔gap的元素进行比较
                j=j-gap;
            }
        }
    }
}

#endif