#ifndef DIVISION_QUICK_SORT_H_
#define DIVISION_QUICK_SORT_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]));

// void printArr(int arr[], int len) {
//     for(int i = 0; i < len; i++) {
//         printf("%d ", arr[i]);
//     }
//     printf("\n");
// }

int swapAB(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//基数为中间元素
void qsort(int arr[], int left, int right) {
    if (left > right) {
        return;
    }

    int mid = arr[(right+left)>>1];
    int i = left;
    int j = right;

    do {
        while (arr[i] < mid) {
            i++;
        }

        while (arr[j] > mid) {
            j--;
        }

        if (i <= j) {
            swapAB(&arr[i], &arr[j]);
            i++;
            j--;
        }

    } while(i<=j);

    if (i <= right) {
        qsort(arr, i, right);
    }

    if (left <= j) {
        qsort(arr, left, j);
    }
}

//二分快速排序
int divisionQuickSort(int arr[], int len) {
    if (len -1 > 0) {
        qsort(arr, 0, len-1);
    }
}



//二分查找
int divisionSearch(int arr[], int len, int element) {
    int left = 0;
    int right = len - 1;
    while (left <= right) {
        int mid = (left+right) >> 1;
        if (element < arr[mid]) {
            right = mid - 1;
        }

        if (element > arr[mid]) {
            left = mid + 1;
        }

        if (element == arr[mid]) {
            return mid;
        }
    }
    return -1;
}

#endif