#include "quickSort.h"
#include "divisionQuickSort.h"
#include "gtest/gtest.h"

using namespace testing;

TEST(TestRadixSort, QuickSortTest) {
    int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
    int aim_arr[] = {3, 14, 53, 63, 154, 214, 542, 616, 649, 748};
    int length = LENGTH(arr);
    quickSort(arr, length);
    for (int i =0; i<length; i++) {
         EXPECT_EQ(arr[i], aim_arr[i]);
    }
}

TEST(TestRadixSort, DivisionQuickSortTest) {
    int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
    int aim_arr[] = {3, 14, 53, 63, 154, 214, 542, 616, 649, 748};
    int length = LENGTH(arr);
    divisionQuickSort(arr, length);
    for (int i =0; i<length; i++) {
         EXPECT_EQ(arr[i], aim_arr[i]);
    }
}

TEST(TestRadixSort, DivisionSearchTest) {
    int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
    int aim_arr[] = {3, 14, 53, 63, 154, 214, 542, 616, 649, 748};
    int length = LENGTH(arr);
    divisionQuickSort(arr, length);
    for (int i =0; i<length; i++) {
         EXPECT_EQ(arr[i], aim_arr[i]);
    }

    int index = divisionSearch(arr, length, 53);
    EXPECT_EQ(index, 2);
}

int main(int argc, char** argv) {
    InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}