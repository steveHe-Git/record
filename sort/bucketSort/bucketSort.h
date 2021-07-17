#ifndef BUCKETSORT_H_
#define BUCKETSORT_H_

#include <stdio.h>
#include <string.h>

#define LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* 插入排序 */
void insertSort(int arr[], int len) {
    for (int i = 1; i < len; i++) {
        int current = arr[i];
        int preIndex = i - 1;
        for (; preIndex >= 0 && arr[preIndex] > current; preIndex--) {
            arr[preIndex + 1] = arr[preIndex];
        }
        arr[preIndex + 1] = current;
    }
}

void bucketSort(int arr[], int len) {
    int minValue = arr[0], maxValue = arr[0];
    for (int i = 0; i < len; i++) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
        }

        if (arr[i] < minValue) {
            minValue = arr[i];
        }
    }

    //设置默认的桶排序的个数是5;
    int bucketSize = 5;
    int bucketCount = ((maxValue - minValue) / bucketSize) + 1;
    int bucketArray[bucketCount][len];

    //初始化
    for (int i = 0; i < bucketCount; i++) {
        for (int j = 0; j < len; j++) {
            bucketArray[i][j] = -1;
        }
    }
    
    // 利用映射函数将数据分配到各个桶中
    for (int i = 0; i < len; i++) {
        bucketArray[(arr[i] - minValue) / bucketSize][i] = arr[i];
    }

    int k = 0;
    for (int i = 0; i < bucketCount; i++) {
        insertSort(bucketArray[i], len);
        //printArr(bucketArray[i], len);
        for (int j = 0; j < len; j++) {
            if (bucketArray[i][j] != -1) {
                arr[k++] = bucketArray[i][j];
            }
        }
    }
}

#endif

// int main() {
//     int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
//     int length = LENGTH(arr);
//     // printArr(arr, length);
//     // insertSort(arr, length);
//     printArr(arr, length);
//     bucketSort(arr, length);
//     printArr(arr, length);
// }