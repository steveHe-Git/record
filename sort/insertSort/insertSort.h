#ifndef SHELLSORT_H_
#define SHELLSORT_H_

#include <stdio.h>
#define LENGTH(arr) (sizeof(arr))/(sizeof(arr[0]));

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void insertSort(int a[], int len) {
    for (int i=1; i<len; i++) {
        int pre_index = i - 1;
        int current = a[i];
        while(pre_index >= 0 && current < a[pre_index]) {
            a[pre_index + 1] = a[pre_index];
            pre_index--;
        }
        a[pre_index + 1] = current; 
    }
}

// int main() {
//     int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
//     int length = LENGTH(arr);
//     printArr(arr, length);
//     insertSort(arr, length);
//     printArr(arr, length);
// }

#endif