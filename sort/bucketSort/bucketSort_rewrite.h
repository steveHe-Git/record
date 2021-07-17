#ifndef BUCKETSORT_REWRITE_H_
#define BUCKETSORT_REWRITE_H_

#include <stdio.h>

#define BUCKET_LEN 5
#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

void printArr(int arr[], int len) {
    for(int i=0; i<len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int insertSort(int arr[], int len) {
    for (int i=1; i<len; i++) {
        int prev = i-1;
        int current = arr[i];
        while(prev >= 0 && current < arr[prev]) {
            arr[prev+1] = arr[prev];
            prev--;
        }
        arr[prev+1] = current;
    }
}

int bucketSort(int arr[], int len) {
    int min = arr[0];
    int max = arr[0];
    for (int i=0; i<len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }

        if(arr[i] < min) {
            min = arr[i];
        }
    }

    int bucket_size = ((max-min) / BUCKET_LEN) + 1;
    int bucket_array[bucket_size][len];

    for (int i=0; i<bucket_size; i++) {
        for (int j=0; j<len; j++) {
            bucket_array[i][j] = -1;
        }
    }

    for(int j=0; j<len; j++) {
        bucket_array[(arr[j] - min)/BUCKET_LEN][j] = arr[j];
    }

    int k = 0;
    for (int i=0; i<bucket_size; i++) {
        insertSort(bucket_array[i], len);
        for(int j=0; j<len; j++) {
            if (bucket_array[i][j] != -1) {
                arr[k++] = bucket_array[i][j];
            }
        }
    }
}

#endif