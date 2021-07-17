#include "insertSort.h"
#include "gtest/gtest.h"

using namespace testing;

TEST(TestRadixSort, SimpleTest) {
    int arr[] = {53, 3, 542, 748, 14, 214, 154, 63, 616, 649};
    int aim_arr[] = {3, 14, 53, 63, 154, 214, 542, 616, 649, 748};
    int length = LENGTH(arr);
    insertSort(arr, length);
    for (int i =0; i<length; i++) {
         EXPECT_EQ(arr[i], aim_arr[i]);
    }
}

int main(int argc, char** argv) {
    InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}