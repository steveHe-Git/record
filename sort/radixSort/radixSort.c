#include <stdio.h>

#define LENGTH(arr) (sizeof(arr))/(sizeof(arr[0]))

void printArr(int arr[], int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int getMax(int arr[], int len) {
    int max = 0;
    printf("getMax\n");
    for (int i = 0; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    printf("return max\n");
    return max;
}

void countSort(int arr[], int len, int exp) {
    int output[len];                     // 存储"被排序数据"的临时数组
    int i, buckets[10] = {0};

    // 将数据出现的次数存储在buckets[]中
    for (i = 0; i < len; i++)
    	buckets[ (arr[i]/exp)%10 ]++;

    printArr(buckets, len);
    // 更改buckets[i]。目的是让更改后的buckets[i]的值，是该数据在output[]中的位置。
    for (i = 1; i < 10; i++)
    	buckets[i] += buckets[i - 1];
    printArr(buckets, len);

    // 将数据存储到临时数组output[]中
    for (i = len - 1; i >= 0; i--) {
    	output[buckets[ (arr[i]/exp)%10 ] - 1] = arr[i];
    	buckets[ (arr[i]/exp)%10 ]--;
    }
    printArr(buckets, len);

    // 将排序好的数据赋值给arr[]
    for (i = 0; i < len; i++)
    	arr[i] = output[i];
    printArr(arr, len);
}

void radixSort(int arr[], int length) {
    int max = getMax(arr, length);
    for (int exp = 1; max/exp > 0; exp*=10) {
        countSort(arr, length, exp);
    }
    return;
}

int main() {
    int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
    int length = LENGTH(arr);
    printArr(arr, length);
    radixSort(arr, length);
    printArr(arr, length);
}