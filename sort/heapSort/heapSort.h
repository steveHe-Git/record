#ifndef HEAPSORT_H_
#define HEAPSORT_H_

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]));

static int len = 0;

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap(int*a, int*b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//堆调整
void heapfiy(int arr[], int index) {
    int left = 2*index + 1;
    int right = 2*index + 2;
    int largest = index;

    if (left < len && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < len && arr[right] > arr[largest]) {
        largest = right;
    }

    if (index != largest) {
        swap(&arr[index], &arr[largest]);
        heapfiy(arr, largest);              //交换数值之后，还需要与下面的所有左右节点进行比较
    }
}

//建立大顶堆
void buildMaxHeap(int arr[], int len) {
    for(int i = len/2; i>=0; i--) {
        heapfiy(arr, i);
    }
}

void heapSort(int arr[], int size) {
    len = size;
    printArr(arr, size);
    buildMaxHeap(arr, size);
    for (int i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]); //交换最大值和最后一个数值
        len--;                  //交换之后最后一个数值为最大值，剩余数值需要重新构建大顶堆，所以len需要－１
        heapfiy(arr, 0);        //只需要从0开始调整堆就行，应该都是已经排序好的,只是0的数值改变了而已
    }
    printArr(arr, size);
}

#endif