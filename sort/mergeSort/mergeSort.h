#ifndef MERGESORT_H_
#define MERGESORT_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]));

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void merge(int a[], int start, int mid, int end) {
#if 1
    int temp[end-start+1];
    int i = start;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= end){
        if (a[i] <= a[j]){
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }

    while (i <= mid){
        temp[k++] = a[i++];
    }
    while (j <= end){
        temp[k++] = a[j++];
    }

    //将临时区域中排序后的元素，整合到原数组中
    for (i = 0; i < k; i++){
        a[start + i] = temp[i];
    }
#else
    int p1 = start;
    int p2 = mid + 1;
    int temp[end-start+1];
    int cur;
    int k = 0;

    //错误的思想，当start=0,mid=0,end=1;循环执行不了
    while (p1 < mid || p2 < end){
        if (p1 == mid) {
            cur = a[p2++];
        } else if (p2 == end) {
            cur= a[p1++];
        } else if (a[p1] > a[p2]) {
            cur = a[p2++];
        } else {
            cur = a[p1++];
        }
        printf("cur: %d\n", cur);
        temp[k++] = cur;
    }

    for (int i = 0; i<k; i++) {
        a[start + i] =  temp[k];
    }

#endif
}

void  merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + ((right - left) >> 1);

        merge_sort(arr, left, mid);
        merge_sort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSort(int arr[], int len){
    printArr(arr, len);
    merge_sort(arr, 0, len - 1);
    printArr(arr, len);
}

#endif