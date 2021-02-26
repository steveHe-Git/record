#include <stdio.h>
#define LENGTH(arr) (sizeof(arr))/(sizeof(arr[0]));

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void insertSort(int arr[], int len) {
    int current;
    int preIndex;
    for (int i = 0; i < len; i++) {
        preIndex = i - 1;
        current = arr[i];
        while (preIndex >=0 && arr[preIndex] > current) {
            arr[preIndex + 1] = arr[preIndex];
            preIndex--;
        }
        arr[preIndex + 1] = current;
    }
}

int main() {
    int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
    int length = LENGTH(arr);
    printArr(arr, length);
    insertSort(arr, length);
    printArr(arr, length);
}