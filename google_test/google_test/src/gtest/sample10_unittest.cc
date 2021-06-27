/*此示例展示了如何使用 Google Test listener API 来实现原始泄漏检查器 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtest/gtest.h"

using ::testing::EmptyTestEventListener;
using ::testing::TestEventListeners;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace 
{
    
class Water
{
private:
    /* data */
    static int allocated;
public:
    void* operator new(size_t allocation_size) {
        ++allocated;
        return malloc(allocation_size);
    }

    void operator delete(void* block) {
        --allocated;
        free(block);
    }

    static int allocate() {
        return allocated;
    }
};

int Water::allocated = 0;

class LeakCheck:public EmptyTestEventListener
{
private:
    /* data */
    void OnTestStart(const TestInfo& test_info) override {
        printf("######### test start\n");
        initially_allocated = Water::allocate();
    }

    void OnTestEnd(const TestInfo& test_info) override {
        int diff = Water::allocate() - initially_allocated;
        EXPECT_LE(diff, 0) << "leak " << diff << " unit(s) of water";
        printf("######### test end\n");
    }

    int initially_allocated;

};

TEST(ListenLeakCheck, DoesNotLeak) {
    auto water = new Water();
    delete water;
}

TEST(ListenLeakCheck, LeakWater) {
    auto water = new Water();
    EXPECT_TRUE(water != nullptr);
}

} // namespace 


int main (int argc, char** argv) {
    InitGoogleTest(&argc, argv);

    bool check_for_leak = false;
    if (argc > 1 && strcmp(argv[1], "--check_for_leak") == 0) {
        check_for_leak = true;
    } else {
        printf("%s\n", "Run this program to enable check leack in"
            "this tests by using --check_for_leak");
    }

    if (check_for_leak) {
        TestEventListeners& listeners = UnitTest::GetInstance()->listeners();

        // We don't need to worry about deleting the new listener later, as
        // Google Test will do it.
        listeners.Append(new LeakCheck);
    }

    return RUN_ALL_TESTS();
}