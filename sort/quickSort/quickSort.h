#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]));

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//基数为最左边的元素
void sort (int left, int right, int arr[]) {
    if (left > right) {
        return;
    }

    int base = arr[left];
    int i = left;
    int j = right;
    while (i < j) {
        while(arr[j] >= base) {
            j--;
        }

        while(arr[i] <= base && i < j) {
            i++;
        }

        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }

    //基数归位
    arr[left] = arr[i];
    arr[i] = base;

    sort(left, i-1, arr);
    sort(i+1, right, arr);
}

//基数快排
void quickSort(int a[], int len) {
    if (len-1 >= 0) {
        sort(0, len-1, a);
    }
}

#endif